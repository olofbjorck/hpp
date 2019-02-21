#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
STATIC FUNCTION DECLARATIONS
*******************************************************************************/

static inline void insert(node_t* __restrict node,
		double x,
		double y,
		double mass);

static void subdivide(node_t* node);

static node_t* findCorrectChildForParticle(node_t* node,
		double x,
		double y);

static void initialize(
		node_t* node,
		double xCenter, double yCenter, double sideHalf);

/*******************************************************************************
PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/

void buildQuadtree(particles_t* __restrict particles,
		const int N,
		node_t* __restrict root) {

	// Initialize Root node
	initialize(root, 0.5, 0.5, 0.5);

	unsigned int i;
	for (i = 0; i < N; i++) {
		insert(root, particles->x[i], particles->y[i], particles->mass[i]);
	}
}

void freeQuadtree(node_t* node) {

	if (node->children[0]) {
		unsigned int i;
		for (i = 0; i < 4; i++) {
			freeQuadtree(node->children[i]);
		}
	}
	free(node);
}

/*******************************************************************************
STATIC FUNCTION DEFINITIONS
*******************************************************************************/

/**
 * Inserts a particle into the quadtree, using recursion
 *
 * @param node		Recursive node of quadtree (call function using root)
 * @param x			Particle x-coordinate
 * @param y			Particle y-coordinate
 * @param m			Particle mass
 */
//node_t* insert(particle_t* particle, node_t* node);
static inline void insert(node_t* __restrict node,
		double x, double y, double mass) {

	if (node->mass) {
		// If node contains particle -> is in interior or is occupied leaf
		if (node->children[0] == NULL) {
			// If node does not have children -> is occupied leaf -> subdivide
			subdivide(node);

			// Then, move input to appropriate child
			insert(findCorrectChildForParticle(node, x, y),
					x, y, mass);
			// Then, move node values to appropriate child
			insert(findCorrectChildForParticle(node,
						node->xCenterOfMass,
						node->yCenterOfMass),
					node->xCenterOfMass, node->yCenterOfMass, node->mass);

		} else { /* else: node has children:
					-> is in interior -> can safely move to appropriate child */
			insert(findCorrectChildForParticle(node, x, y),
					x, y, mass);
		}

		// Update center of mass and mass of node due to all contained particles
		node->xCenterOfMass = (node->xCenterOfMass * node->mass +
								x * mass)/(node->mass + mass);
		node->yCenterOfMass = (node->yCenterOfMass * node->mass +
								y * mass)/(node->mass + mass);
		node->mass += mass;

	} else {
		// Node is empty leaf -> add particle
		node->xCenterOfMass = x;
		node->yCenterOfMass = y;
		node->mass = mass;
	}
}

/**
 * Subdivides this node (makes it an interior node) by giving it four children.
 *
 * @param node Node of quadtree
 */
static void subdivide(node_t* node) {

	// Allocate children
	unsigned int i;
	for (i = 0; i < 4; i++) {
		node->children[i] = (node_t*) malloc(sizeof(node_t));
	}

	// Initialize children
	initialize(
			node->children[0],
			node->xCenterOfNode - node->sideHalf,
			node->yCenterOfNode + node->sideHalf,
			node->sideHalf/2);
	initialize(
			node->children[1],
			node->xCenterOfNode + node->sideHalf,
			node->yCenterOfNode + node->sideHalf,
			node->sideHalf/2);
	initialize(
			node->children[2],
			node->xCenterOfNode - node->sideHalf,
			node->yCenterOfNode - node->sideHalf,
			node->sideHalf/2);
	initialize(
			node->children[3],
			node->xCenterOfNode + node->sideHalf,
			node->yCenterOfNode - node->sideHalf,
			node->sideHalf/2);
}

/**
 * Finds the child of @param node where @param particle shall be inserted.
 *
 * @param node		Recursive node of quadtree
 * @param x			Particle x-coordinate
 * @param y			Particle y-coordinate
 *
 * @return          A child of @param node where @param particle belongs
 */
static node_t* findCorrectChildForParticle(node_t* node, double x, double y) {

	if (x >= node->xCenterOfNode) {
		// Indicates R.H. side
		if (y >= node->yCenterOfNode) {
			// Indicates top part
			return node->children[1];
		} else {
			// Indicates bottom part
			return node->children[3];
		}

	} else {
		// Indicates L.H. side
		if(y >= node->yCenterOfNode) {
			// Indicates top part
			return node->children[0];
		} else {
			// Indicates bottom part
			return node->children[2];
		}
	}
}

/**
 * Initializes a node.
 *
 * @param node   Node of quadtree
 * @param yTop   Top y-value of node
 * @param yBot   Bottom y-value of node
 * @param xLeft  Left x-value of node
 * @param xRight Right x-value of node
 */
static void initialize(
		node_t* node,
		double xCenterOfNode, double yCenterOfNode, double sideHalf) {

	// Initialize children to NULL
	unsigned int i;
	for (i = 0; i < 4; i++) {
		node->children[i] = NULL;
	}

	// Initialize node particle info
	node->xCenterOfMass = 0.0;
	node->yCenterOfMass = 0.0;
	node->mass = 0.0;

	// Initialize node info
	node->xCenterOfNode = xCenterOfNode;
	node->yCenterOfNode = yCenterOfNode;
	node->sideHalf = sideHalf;
}
