#include "galsim.h"

// Simulate the movement of the particles
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
		//buildQuadtree(particles, root);
		//computeMasses(particles, root);
		//computeForces(particles, root);
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
extern inline void updateParticles(particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const double delta_t) {

	unsigned int i, j; // Loop iterators
	double r = 0.0, r_x = 0.0, r_y = 0.0; // r-vector
	double denom = 0.0; // Denominator

	// Loop for first particle and initialize acceleration to 0.0
	particles[0].a_x = 0.0;
	particles[0].a_y = 0.0;
	for (j = 1; j < N; j++) {
		particles[j].a_x = 0.0;
		particles[j].a_y = 0.0;
		// Calculate r-vector
		r_x = particles[0].x - particles[j].x;
		r_y = particles[0].y - particles[j].y;
		r = sqrt(r_x*r_x + r_y*r_y);
		// Calculate denominator
		denom = r + eps0;
		denom = 1/(denom*denom*denom); // 1 msec faster than using /denom below
		// Calculate acceleration (a_x, a_y == 0 at start due to calloc)
		particles[0].a_x += particles[j].mass*r_x*denom;
		particles[0].a_y += particles[j].mass*r_y*denom;
		// Calculate corresponding acceleration for other particle, using
		// Newton's third law
		particles[j].a_x -= particles[0].mass*r_x*denom;
		particles[j].a_y -= particles[0].mass*r_y*denom;
	}
	// Update velocity
	particles[0].v_x += -G*delta_t*particles[0].a_x;
	particles[0].v_y += -G*delta_t*particles[0].a_y;
	// Update position
	particles[0].x += delta_t*particles[0].v_x;
	particles[0].y += delta_t*particles[0].v_y;

	// Loop remaining particles, assuming acceleration is properly initialized
	for (i = 1; i < N; i++) {
		for (j = i + 1; j < N; j++) {
			// Calculate r-vector
			r_x = particles[i].x - particles[j].x;
			r_y = particles[i].y - particles[j].y;
			r = sqrt(r_x*r_x + r_y*r_y);
			// Calculate denominator
			denom = r + eps0;
			denom = 1/(denom*denom*denom); // 1 msec faster than using /denom below
			// Calculate acceleration (a_x, a_y == 0 at start due to calloc)
			particles[i].a_x += particles[j].mass*r_x*denom;
			particles[i].a_y += particles[j].mass*r_y*denom;
			// Calculate corresponding acceleration for other particle, using
			// Newton's third law
			particles[j].a_x -= particles[i].mass*r_x*denom;
			particles[j].a_y -= particles[i].mass*r_y*denom;
		}
		// Update velocity
		particles[i].v_x += -G*delta_t*particles[i].a_x;
		particles[i].v_y += -G*delta_t*particles[i].a_y;
		// Update position
		particles[i].x += delta_t*particles[i].v_x;
		particles[i].y += delta_t*particles[i].v_y;
	}
}

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
