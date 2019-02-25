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

// Argument for threaded function updateParticles()
typedef struct threadData {
	node_t* root;
	particles_t* particles;
	const int* N;
	const double* G;
	const double* eps0;
	const double* delta_t;
	const double* theta_max;
	int threadIdx;
	int workSize;
} threadData_t;
