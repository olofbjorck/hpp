#include "quadtree.h"

void buildQuadtree(particle_t* particles, int N, node_t* root) {

	unsigned int i;
	for (i = 0; i < N; i++) {
		insert(&particles[i], root);
	}
}

/*
void push(particle_t particle, node_t node) {

	if (node.filled) { // Node already occupied?
		if (node.hasChildren) { // Node has children?
			insert(particle, node);
		} else { // Node has no children, subdivide and then insert
			subdivide(node);
			push(node.particle, node); // insert particle that currently occupies node
			insert(particle, node);
		}
	} else { // Node isn't occupied
		node.filled = 1;
		node.mass = particle.mass;
	}
}
*/

// varför returnera node_t*?
//node_t* insert(particle_t* particle, node_t* node) {
void insert(particle_t* particle, node_t* node) {

	// NULL node - make a leaf and add particle
	/*
	if(!node) {
		return 1;
	}*/

	// Node already contains a particle - is an occupied leaf?
	if (node->particle) {

		// Make non-leaf
		particle_t* tempParticle = node->particle;
		node->particle = NULL;

		// Subdivide the node (make it an interior node)
		subdivide(node);

		// Recursively insert on the correct child of this node
		// Insert this node's old particle
		insert(tempParticle, findCorrectChildForParticle(tempParticle, node));
		// Insert the new particle
		insert(particle, findCorrectChildForParticle(particle, node));

	// else: We've found an empty leaf or an interior node
	} else {

		// Is this an interior node?
		if (node->childNorthWest) {
			// Recurse on the correct child
			insert(particle, findCorrectChildForParticle(particle, node));

		// else: This is an empty leaf
		} else {
			// Add particle to node, make it an occupied leaf
			node->particle = particle;
		}
	}
}

void subdivide(node_t* node) {
	node->childNorthWest = (node_t*) malloc(sizeof(node_t*));
	node->childNorthEast = (node_t*) malloc(sizeof(node_t*));
	node->childSouthWest = (node_t*) malloc(sizeof(node_t*));
	node->childSouthWest = (node_t*) malloc(sizeof(node_t*));
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
			if(node->childNorthWest) {
				// ???
			} else {
			}
		} else {
			// Indicates bottom part

		}

	} else {
		// Indicates L.H. side
		if(
				particle->y < node->topBorder
				&& particle->y >= (node->topBorder - node->botBorder)/2.0) {
			// Indicates top part

		} else {
			// Indicates bottom part

		}
	}
}
