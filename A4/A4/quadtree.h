/**
 *	quadtree.h
 * 	Functions for creating and modifying a quadtree data structure
 *
 */

#pragma once
#include "modules.h"
#include "galsim.h"

/**
 * Builds a quadtree of size N from a root node, and fills it with particles
 *
 * @param particles	Array of particles
 * @param N			Total number of particles
 * @param root		Root node of quadtree
 */
void buildQuadtree(particle_t* particles, int N, node_t* root);

/**
 * Inserts a particle into the quadtree, using recursion
 *
 * @param particle	Particle to insert
 * @param node		Node of quatree
 * @return 			Returns 0 if insert was successful, 1 otherwise
 */
//node_t* insert(particle_t* particle, node_t* node);
void insert(particle_t* particle, node_t* node);

/**
 * Subdivides this node (makes it an interior node) by giving it children.
 *
 * @param node Node of quadtree
 */
void subdivide(node_t* node);

/**
 * Finds the child of @param node where @param particle shall be inserted.
 *
 * @param  particle Particle to insert
 * @param  node     Node of quadtree
 *
 * @return          A child of @param node where @param particle belongs
 */
node_t* findCorrectChildForParticle(particle_t* particle, node_t* node);

/**
 * Prints the quadtree for easier debugging
 *
 * @param node Node of quadtree
 */
void printQuadtree(node_t* node);

/**
 * Frees all nodes except the root of the quadtree.
 *
 * @param node Node of quadtree
 */
void freeQuadtree(node_t* node);

/**
 * Initializes a node.
 *
 * @param node   Node of quadtree
 * @param yTop   Top y-value of node
 * @param yBot   Bottom y-value of node
 * @param xLeft  Left x-value of node
 * @param xRight Right x-value of node
 */
void initialize(
		node_t* node,
		double xCenter, double yCenter, double sideHalf);

void printQuadtree(node_t* node);
void freeQuadtree(node_t* node);
