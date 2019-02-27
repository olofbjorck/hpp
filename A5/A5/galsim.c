#include "galsim.h"

/*******************************************************************************
GLOBAL PTHREAD VARIABLES
*******************************************************************************/

pthread_mutex_t lock;

/*******************************************************************************
STATIC FUNCTION DECLARATIONS
*******************************************************************************/

static void* updateParticles(void* arg);

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
		const double circleRadius,
		const int circleColour);

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

	// Simulation constants
	const int N = *simulationConstants->N;
	const int n_threads = *simulationConstants->n_threads;
	const int nsteps = *simulationConstants->nsteps;

	// Declare threads
	pthread_t threads[n_threads];

	// Declare thread args
	threadData_t** data =
			(threadData_t**) malloc(n_threads*sizeof(threadData_t*));

	// Nr of elements each thread will calculate
	unsigned int workSize = N/n_threads;

	unsigned int i;
	unsigned int j;
	for (i = 0; i < nsteps; i++) {
		node_t* root = (node_t*) malloc(sizeof(node_t));
		buildQuadtree(particles, N, root);

		// Pthreads
		for(j = 0; j < n_threads; j++) {
			// Initialize argument data
			data[j] = (threadData_t*) malloc(sizeof(threadData_t));
			data[j]->root = root;
			data[j]->particles = particles;
			data[j]->simulationConstants = simulationConstants;
			data[j]->threadIdx = j;
			data[j]->workSize = workSize;
			// Make threads
			pthread_create(&threads[j], NULL, updateParticles, (void*) data[j]);
		}

		// Join threads
		void* status;
		for(j = 0; j < n_threads; j++) {
			pthread_join(threads[j], &status);
		}

		// Free quadtree
		freeQuadtree(root);
	}

	// Free threads
	for(i = 0; i < n_threads; i++) {
		free(data[i]);
	}
	free(data);
}

// Simulate the movement of the particles and show graphically
void simulateWithGraphics(
		particles_t* __restrict particles,
		simulationConstants_t* simulationConstants,
		graphicsConstants_t* graphicsConstants) {

	// Simulation constants
	const int N = *simulationConstants->N;
	const int n_threads = *simulationConstants->n_threads;
	const int nsteps = *simulationConstants->nsteps;

	// Graphics constants
	const char* program = *graphicsConstants->program;
	const unsigned int windowSize = *graphicsConstants->windowSize;
	const float circleRadius = *graphicsConstants->circleRadius;
	const float circleColour = *graphicsConstants->circleColour;

	// Initialize graphics handles
	InitializeGraphics((char*) program, windowSize, windowSize);
	SetCAxes(0,1);	// Color axis (so 0 = white, 1 = black)

	// Declare threads
	pthread_t threads[n_threads];

	// Declare thread args
	threadData_t** data = (threadData_t**) malloc(n_threads*sizeof(threadData_t*));

	// Nr of elements each thread will calculate
	unsigned int workSize = N/n_threads;

	unsigned int j;
	unsigned int i;
	for (i = 0; i < nsteps; i++) {
		node_t* root = (node_t*) malloc(sizeof(node_t));
		buildQuadtree(particles, N, root);

		// Pthreads
		for(j = 0; j < n_threads; j++) {
			// Initialize argument data
			data[j] = (threadData_t*) malloc(sizeof(threadData_t));
			data[j]->root = root;
			data[j]->particles = particles;
			data[j]->simulationConstants = simulationConstants;
			data[j]->threadIdx = j;
			data[j]->workSize = workSize;
			// Make threads
			pthread_create(&threads[j], NULL, updateParticles, (void*)data[j]);
		}

		// Join threads
		void* status;
		for(j = 0; j < n_threads; j++) {
			pthread_join(threads[j], &status);
		}
		
		freeQuadtree(root);
		showGraphics(particles, N, circleRadius, circleColour);
	}

	// Free threads
	for(i = 0; i < n_threads; i++) {
		free(data[i]);
	}
	free(data);

	// Remove graphics handles
	FlushDisplay();
	CloseDisplay();
}

/*******************************************************************************
STATIC FUNCTION DEFINITIONS
*******************************************************************************/

static void* updateParticles(void* arg) {

	threadData_t* data = (threadData_t*) arg;

	// Get some constants on stack for speedup
	const double G = *(data->simulationConstants->G);
	const double eps0 = *(data->simulationConstants->eps0);
	const double delta_t = *(data->simulationConstants->delta_t);
	const double theta_max = *(data->simulationConstants->theta_max);

	// Declare acceleration
	double a_x; // x-acceleration
	double a_y; // y-acceleration

	// Loop remaining particles
	unsigned int i;
	unsigned int iStart = data->threadIdx * data->workSize;
	unsigned int iEnd = iStart + data->workSize;
	for (i = iStart; i < iEnd; i++) {

		// Set acceleration to zero
		a_x = 0.0;
		a_y = 0.0;
		const double x = data->particles->x[i];
		const double y = data->particles->y[i];

		// Update acceleration
		calculateForces(
				x, y,
				data->root,
				G, eps0, delta_t, theta_max,
				&a_x, &a_y);

		// Update velocity
		data->particles->v_x[i] += -G * delta_t * a_x;
		data->particles->v_y[i] += -G * delta_t * a_y;

		// Update position
		data->particles->x[i] += delta_t * data->particles->v_x[i];
		data->particles->y[i] += delta_t * data->particles->v_y[i];
	}
	pthread_exit(NULL);
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
		const double circleRadius,
		const int circleColour) {

	ClearScreen();
	unsigned int i;
	for(i = 0; i < N; i++) {
		DrawCircle(particles->x[i], particles->y[i], 1, 1, circleRadius, circleColour);
	}
	Refresh();
	usleep(3000);	// TODO make variable fps
}


/*******************************************************************************
DEBUG FUNCTIONS
*******************************************************************************/

/*
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
