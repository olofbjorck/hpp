/**
 *	galsim.h
 *	Contains functions for simulating the interactions between particles
 *
 */

#pragma once
#include "graphics.h"
#include <math.h>

/**
 * Particle struct declaration.
 *
 */
typedef struct particle {
	double x, v_x; // x-position x and x-velocity vx
	double y, v_y; // y-position y and y-velocity vy
	double a_x, a_y; // Force exerted on particle
	double mass;  // particle mass.
} particle_t;

/**
 * Simulates the movement of all the particles in particle_t* particles array.
 * The simulation calculates the forces acting on each particle and then
 * updates every particle's state. The simulation is carried out nsteps times,
 * with timestep delta_t.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smoothe calculations.
 * @param nsteps    Number of timesteps to simulate.
 * @param delta_t   Timestep [seconds].
 */
void simulate(
		particle_t* __restrict particles, const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t);

/**
 * Extension of function simulate, showing the movements of the particles
 * graphically during the simulation. Unsuitable for performance testing.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smoothe calculations.
 * @param nsteps    Number of timesteps to simulate.
 * @param delta_t   Timestep [seconds].
 */
void simulateWithGraphics(
		particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const char* __restrict program,
		const unsigned int windowSize,
		const float circleRadius,
		const float circleColour);

/**
 * Calculates and stores the aggregate force exerted on every particle by all
 * other particles in array particle_t* particles.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smooth calculations.
 */
inline void updateParticles(particle_t* __restrict particles,
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

/**
 * Shows the state of the particles graphically.
 *
 * @param particles Information about every particle.
 */
void showGraphics(
		particle_t* __restrict particles,
		const int N,
		const double circleRadius,
		const int circleColour);
