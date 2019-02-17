#include "quadtree.h"

void printQuadTree(node_t* node);
void freeQuadTree(node_t* node);

void buildQuadtree(particle_t* particles, int N, node_t* root) {

	unsigned int i;
	for (i = 0; i < N; i++) {
		printf("Inserting particle %d\n", i);
		//printf("%lu\n", sizeof(*root));
		insert(&particles[i], root);
	}
}

// varför returnera node_t*?
//node_t* insert(particle_t* particle, node_t* node) {
void insert(particle_t* particle, node_t* node) {

	// NULL node - make a leaf and add particle
	/*
	if(!node) {
		return 1;
	}*/

	printf("%s\n", "Inside insert function");

	// Node already contains a particle - is an occupied leaf?
	if (node->particle) {

		printf("%s\n", "In node->particle:");

		// Make non-leaf
		printf("%s\n", "... making non-leaf");
		particle_t* tempParticle = node->particle;
		node->particle = NULL;

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
		if (node->childNorthWest) {
			printf("%s\n", "In node->childNorthWest");
			// Recurse on the correct child
			printf("%s\n", "Recursing on new particle");
			insert(particle, findCorrectChildForParticle(particle, node));

		// else: This is an empty leaf
		} else {
			printf("%s\n", "In else (node->childNorthWest == NULL)");
			// Add particle to node, make it an occupied leaf
			node->particle = particle;
		}
	}
}

void subdivide(node_t* node) {
	printf("%s\n", "... allocating:\tnorth west");
	node->childNorthWest = (node_t*) malloc(sizeof(node_t));
	printf("%s\n", "... \t\tnorth east");
	node->childNorthEast = (node_t*) malloc(sizeof(node_t));
	printf("%s\n", "... \t\tsouth west");
	node->childSouthWest = (node_t*) malloc(sizeof(node_t));
	printf("%s\n", "... \t\tsouth east");
	node->childSouthWest = (node_t*) malloc(sizeof(node_t));
}

node_t* findCorrectChildForParticle(particle_t* particle, node_t* node) {

	if(
			particle->x < node->rightBorder &&
			particle->x >= (node->leftBorder - node->rightBorder)/2.0) {
		// Indicates R.H. side
		if(
				particle->y < node->topBorder &&
				particle->y >= (node->topBorder - node->botBorder)/2.0) {
			// Indicates top part
			return node->childNorthEast;
		} else {
			// Indicates bottom part
			return node->childSouthEast;
		}

	} else {
		// Indicates L.H. side
		if(
				particle->y < node->topBorder
				&& particle->y >= (node->topBorder - node->botBorder)/2.0) {
			// Indicates top part
			return node->childNorthWest;
		} else {
			// Indicates bottom part
			return node->childSouthWest;
		}
	}
}

void printQuadTree(node_t* node) {

	if (node->childNorthWest) {
		printQuadTree(node->childNorthWest);
		printQuadTree(node->childNorthWest);
		printQuadTree(node->childSouthWest);
		printQuadTree(node->childSouthWest);
	}
	if (node->particle) {
		printf("x = %lf\n", node->particle->x);
		printf("y = %lf\n", node->particle->y);
	} else {
		printf("%s\n", "Empty leaf");
	}
}

void freeQuadTree(node_t* node) {

	if (node->childNorthWest) {
		freeQuadTree(node->childNorthWest);
		freeQuadTree(node->childNorthWest);
		freeQuadTree(node->childSouthWest);
		freeQuadTree(node->childSouthWest);
	}
	free(node);
}
