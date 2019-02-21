#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>

/*******************************************************************************
  STATIC FUNCTION DECLARATIONS
 *******************************************************************************/

static inline void insert(
		node_t* __restrict node,
		double x,
		double y,
		double mass);

static void subdivide(node_t* node);

static node_t* findCorrectChildForParticle(
		node_t* node,
		double x,
		double y)__attribute__((pure));

static void initialize(
		node_t* node,
		double xCenter, double yCenter, double sideHalf);

/*******************************************************************************
  PUBLIC FUNCTION DEFINITIONS
 *******************************************************************************/

void buildQuadtree(
		particles_t* __restrict particles,
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

	if (node->children) {
		unsigned int i;
		for (i = 0; i < 4; i++) {
			freeQuadtree(node->children+i);
		}
		free(node->children);
	}
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
 * @param mass		Particle mass
 */
static inline void insert(
		node_t* __restrict node,
		double x,
		double y,
		double mass) {

	if(node->children) {
		// If node has children -> is interior -> recurse on child
		insert(findCorrectChildForParticle(node, x, y),
				x, y, mass);

		// Update center of mass and mass of node due to new particle
		const double newMass = node->mass + mass;
		node->xCenterOfMass = (node->xCenterOfMass * node->mass
				+ x * mass)/newMass;
		node->yCenterOfMass = (node->yCenterOfMass * node->mass
				+ y * mass)/newMass;
		node->mass = newMass;

	} else {
		// If node does not have children -> is a leaf

		if(node->mass) {
			// If leaf is already occupied -> subdivide
			subdivide(node);

			// Then, move input to appropriate child
			insert(findCorrectChildForParticle(node, x, y),
					x, y, mass);
			// Then, move node values to appropriate child
			insert(findCorrectChildForParticle(node,
						node->xCenterOfMass,
						node->yCenterOfMass),
					node->xCenterOfMass, node->yCenterOfMass, node->mass);

			// Update center of mass and mass of node due to new particle
			const double newMass = node->mass + mass;
			node->xCenterOfMass = (node->xCenterOfMass * node->mass
					+ x * mass)/newMass;
			node->yCenterOfMass = (node->yCenterOfMass * node->mass
					+ y * mass)/newMass;
			node->mass = newMass;

		} else {
			// Leaf is not occupied -> simply add particle
			node->xCenterOfMass = x;
			node->yCenterOfMass = y;
			node->mass = mass;
		}
	}
}

/**
 * Subdivides this node (makes it an interior node) by giving it four children.
 *
 * @param node Node of quadtree
 */
static inline void subdivide(node_t* node) {

	// Allocate children
	node_t* children = (node_t*) malloc(4 * sizeof(node_t));
	node->children = children;

	// Calculate side length and all centers beforehand
	const double sideQuarter = node->sideHalf/2;
	const double xLeft = node->xCenterOfNode - node->sideHalf;
	const double xRight = node->xCenterOfNode + node->sideHalf;
	const double yTop = node->yCenterOfNode + node->sideHalf;
	const double yBot = node->yCenterOfNode - node->sideHalf;

	// Initialize children
	initialize(
			node->children,
			xLeft,
			yTop,
			sideQuarter);
	initialize(
			node->children+1,
			xRight,
			yTop,
			sideQuarter);
	initialize(
			node->children+2,
			xLeft,
			yBot,
			sideQuarter);
	initialize(
			node->children+3,
			xRight,
			yBot,
			sideQuarter);
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

	if (x > node->xCenterOfNode) {
		// Indicates R.H. side
		if (y > node->yCenterOfNode) {
			// Indicates top part
			return node->children+1;
		} else {
			// Indicates bottom part
			return node->children+3;
		}

	} else {
		// Indicates L.H. side
		if(y > node->yCenterOfNode) {
			// Indicates top part
			return node->children;
		} else {
			// Indicates bottom part
			return node->children+2;
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
		double xCenterOfNode,
		double yCenterOfNode,
		double sideHalf) {

	// Initialize children to NULL
	node->children = NULL;

	// Initialize node particle info
	node->xCenterOfMass = 0.0;
	node->yCenterOfMass = 0.0;
	node->mass = 0.0;

	// Initialize node info
	node->xCenterOfNode = xCenterOfNode;
	node->yCenterOfNode = yCenterOfNode;
	node->sideHalf = sideHalf;
}
