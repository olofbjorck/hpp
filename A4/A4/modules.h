#pragma once

typedef struct particle {
	double x, v_x; // x-position x and x-velocity vx
	double y, v_y; // y-position y and y-velocity vy
	//double a_x, a_y; // Force exerted on particle
	double mass;  // particle mass.
} particle_t;

typedef struct node {

	particle_t* particle;	// Particle in leaf
	double centerOfMass_x; // Center of mass x-value
	double centerOfMass_y; // Center of mass y-value
	double mass; // Mass of the node

	double xCenter;
	double yCenter;
	double sideHalf;

	// [0] == childNorthWest, [1] == childNorthEast,
	// [2] == childSouthWest, [3] == childSouthEast
	struct node* children[4];

} node_t;
