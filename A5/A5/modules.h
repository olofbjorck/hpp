#pragma once

// Contains arrays with information about each particle
typedef struct particles {
	double* x;
	double* y;
	double* v_x;
	double* v_y;
	double* mass;
} particles_t;

// Quadtree node
typedef struct node {
	// [0] == childNorthWest, [1] == childNorthEast,
	// [2] == childSouthWest, [3] == childSouthEast
	struct node* children;

	// "Particle" position and mass
	double xCenterOfMass;
	double yCenterOfMass;
	double mass;

	// Node location and size
	double xCenterOfNode;
	double yCenterOfNode;
	double sideHalf;

} node_t;

// Input constants
typedef struct simulationConstants {
	const double* delta_t; // Timestep
	const double* theta_max;
	const double* G; // Gravitational constant
	const double* eps0; // Plummer sphere constant
	const int* N; // Nr of stars to simulate
	const int* nsteps; // Nr of filesteps
	const int* n_threads;
} simulationConstants_t;

// Graphics constants
typedef struct graphicsConstants {
	const char** program;
	const unsigned int* windowSize;
	const float* circleRadius;
	const float* circleColour;
} graphicsConstants_t;

// Argument for threaded function updateParticles()
typedef struct threadData {
	node_t* root;
	particles_t* particles;
	const simulationConstants_t* simulationConstants;
	unsigned int iStart;
	unsigned int iEnd;
} threadData_t;
