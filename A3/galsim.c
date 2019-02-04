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
int readData(particle_t* particles, const char* filename, int N);
// Simulates the galaxies
void simulate(particle_t*);
// Saves final positions and velocities to result.gal
void writeOutput(particle_t* particles, int N);
// Calculates plummer spheres force
void calculateForces(particle_t*);

// Print for debugging. Can choose to print first n particles.
void printParticles(particle_t*, int);

// Main function
int main(int argc, char const *argv[]) {

  // Check proper number of input arguments
  if (argc != 6) {
    printf("%s\n", "Input error: Expected 5 input arguments");
    return 1;
  }

  // Read input from command line
  const int N = atoi(argv[1]); // Nr of stars to simulate
  const char* filename = argv[2];
  const int nsteps = atoi(argv[3]); // Nr of filesteps
  const double delta_t = atof(argv[4]); // Timestep
  const int graphics = atoi(argv[5]); // Graphics on/off as 1/0

  // Define some constants
  const double G = 100/N;
  const double eps0 = 0.001;
  // delta_t should be 10^-5 = 0.00001 passed as input

  // Create array with all particles
  particle_t particles[N]; // Use malloc? N can be large?
  if (!readData(particles, filename, N)) return 0;
  printParticles(particles, N);
  // calculateForces(particles);
  simulate(particles); // calculateForces is part of the simulation
  writeOutput(particles, N);

  return 0;
}

// Read data from file.
int readData(particle_t* particles, const char* filename, int N) {

  // Open file
  FILE* fp = fopen(filename, "r");

// TODO: Check if at end of file? Currently reads a new particle with vals = 0

  if (fp) {
    // Read file
    int i;
    for (i = 0; i < N; i++) {
      fread(&particles[i].x, sizeof(double), 1, fp);
      fread(&particles[i].y, sizeof(double), 1, fp);
      fread(&particles[i].mass, sizeof(double), 1, fp);
      fread(&particles[i].vx, sizeof(double), 1, fp);
      fread(&particles[i].vy, sizeof(double), 1, fp);
      fread(&particles[i].brightness, sizeof(double), 1, fp);
    }

    // Close file
    fclose(fp);

    return 1;
  } else {
    printf("%s\n", "Error reading file: doesn't exist.");
    return 0;
  }
}

void simulate(particle_t* particles) {

}

// Write current state of all particles to file
void writeOutput(particle_t* particles, int N) {

  // Create file to write
  FILE* fp = fopen("result.gal", "w");

  // Write to file
  int i;
  for (i = 0; i < N; i++) {
    fwrite(&particles[i].x, sizeof(double), 1, fp);
    fwrite(&particles[i].y, sizeof(double), 1, fp);
    fwrite(&particles[i].mass, sizeof(double), 1, fp);
    fwrite(&particles[i].vx, sizeof(double), 1, fp);
    fwrite(&particles[i].vy, sizeof(double), 1, fp);
    fwrite(&particles[i].brightness, sizeof(double), 1, fp);
  }

  // Close file
  fclose(fp);
}

// Print particles (for debugging)
void printParticles(particle_t* particles, int N) {

  int i;
  for (i = 0; i < N; i++) {
    printf("Particle %d:\n", i);
    printf("\t(x, y) = (%.2lf, %.2lf)\n", particles[i].x, particles[i].y);
    printf("\t(vx, vy) = (%.2lf, %.2lf)\n", particles[i].vx, particles[i].vy);
    printf("\tmass = %lf\n", particles[i].mass);
    printf("\tbrightness = %lf\n", particles[i].brightness);
  }
}
