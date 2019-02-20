#include "galsim.h"

// Simulate the movement of the particles
void simulate(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const double theta_max) {


	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		//printf("%s\n", "Allocating root");
		node_t* root = (node_t*) malloc(sizeof(node_t));
		//printf("%s\n", "Building quadtree");
		buildQuadtree(particles, N, root);
		computeCenterOfMass(root);
		updateParticles(particles, N, root, G, eps0, delta_t, theta_max);
		//printf("root->mass = %lf\n", root->mass);
		//printf("root->centerOfMass_x = %lf\n", root->centerOfMass_x);
		//printf("root->centerOfMass_y = %lf\n", root->centerOfMass_y);
		freeQuadtree(root);
	}
}

// Simulate the movement of the particles and show graphically
void simulateWithGraphics(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const double theta_max,
		const char* program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour) {

	// Initialize graphics handles
	InitializeGraphics((char*) program, windowSize, windowSize);
	SetCAxes(0,1);	// Color axis (so 0 = white, 1 = black)

	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		node_t* root = (node_t*) malloc(sizeof(node_t));
		buildQuadtree(particles, N, root);
		computeCenterOfMass(root);
		updateParticles(particles, N, root, G, eps0, delta_t, theta_max);
		freeQuadtree(root);
		showGraphics(particles, N, circleRadius, circleColour);
	}

	// Remove graphics handles
	FlushDisplay();
	CloseDisplay();
}

// See galsim.h for definition
// Updates particle positions
extern inline void updateParticles(particle_t* __restrict particles,
		const int N,
		node_t* __restrict root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max);

// Calculates force exerted on every particle, recursively
void calculateForces(particle_t* __restrict particle,
		node_t* __restrict node,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max) {

	// Get distance particle<->box
	double r_x = particle->x - node->centerOfMass_x;
	double r_y = particle->y - node->centerOfMass_y;
	double r = sqrt(r_x*r_x + r_y*r_y);

	unsigned int i;
	// Check theta and if box has children
	if((2*node->sideHalf)/r > theta_max && node->children[0]) {
		// Travel branch
		for(i = 0; i < 4; i++)
			calculateForces(particle, node->children[i], G, eps0, delta_t, theta_max);

	} else {
		// Treat as particle
		// Calculate denominator
		double denom = r + eps0;
		denom = 1/(denom*denom*denom);
		// Acceleration
		particle->a_x += node->mass*r_x*denom;
		particle->a_y += node->mass*r_y*denom;
	}
}


// Show particles graphically
void showGraphics(
		particle_t* __restrict particles,
		const int N,
		const double circleRadius,
		const int circleColour) {

	ClearScreen();
	unsigned int i;
	for(i = 0; i < N; i++) {
		DrawCircle(particles[i].x, particles[i].y, 1, 1, circleRadius, circleColour);
	}
	Refresh();
	usleep(3000);	// TODO make variable fps
}

void computeCenterOfMass(node_t* node) {

	// If node has children (node is interior node)
	if (node->children[0]) {

		unsigned int i;
		for (i = 0; i < 4; i++) {
			// Recurse
			computeCenterOfMass(node->children[i]);
			// Add to centerOfMass
			node->centerOfMass_x += node->children[i]->centerOfMass_x
				*node->children[i]->mass;
			node->centerOfMass_y += node->children[i]->centerOfMass_y
				*node->children[i]->mass;
		}

		// Compute centerOfMass
		node->centerOfMass_x = node->centerOfMass_x/node->mass;
		node->centerOfMass_y = node->centerOfMass_y/node->mass;

		// else: Node is leaf
	} else {
		// If node is occupied leaf
		if (node->particle) {
			// Set center of mass
			node->centerOfMass_x = node->particle->x;
			node->centerOfMass_y = node->particle->y;
		}
		// else: do nothing, centerOfMass is 0 as default
	}
}

void printParticles(particle_t* particles, int N) {
	unsigned int i;
	for (i = 0; i < N; i++) {
		printf("Particle %d:\n", i);
		printf("... (x, y) = (%lf, %lf)\n", particles[i].x, particles[i].y);
		printf("... mass = %lf\n", particles[i].mass);
	}
}

void printTotalMass(particle_t* particles, int N) {
	unsigned int i;
	double mass = 0;
	for (i = 0; i < N; i++) {
		mass += particles[i].mass;
	}
	printf("Total mass = %lf\n", mass);
}
