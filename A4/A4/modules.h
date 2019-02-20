#pragma once

typedef struct particles {
	double* x;
	double* y;
	double* v_x;
	double* v_y;
	double* mass;
} particles_t;

typedef struct node {
	double xCenterOfMass, yCenterOfMass;
	double mass;

	double xCenterOfNode;
	double yCenterOfNode;
	double sideHalf;

	// [0] == childNorthWest, [1] == childNorthEast,
	// [2] == childSouthWest, [3] == childSouthEast
	struct node* children[4];
} node_t;
