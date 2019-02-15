#include "quadtree.h"

void buildQuadtree(particle_t* particles, int N, node_t* root) {

	unsigned int i;
	for (i = 0; i < N; i++) {
		//push(particles[i], root);
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

node_t* insert(particle_t* particle, node_t* node) {

	// NULL node - make a leaf and add particle
	if(!node) {
		return 1;
	}

	// Node already contains a particle - is a leaf 
	if(node->particle) {
		particle_t* tempParticle = node->particle;
		node->particle = NULL;

		if(particle->x < node->rightBorder && particle->x >= (node->leftBorder - node->rightBorder)/2.0) {
			// Indicates R.H. side
			if(particle->y < node->topBorder && particle->y >= (node->topBorder - node->botBorder)/2.0) {
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
			if(particle->y < node->topBorder && particle->y >= (node->topBorder - node->botBorder)/2.0) {
				// Indicates top part
				
			} else {
				// Indicates bottom part

			}


		}

	// else: Make node a leaf
	// Add particle to node
	} else {
		node->particle = particle;
	}
}
