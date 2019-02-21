#include "galsim.h"

void printParticles(particles_t* particles, int N);
double printQuadtree(node_t* node);
void printTotalMass(particles_t* particles, int N);
void printCorrectCOM(particles_t* particles, int N);

// Simulate the movement of the particles
void simulate(
		particles_t* __restrict particles,
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
		buildQuadtree(particles, N, root);

		printTotalMass(particles, N);
		printCorrectCOM(particles, N);
		printf("Tree total mass = %lf\n", printQuadtree(root));

		////printf("%s\n", "Updating particles");
		//updateParticles(particles, N, root, G, eps0, delta_t, theta_max);
		////printf("root->mass = %lf\n", root->mass);
		////printf("root->centerOfMass_x = %lf\n", root->centerOfMass_x);
		////printf("root->centerOfMass_y = %lf\n", root->centerOfMass_y);
		if (i == 0)  {
			printParticles(particles, N);
			printf("COM(x, y) = (%lf, %lf)\n", root->xCenterOfMass, root->yCenterOfMass);
			//printf("root->mass = %lf\n", root->mass);
		}
		freeQuadtree(root);
	}
}

/*
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
void updateParticles(particle_t* __restrict particles,
		const int N,
		node_t* __restrict root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max) {


}

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
		////printf("%s\n", "Calculating distance");
		double r_x = particle->x - node->particle->x;
		double r_y = particle->y - node->particle->y;
		double r = sqrt(r_x*r_x + r_y*r_y);

		unsigned int i;
		// Check theta and if box has children
		////printf("%s\n", "Checking theta");
		if (node->children[0] && (2*node->sideHalf)/r > theta_max) {
			// Travel branch
			for(i = 0; i < 4; i++) {
				calculateForces(particle, node->children[i],
						G, eps0, delta_t, theta_max,
						a_x, a_y);
			}

		} else {
			////printf("%s\n", "else if node->particle");
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
*/


double printQuadtree(node_t* node) {
	double mass = node->mass;
	if (node->children[0]) {
		unsigned int i;
		for (i = 0; i < 4; i++) {
			if (!node->children[0]) {
			mass += printQuadtree(node->children[i]);}
		}
	}
	//printf("mass = %lf, (x, y) = (%lf, %lf)\n", node->mass, node->xCenterOfMass, node->yCenterOfMass);
	return mass;
}

void printParticles(particles_t* particles, int N) {
	unsigned int i;
	for (i = 0; i < N; i++) {
		//printf("Particle %d:\n", i);
		////printf("... (x, y) = (%lf, %lf)\n", particles[i].x, particles[i].y);
		////printf("... mass = %lf\n", particles[i].mass);
		//printf("... (x, y) = (%lf, %lf)\n", particles->x[i], particles->y[i]);
	}
}

void printTotalMass(particles_t* particles, int N) {
	unsigned int i;
	double mass = 0;
	for (i = 0; i < N; i++) {
		mass += particles->mass[i];
	}
	printf("particles total mass = %lf\n", mass);
}

void printCorrectCOM(particles_t* particles, int N) {
	double x = 0.0;
	double y = 0.0;
	unsigned int i;
	double mass = 0;
	for (i = 0; i < N; i++) {
		x += particles->x[i]*particles->mass[i];
		y += particles->y[i]*particles->mass[i];
		mass += particles->mass[i];
	}
	x = x/mass;
	y = y/mass;
	printf("Particles COM (x, y) = (%lf, %lf)\n", x, y);
}
