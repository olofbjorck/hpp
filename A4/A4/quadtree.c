#include "quadtree.h"

void printQuadtree(node_t* node);
void freeQuadtree(node_t* node);

void buildQuadtree(particle_t* particles, int N, node_t* root) {

	// Initialize Root node
	initialize(root, 1.0, 0.0, 0.0, 1.0);

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
		printf("ERROR: NULL node");
		return;
	}
	*/

	printf("%s\n", "Inside insert function");

	// Node already contains a particle - is an occupied leaf?
	if (node->particle) {

		printf("%s\n", "In node->particle:");

		// Make non-leaf
		printf("%s\n", "... making non-leaf");
		particle_t* tempParticle;
		//printf("%p\n", node->particle);
		tempParticle = node->particle;
		node->particle = NULL;


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
		return;
	// else: We've found an empty leaf or an interior node
	} else {
		printf("%s\n", "In else (node->particle == NULL)");
		// Is this an interior node?
		if (node->childNorthWest) {
			printf("%s\n", "In node->childNorthWest");
			// Recurse on the correct child
			printf("%s\n", "Recursing on new particle");
			insert(particle, findCorrectChildForParticle(particle, node));
			return;
		// else: This is an empty leaf
		} else {
			printf("%s\n", "In else (node->childNorthWest == NULL)");
			// Add particle to node, make it an occupied leaf
			node->particle = particle;
			printf("%s\n", "Inserted particle in empty leaf!\n");
			return;
		}
	}
}

void subdivide(node_t* node) {
	printf("%s\n", "... allocating:\tnorth west");
	node->childNorthWest = (node_t*) malloc(sizeof(node_t));
	initialize(node->childNorthWest, node->yTop, (node->yBot)/2, node->xLeft, (node->xRight)/2);
	printf("%s\n", "... \t\tnorth east");
	node->childNorthEast = (node_t*) malloc(sizeof(node_t));
	initialize(node->childNorthEast, node->yTop, (node->yBot)/2, (node->xLeft)/2, node->xRight);
	printf("%s\n", "... \t\tsouth west");
	node->childSouthWest = (node_t*) malloc(sizeof(node_t));
	initialize(node->childSouthWest, (node->yTop)/2, node->yBot, node->xLeft, (node->xRight)/2);
	printf("%s\n", "... \t\tsouth east");
	node->childSouthEast = (node_t*) malloc(sizeof(node_t));
	initialize(node->childSouthEast, (node->yTop)/2, node->yBot, (node->xLeft)/2, node->xRight);
}

node_t* findCorrectChildForParticle(particle_t* particle, node_t* node) {

	if(particle->x) {

	}

	printf("%s\n", "Inside findCorrectChildForParticle");

	if(
			particle->x < node->xRight &&
			particle->x >= (node->xLeft - node->xRight)/2.0) {
		// Indicates R.H. side
		printf("%s\n", "... R.H. side");
		if(
				particle->y < node->yTop &&
				particle->y >= (node->yTop - node->yBot)/2.0) {
			// Indicates top part
			printf("%s\n", "... \t top part");
			return node->childNorthEast;
		} else {
			// Indicates bottom part
			printf("%s\n", "... \t bottom part");
			return node->childSouthEast;
		}

	} else {
		// Indicates L.H. side
		printf("%s\n", "... L.H. side");
		if(
				particle->y < node->yTop
				&& particle->y >= (node->yTop- node->yBot)/2.0) {
			// Indicates top part
			printf("%s\n", "... \t top part");
			return node->childNorthWest;
		} else {
			// Indicates bottom part
			printf("%s\n", "... \t bottom part");
			return node->childSouthWest;
		}
	}
}

void printQuadtree(node_t* node) {

	if (node->childNorthWest) {
		printQuadtree(node->childNorthWest);
		printQuadtree(node->childNorthEast);
		printQuadtree(node->childSouthWest);
		printQuadtree(node->childSouthEast);
	}
	if (node->particle) {
		printf("x = %lf\n", node->particle->x);
		printf("y = %lf\n", node->particle->y);
	} else {
		printf("%s\n", "Empty leaf");
	}
}

void freeQuadtree(node_t* node) {
printf("%s\n", "Freeing Quadtree");
	if (node->childNorthWest) {
		printf("%s\n", "... northwest");
		freeQuadtree(node->childNorthWest);
		printf("%s\n", "... northeast");
		freeQuadtree(node->childNorthEast);
		printf("%s\n", "... southwest");
		freeQuadtree(node->childSouthWest);
		printf("%s\n", "... southeast");
		freeQuadtree(node->childSouthEast);
	}
	printf("%s\n", "... free(node)");
	free(node);
}

void initialize(
		node_t* node,
		double yTop, double yBot, double xLeft, double xRight) {

	// Initialize Root node
	node->particle = NULL;
	node->centerOfMass_x = 0.0;
	node->centerOfMass_y = 0.0;
	node->mass = 0.0;

	node->yTop = yTop;
	node->yBot = yBot;
	node->xLeft = xLeft;
	node->xRight = xRight;

	node->childNorthWest = NULL;
	node->childNorthEast = NULL;
	node->childSouthWest = NULL;
	node->childSouthEast = NULL;
}
