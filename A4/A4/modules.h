#pragma once

typedef struct particles {
	double* x;
	double* y;
	double* v_x;
	double* v_y;
	double* mass;
} particles_t;

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
