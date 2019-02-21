/**
 *	quadtree.h
 * 	Functions for creating and modifying a quadtree data structure
 *
 */

#pragma once
#include "modules.h"

/**
 * Builds a quadtree of size N from a root node, and fills it with particles
 *
 * @param particles	Array of particles
 * @param N			Total number of particles
 * @param root		Root node of quadtree
 */
void buildQuadtree(particles_t* __restrict particles,
		const int N,
		node_t* __restrict root);

/**
 * Frees every sub-node of node in quadtree
 *
 * @param node Node of quadtree
 */
void freeQuadtree(node_t* node);
