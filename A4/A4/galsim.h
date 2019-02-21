/**
 *	galsim.h
 *	Contains functions for simulating the interactions between particles
 *
 */

#pragma once
#include "modules.h"
#include "graphics.h"
#include <math.h>
#include "quadtree.h"

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
		particles_t* __restrict particles,
		const int N,
		const double G,
		const double eps0,
		const int nsteps,
		const double delta_t,
		const double theta_max);

void updateParticles(particles_t* particles,
		const int N,
		node_t* __restrict root,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max);

void calculateForces(double x, double y,
		node_t* node,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max,
		double* a_x,
		double* a_y);
