#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>

void buildQuadtree(particles_t* __restrict particles,
		const int N,
		node_t* __restrict root) {

	// Initialize Root node
	//printf("\n\n\n\n\n\t\t\t\t\t\t*** BUILDING QUADTREE ***\n\n\n\n\n\n");
	initialize(root, 0.5, 0.5, 0.5);
	
	unsigned int i;
	for (i = 0; i < N; i++) {
		//printf("Inserting particle %d\n", i);
		insert(root, particles->x[i], particles->y[i], particles->mass[i]);
	}
}

void insert(node_t* __restrict node,
		double x, double y, double mass) {

	//printf("%s\n", "Inside insert function");

	if(node->mass) {
		//printf("in node-mass\n");
		// If node contains particle
		// -> is in interior or is occupied leaf
		if(node->children[0] == NULL) {
			//printf("in node->children\n");
			// If node does not have children
			// -> is occupied leaf
			// -> subdivide node
			//printf("subdividing\n");
			subdivide(node);

			// Then, move input to appropriate child
			//printf("inserting new node\n");
			insert(findCorrectChildForParticle(node, x, y),
					x, y, mass);

			// Then, move node values to appropriate child
			//printf("moving root-ish node\n");
			insert(findCorrectChildForParticle(node,
						node->xCenterOfMass,
						node->yCenterOfMass),
					node->xCenterOfMass, node->yCenterOfMass, node->mass);

		} else {
			// else node has children
			// -> is in interior
			// -> can safely move to appropriate child
			//printf("recursing on children\n");
			insert(findCorrectChildForParticle(node, x, y),
					x, y, mass);
		}

	} else {
		// Node is empty leaf
		// -> add node
		//printf("node is empty\n");
		node->xCenterOfMass = x;
		node->yCenterOfMass = y;
		node->mass = mass;
	}
}

void subdivide(node_t* node) {

	unsigned int i;
	for (i = 0; i < 4; i++) {
		//printf("%s\n", "Allocating children");
		node->children[i] = (node_t*) malloc(sizeof(node_t));
	}

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

	//printf("%s\n", "Inside findCorrectChildForParticle");

	if (x >= node->xCenterOfNode) {
		// Indicates R.H. side
		//printf("%s\n", "... R.H. side");
		if (y >= node->yCenterOfNode) {
			// Indicates top part
			//printf("%s\n", "... \t top part");
			return node->children[1];
		} else {
			// Indicates bottom part
			//printf("%s\n", "... \t bottom part");
			return node->children[3];
		}

	} else {
		// Indicates L.H. side
		//printf("%s\n", "... L.H. side");
		if(y >= node->yCenterOfNode) {
			// Indicates top part
			//printf("%s\n", "... \t top part");
			return node->children[0];
		} else {
			// Indicates bottom part
			//printf("%s\n", "... \t bottom part");
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


	//printf("%s\n", "In initialize");
	// Initialize node
	node->xCenterOfMass = 0.0;
	node->yCenterOfMass = 0.0;
	node->mass = 0.0;

	node->xCenterOfNode = xCenterOfNode;
	node->yCenterOfNode = yCenterOfNode;
	node->sideHalf = sideHalf;

	unsigned int i;
	for (i = 0; i < 4; i++) {
		//printf("%s\n", "Initializing children");
		node->children[i] = NULL;
	}
}
