// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define particle
// Vet inte om det är snabbare för alla beräkningar att ha separate arrayer för
// allt, än att ha en enda array med massa particle structs. Tänkte att vi
// kan börja med att implementera med en struct och få allting att fungera,
// sedan jobba på optimeringen.
typedef struct particle {
	double x, vx; // x-position x and x-velocity vx
	double y, vy; // y-position y and y-velocity vy
	double mass; // particle mass
  double brightness; // particle brightness
  double force;
} particle_t;


// Read input data
void readData(particle_t*);
// Simulates the galaxies
void simulate(particle_t*);
// Saves final positions and velocities to result.gal
void writeOutput(particle_t*);
// Calculates plummer spheres force
void calculateForces(particle_t*);

// Main function
int main(int argc, char const *argv[]) {

  // Check proper number of input arguments
  if (argc != 6) {
    printf("%s\n", "Input error: Expected 5 input arguments");
    return 1;
  }

  // Read input from command line
  const int N = atoi(argv[1]); // Nr of stars to simulate
  char* filename[strlen(argv[2])]; strcpy(*filename, argv[2]);
  const int nsteps = atoi(argv[3]); // Nr of filesteps
  const double delta_t = atof(argv[4]); // Timestep
  const int graphics = atoi(argv[5]); // Graphics on/off as 1/0

  // Define some constants
  const double G = 100/N;
  const double eps0 = 0.001;
  // delta_t should be 10^-5 = 0.00001 passed as input

  // Create array with all particles
  particle_t particles[N]; // Use malloc? N can be large?
  readData(particles);
  // calculateForces(particles);
  simulate(particles); // calculateForces is part of the simulatio
  writeOutput(particles);

  return 0;
}
