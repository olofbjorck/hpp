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
		////printf("%s\n", "Allocating root");
		node_t* root = (node_t*) malloc(sizeof(node_t));
		//printf("%s\n", "Building quadtree");
		buildQuadtree(particles, N, root);
		//printf("%s\n", "Computing center of mass");
		computeCenterOfMass(root);
		//printf("%s\n", "Updating particles");
		updateParticles(particles, N, root, G, eps0, delta_t, theta_max);
		////printf("root->mass = %lf\n", root->mass);
		////printf("root->centerOfMass_x = %lf\n", root->centerOfMass_x);
		////printf("root->centerOfMass_y = %lf\n", root->centerOfMass_y);
		//printf("%s\n", "Freeing quadtree");
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
void calculateForces(particle_t* particle,
		node_t* node,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max,
		double* a_x,
		double* a_y) {

	if (node->particle) {
		// Get distance particle<->box
		//printf("%s\n", "Calculating distance");
		double r_x = particle->x - node->particle->x;
		double r_y = particle->y - node->particle->y;
		double r = sqrt(r_x*r_x + r_y*r_y);

		unsigned int i;
		// Check theta and if box has children
		//printf("%s\n", "Checking theta");
		if (node->children[0] && (2*node->sideHalf)/r > theta_max) {
			// Travel branch
			for(i = 0; i < 4; i++) {
				calculateForces(particle, node->children[i],
						G, eps0, delta_t, theta_max,
						a_x, a_y);
			}

		} else {
			//printf("%s\n", "else if node->particle");
			// Treat as particle
			// Calculate denominator
			double denom = r + eps0;
			denom = 1/(denom*denom*denom);
			// Acceleration
			*a_x += node->particle->mass*r_x*denom;
			*a_y += node->particle->mass*r_y*denom;
			//particle->a_x += node->mass*r_x*denom;
			//particle->a_y += node->mass*r_y*denom;
		}
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

		// Allocate interior node particle
		node->particle = (particle_t*) malloc(sizeof(particle_t));
		node->particle->mass = 0.0;

		unsigned int i;
		for (i = 0; i < 4; i++) {
			// Recurse
			computeCenterOfMass(node->children[i]);

			if (node->children[i]->particle) {
				// Compute mass
				node->particle->mass += node->children[i]->particle->mass;

				// Add to centerOfMass
				node->particle->x += node->children[i]->particle->x *
									 node->children[i]->particle->mass;
				node->particle->y += node->children[i]->particle->y *
									 node->children[i]->particle->mass;
			}
		}

		// Compute centerOfMass
		node->particle->x = node->particle->x/node->particle->mass;
		node->particle->y = node->particle->y/node->particle->mass;

	// else: Node is leaf
	} else {
		// If node is unoccupied leaf
		/*if (!node->particle) {
			// Allocate empty leaf particle
			node->particle = (particle_t*) malloc(sizeof(particle_t));
			node->particle->mass = 0.0;
			node->particle->x = 0.0;
			node->particle->y = 0.0;
		}*/
		// else: do nothing, centerOfMass is 0 as default
	}
}


void printParticles(particle_t* particles, int N) {
	unsigned int i;
	for (i = 0; i < N; i++) {
		//printf("Particle %d:\n", i);
		//printf("... (x, y) = (%lf, %lf)\n", particles[i].x, particles[i].y);
		//printf("... mass = %lf\n", particles[i].mass);
	}
}

void printTotalMass(particle_t* particles, int N) {
	unsigned int i;
	double mass = 0;
	for (i = 0; i < N; i++) {
		mass += particles[i].mass;
	}
	//printf("Total mass = %lf\n", mass);
}
