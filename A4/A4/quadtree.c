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

int insert(particle_t* particle, node_t* node) {

	// NULL pointer
	if(node) {
		// Is the node already occupied?
		if(node->filled) {

		} else {

		}
	} else {
		return 1;
	}





	/*
	if (particle.x < node.x) {
		if (particle.y < node.y) {
			push(particle, node.childSouthWest);
		} else {
			push(particle, node.childNorthWest);
		}
	} else {
		if (particle.y < node.y) {
			push(particle, node.childSouthEast);
		} else {
			push(particle, node.childNorthEast);
		}
	}
*/
}

void subdivide(node_t* node) {

	node->childNorthWest = (node_t*) malloc(sizeof(node_t));
	node->childNorthEast = (node_t*) malloc(sizeof(node_t));
	node->childSouthWest = (node_t*) malloc(sizeof(node_t));
	node->childSouthEast = (node_t*) malloc(sizeof(node_t));
}
