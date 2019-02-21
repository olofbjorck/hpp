#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>

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

void insert(node_t* __restrict node,
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

void subdivide(node_t* node) {

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

node_t* findCorrectChildForParticle(node_t* node, double x, double y) {

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

void freeQuadtree(node_t* node) {

	if (node->children[0]) {
		unsigned int i;
		for (i = 0; i < 4; i++) {
			freeQuadtree(node->children[i]);
		}
	}
	free(node);
}

void initialize(
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
