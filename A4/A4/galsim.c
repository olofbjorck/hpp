#include "galsim.h"

// Simulate the movement of the particles
/*
void simulate(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t) {

	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		//updateParticles(particles, N, G, eps0, delta_t);
		buildQuadtree(particles, root);
		//computeMasses(particles, root);
		//computeForces(particles, root);
	}
}*/




// Simulate the movement of the particles and show graphically
void simulateWithGraphics(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const char* program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour) {

	// Initialize graphics handles
	InitializeGraphics((char*) program, windowSize, windowSize);
	SetCAxes(0,1);	// Color axis (so 0 = white, 1 = black)

	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		updateParticles(particles, N, G, eps0, delta_t);
		showGraphics(particles, N, circleRadius, circleColour);
	}

	// Remove graphics handles
	FlushDisplay();
	CloseDisplay();
}

// Calculates force exerted on every particle
// See galsim.h for definition
extern void updateParticles(particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const double delta_t);

// Show particles graphically
inline void showGraphics(
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

extern void updateTreeForces(particle_t* particle, ) {


}
