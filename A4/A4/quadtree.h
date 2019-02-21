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
 * Inserts a particle into the quadtree, using recursion
 *
 * @param node		Recursive node of quadtree (call function using root)
 * @param x			Particle x-coordinate
 * @param y			Particle y-coordinate
 * @param m			Particle mass
 */
//node_t* insert(particle_t* particle, node_t* node);
void insert(node_t* __restrict node,
		double x,
		double y,
		double mass);

/**
 * Subdivides this node (makes it an interior node) by giving it four children.
 *
 * @param node Node of quadtree
 */
void subdivide(node_t* node);

/**
 * Finds the child of @param node where @param particle shall be inserted.
 *
 * @param node		Recursive node of quadtree
 * @param x			Particle x-coordinate	
 * @param y			Particle y-coordinate	
 *
 * @return          A child of @param node where @param particle belongs
 */
node_t* findCorrectChildForParticle(node_t* node,
		double x,
		double y);

/**
 * Prints the quadtree for easier debugging
 *
 * @param node Recursive node of quadtree (call function using root)
 */
void printQuadtree(node_t* node);

/**
 * Frees every sub-node of node in quadtree
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
