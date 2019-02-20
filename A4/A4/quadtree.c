#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>

void buildQuadtree(particle_t* particles, int N, node_t* root) {

	// Initialize Root node
	printf("\n\n\n\n\n\t\t\t\t\t\t*** BUILDING QUADTREE ***\n\n\n\n\n\n");
	initialize(root, 0.5, 0.5, 0.5);

	unsigned int i;
	for (i = 0; i < N; i++) {
		printf("Inserting particle %d\n", i);
		printf("%lu\n", sizeof(*root));
		printf("Inserting particle %d\n", i);
		insert(&particles[i], root);
	}
}

// varför returnera node_t*?
//node_t* insert(particle_t* particle, node_t* node) {
void insert(particle_t* particle, node_t* node) {

	printf("%s\n", "Inside insert function");

	//node->particle->mass += particle->mass;
	// Node already contains a particle - is an occupied leaf?
	if (node->particle) {

		printf("%s\n", "In node->particle:");

		// Make non-leaf
		printf("%s\n", "... making non-leaf");
		particle_t* tempParticle;
		printf("%p\n", node->particle);
		tempParticle = node->particle;
		node->particle = NULL;
		//node->mass = 0.0;


		printf("\t%p\n", tempParticle);

		// Subdivide the node (make it an interior node)
		printf("%s\n", "... subdividing");
		subdivide(node);

		// Recursively insert on the correct child of this node
		// Insert this node's old particle
		printf("%s\n", "Recursing on tempParticle");
		insert(tempParticle, findCorrectChildForParticle(tempParticle, node));
		// Insert the new particle
		printf("%s\n", "Recursing on new particle");
		insert(particle, findCorrectChildForParticle(particle, node));
	// else: We've found an empty leaf or an interior node
	} else {
		printf("%s\n", "In else (node->particle == NULL)");
		// Is this an interior node?
		if (node->children[0]) {
			printf("%s\n", "In node->childNorthWest");
			// Recurse on the correct child
			printf("%s\n", "Recursing on new particle");
			insert(particle, findCorrectChildForParticle(particle, node));
		// else: This is an empty leaf
		} else {
			printf("%s\n", "In else (node->childNorthWest == NULL)");
			// Add particle to node, make it an occupied leaf
			node->particle = particle;
			printf("%s\n", "Inserted particle in empty leaf!\n");
		}
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
			node->xCenter - node->sideHalf,
			node->yCenter + node->sideHalf,
			node->sideHalf/2);

	initialize(
			node->children[1],
			node->xCenter + node->sideHalf,
			node->yCenter + node->sideHalf,
			node->sideHalf/2);

	initialize(
			node->children[2],
			node->xCenter - node->sideHalf,
			node->yCenter - node->sideHalf,
			node->sideHalf/2);

	initialize(
			node->children[3],
			node->xCenter + node->sideHalf,
			node->yCenter - node->sideHalf,
			node->sideHalf/2);
}

node_t* findCorrectChildForParticle(particle_t* particle, node_t* node) {

	printf("%s\n", "Inside findCorrectChildForParticle");

	if (particle->x >= node->xCenter) {
		// Indicates R.H. side
		printf("%s\n", "... R.H. side");
		if (particle->y >= node->yCenter) {
			// Indicates top part
			printf("%s\n", "... \t top part");
			return node->children[1];
		} else {
			// Indicates bottom part
			printf("%s\n", "... \t bottom part");
			return node->children[3];
		}

	} else {
		// Indicates L.H. side
		printf("%s\n", "... L.H. side");
		if(particle->y >= node->yCenter) {
			// Indicates top part
			printf("%s\n", "... \t top part");
			return node->children[0];
		} else {
			// Indicates bottom part
			printf("%s\n", "... \t bottom part");
			return node->children[2];
		}
	}
}

void printQuadtree(node_t* node) {

}

void freeQuadtree(node_t* node) {

	if (node->children[0]) {
		unsigned int i;
		for (i = 0; i < 4; i++) {
			freeQuadtree(node->children[i]);
		}
		free(node->particle);
	}
	free(node);
}

void initialize(
		node_t* node,
		double xCenter, double yCenter, double sideHalf) {


			printf("%s\n", "In initialize");
	// Initialize node
	node->particle = NULL;
	//node->centerOfMass_x = 0.0;
	//node->centerOfMass_y = 0.0;
	//node->mass = 0.0;

	node->xCenter = xCenter;
	node->yCenter = yCenter;
	node->sideHalf = sideHalf;

	unsigned int i;
	for (i = 0; i < 4; i++) {
		//printf("%s\n", "Initializing children");
		node->children[i] = NULL;
	}
}

node_t* find(particle_t* particle, node_t* node) {

	// If node has no children (node is leaf)
	if (node->children[0]) {
		return find(particle, findCorrectChildForParticle(particle, node));
	} else {
		return node;
		/*
		if (node->particle &&
			particle->x == node->centerOfMass_x &&
			particle->y == node->centerOfMass_y) {
				return node;
		} else {
			printf("%s\n", "ERROR: Particle could not be found");
			return node;
		}*/
	}
}
