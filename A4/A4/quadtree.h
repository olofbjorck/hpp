/**
 *	quadtree.h
 * 	Functions for creating and modifying a quadtree data structure
 *
 */

#pragma once
#include "galsim.h"

/**
 * Quadtree node struct declaration.
 *
 */
// Tänker att vi börjar med en stor, ful, och enkel node så kan vi
// slimma den sen när vi optimerar
typedef struct node {
	particle_t* particle;	// Particle in leaf
	double centerOfMass_x; // Center of mass x-value
	double centerOfMass_y; // Center of mass y-value
	double mass; // Mass of the node

	double x; // Center x-value
	double y; // Center y-value
	double side; // Side of the node quadrant
	// Cannot refer to typedef within itself, use struct
	//struct node* parent; // Parent node redundant?
	struct node* childNorthWest; // Upper left quadrant child
	struct node* childNorthEast; // Upper right quadrant child
	struct node* childSouthWest; // Lower left quadrant child
	struct node* childSouthEast; // Lower right quadrant child

	unsigned int filled; // Are there any particles in this node or not?
	unsigned int hasChildren;	// Does this node have any subnodes?
} node_t;

/**
 * Builds a quadtree of size N from a root node, and fills it with particles
 *
 * @param particles	Array of particles
 * @param N			Total number of particles
 * @param root		Root node of quadtree
 */
void buildQuadTree(particle_t* particles, int N, node_t* root);

/**
 * Inserts a particle into the quadtree, using recursion
 *
 * @param particle	Particle to insert
 * @param node		Node of quatree
 * @return 			Returns 0 if insert was successful, 1 otherwise
 */
int insert(particle_t* particle, node_t* node);

/**
 * Subdivides a quadtree node into four subnodes
 *
 * @param node	Quadtree node to subdivide
 */
void subdivide(node_t* node);
