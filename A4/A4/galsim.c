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
		updateParticles(particles, N, root, G, eps0, delta_t, theta_max);
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
*/

void updateParticles(particles_t* particles,
		const int N,
		node_t* __restrict root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max) {

	unsigned int i; // Loop iterators
	double a_x; // x-acceleration
	double a_y; // y-acceleration

	// Loop remaining particles
	for (i = 0; i < N; i++) {

		// Set acceleration to zero
		a_x = 0.0;
		a_y = 0.0;

		// Update acceleration
		calculateForces(particles->x[i], particles->y[i], root,
				G, eps0, delta_t, theta_max,
				&a_x, &a_y);

		// Update velocity
		particles->v_x[i] += -G*delta_t*a_x;
		particles->v_y[i] += -G*delta_t*a_y;

		// Update position
		particles->x[i] += delta_t*particles->v_x[i];
		particles->y[i] += delta_t*particles->v_y[i];
	}
}

// Calculates force exerted on every particle, recursively
void calculateForces(double x, double y,//particles_t* particle,
		node_t* node,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max,
		double* a_x,
		double* a_y) {

		// Get distance particle<->box
		double r_x = x - node->xCenterOfMass;
		double r_y = y - node->yCenterOfMass;
		double r = sqrt(r_x*r_x + r_y*r_y);

		unsigned int i;
		// Check theta and if box has children
		if (node->children[0] && (2*node->sideHalf)/r > theta_max) {
			// Travel branch
			for(i = 0; i < 4; i++) {
				calculateForces(x, y, node->children[i],
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
			*a_x += node->mass*r_x*denom;
			*a_y += node->mass*r_y*denom;
		}
}
/*

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
*/
