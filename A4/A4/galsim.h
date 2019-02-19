/**
 *	galsim.h
 *	Contains functions for simulating the interactions between particles
 *
 */

#pragma once
#include "modules.h"
#include "graphics.h"
#include <math.h>

/**
 * Particle struct declaration.
 *
 */


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
/*
inline void updateParticles(particle_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const double delta_t) {

	unsigned int i, j; // Loop iterators
	double r = 0.0, r_x = 0.0, r_y = 0.0; // r-vector
	double denom = 0.0; // Denominator
	//double theta;

	for (i = 0; i < N; i++) {
		particles[i].a_x = 0.0;
		particles[i].a_y = 0.0;
	}

	// Loop remaining particles, assuming acceleration is properly initialized
	for (i = 0; i < N; i++) {
		//getLeaf();
		//compareWithSiblings();

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
*/

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

void computeCenterOfMass(node_t* node);
