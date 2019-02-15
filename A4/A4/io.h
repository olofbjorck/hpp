/**
 *	io.h
 * 	Contains functions for reading and writing .gal files
 */

#pragma once
#include "galsim.h"
#include <string.h>

/**
 * Reads galaxy data from input file "filename" into particle_t* array
 * particles. Galaxy brightness is stored in separate array double* brightness
 * for speed as brightness isn't used in calculations. Returns 0 if data was
 * read successfully, 1 otherwise.
 *
 * @param  particles  Information about every particle.
 * @param  brightness Array with brightness information about every particle.
 * @param  filename   The input data filename (ending with ".gal").
 * @param  N          The total number of particles.
 * @return            Returns 1 if data was read successfully, else 0.
 */
 int readData(
		particle_t* __restrict particles,
		double* __restrict brightness,
		const char* filename, const int N);

/**
 * Writes current state of all particles to output file "result.gal".
 *
 * @param particles  Information about every particle.
 * @param brightness Array with brightness information about every particle.
 * @param N          The total number of particles.
 */
 int writeOutput(
		particle_t* __restrict particles,
		double* __restrict brightness,
		const int N);
