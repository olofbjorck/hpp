#include "galsim.h"
#include <time.h>

#define GRAPHICS_FPS 30

/*******************************************************************************
  STATIC FUNCTION DECLARATIONS
 *******************************************************************************/

static void updateParticles(
		node_t* __restrict root,
		particles_t* __restrict particles,
		simulationConstants_t* __restrict simulationConstants);

static void calculateForces(
		double x,
		double y,
		node_t* __restrict node,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max,
		double* __restrict a_x,
		double* __restrict a_y);

static void showGraphics(
		particles_t* __restrict particles,
		const int N,
		graphicsConstants_t* __restrict graphicsConstants);

/* Debug funcs
   void printParticles(particles_t* particles, int N);
   double printQuadtree(node_t* node);
   void printTotalMass(particles_t* particles, int N);
   void printCorrectCOM(particles_t* particles, int N);
   */

/*******************************************************************************
  FUNCTION DEFINITIONS
 *******************************************************************************/

// Simulate the movement of the particles
void simulate(
		particles_t* __restrict particles,
		simulationConstants_t* __restrict simulationConstants) {

	// Set number of threads
	#ifdef _OPENMP
	omp_set_num_threads(*simulationConstants->n_threads);
	#endif

	// Create root
	node_t root;

	// Simulate
	unsigned int i;
	for (i = 0; i < *simulationConstants->nsteps; i++) {

		// Build quadtree
		buildQuadtree(particles, *simulationConstants->N, &root);

		// Update particles
		updateParticles(&root, particles, simulationConstants);

		// Free quadtree
		freeQuadtree(&root);
	}

}


// Simulate the movement of the particles and show graphically
void simulateWithGraphics(
		particles_t* __restrict particles,
		simulationConstants_t* simulationConstants,
		graphicsConstants_t* graphicsConstants) {

	// Graphics constants
	const char* program = *graphicsConstants->program;
	const unsigned int windowSize = *graphicsConstants->windowSize;

	// Initialize graphics handles
	InitializeGraphics((char*) program, windowSize, windowSize);
	SetCAxes(0,1);	// Color axis (so 0 = white, 1 = black)

	// Create root
	node_t root;

	// Simulate
	unsigned int i;
	double loopTimer;
	for (i = 0; i < *simulationConstants->nsteps; i++) {
		clock_t timeBefore = clock();	// for fps

		// Build quadtree
		buildQuadtree(particles, *simulationConstants->N, &root);

		// Update particles
		updateParticles(&root, particles, simulationConstants);

		// Free quadtree
		freeQuadtree(&root);

		// Variable fps
		loopTimer = (double) (clock() - timeBefore)/CLOCKS_PER_SEC;	//Time in seconds
		if(loopTimer < 1.0/GRAPHICS_FPS) {
			usleep(1000000*(1.0/GRAPHICS_FPS - loopTimer));	// Time in useconds
		}

		// Show graphics
		showGraphics(particles, *simulationConstants->N, graphicsConstants);

	}

	// Remove graphics handles
	FlushDisplay();
	CloseDisplay();

}

/*******************************************************************************
  STATIC FUNCTION DEFINITIONS
 *******************************************************************************/

static void updateParticles(
		node_t* __restrict root,
		particles_t* __restrict particles,
		simulationConstants_t* __restrict simulationConstants) {

	// Get some constants on stack for speedup
	const double G = *(simulationConstants->G);
	const double eps0 = *(simulationConstants->eps0);
	const double delta_t = *(simulationConstants->delta_t);
	const double theta_max = *(simulationConstants->theta_max);
	const int N = *(simulationConstants->N);

	// Loop particles
	unsigned int i;
	#pragma omp parallel
	{
		#pragma omp for schedule(auto)
			for (i = 0; i < N; i++) {

				// Set acceleration to zero
				double a_x = 0.0;
				double a_y = 0.0;
				const double x = particles->x[i];
				const double y = particles->y[i];

				// Update acceleration
				calculateForces(
						x, y,
						root,
						G, eps0, delta_t, theta_max,
						&a_x, &a_y);

				// Update velocity
				particles->v_x[i] += -G * delta_t * a_x;
				particles->v_y[i] += -G * delta_t * a_y;

				// Update position
				particles->x[i] += delta_t * particles->v_x[i];
				particles->y[i] += delta_t * particles->v_y[i];
			}
	}
}

// Calculates force exerted on every particle, recursively
static void calculateForces(
		const double x,
		const double y,
		node_t* __restrict node,
		const double G,
		const double eps0,
		const double delta_t,
		const double theta_max,
		double* __restrict a_x,
		double* __restrict a_y) {

	// Get distance particle<->box
	double r_x = x - node->xCenterOfMass;
	double r_y = y - node->yCenterOfMass;
	double r = sqrt(r_x*r_x + r_y*r_y);

	// Check if box has children, then theta
	if (node->children &&
			(node->sideHalf + node->sideHalf) > theta_max * r)  {
		// Travel branch
		unsigned int i;
		for(i = 0; i < 4; i++) {
			calculateForces(
					x, y,
					node->children + i,
					G, eps0, delta_t, theta_max,
					a_x, a_y);
		}
	} else {
		// Calculate denominator
		double denom = r + eps0;
		denom = 1/(denom*denom*denom);
		// Acceleration
		*a_x += node->mass * r_x * denom;
		*a_y += node->mass * r_y * denom;
	}
}

// Show particles graphically
static void showGraphics(
		particles_t* __restrict particles,
		const int N,
		graphicsConstants_t* __restrict graphicsConstants) {

	ClearScreen();
	unsigned int i;
	for(i = 0; i < N; i++) {
		DrawCircle(particles->x[i], particles->y[i], 1, 1,
				*(graphicsConstants->circleRadius), *(graphicsConstants->circleColour));
	}
	Refresh();
	usleep(3000);	
}
