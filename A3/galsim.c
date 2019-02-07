// RUN BY:
// ./galsim 2 circles_N_2.gal 5 0.00001 1
// ./galsim 10 ellipse_N_00010.gal 5 0.00001 1
// time ./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 1

/**
 * Simulates galaxy movement in outer space.
 *
 * Original authors: Olof Björck, Gunnlaugur Geirsson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Particle struct declaration.
 *
 */
typedef struct particle {
	double x, v_x; // x-position x and x-velocity vx
	double y, v_y; // y-position y and y-velocity vy
  double force_x, force_y; // Force exerted on particle
	double mass;  // particle mass.
} particle_t;

/**
 * Reads galaxy data from input file "filename" into particle_t* array
 * particles. Galaxy brightness is stored in separate array double* brightness
 * for speed as brightness isn't use in calculations. Returns 1 if data was
 * read successfully, 0 otherwise.
 *
 * @param  particles  Information about every particle.
 * @param  brightness Array with brightness information about every particle.
 * @param  filename   The input data filename (ending with ".gal").
 * @param  N          The total number of particles.
 * @return            Returns 1 if data was read successfully, else 0.
 */
static int readData(particle_t* __restrict particles,
              double* __restrict brightness,
              const char* filename, const int N);
/**
 * Simulates the movement of all the particles in particle_t* particles array.
 * The simulation calculates the forces acting on each particle and then
 * updates every particle's state. The simulation is carried out nsteps times,
 * with timestep delta_t.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smoothe calculations.
 * @param nsteps    Number of timesteps to simulate.
 * @param delta_t   Timestep [seconds].
 */
static void simulate(particle_t* __restrict particles, const int N,
                      const double G, const double eps0,
                      const int nsteps, const double delta_t);
/**
 * Calculates and stores the aggregate force exerted on every particle by all
 * other particles in array particle_t* particles.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param G         The Newton gravitational constant G.
 * @param eps0      Plummer spheres constant to smoothe calculations.
 */
inline static void calculateForces(particle_t* __restrict particles, const int N,
                            const double G, const double eps0);
/**
 * Updates the state of the particles with timestep delta_t. Does not compute
 * any forces; current forces particles.force_x and particles.force_y need
 * to be computed and set before using this function.
 *
 * @param particles Information about every particle.
 * @param N         The total number of particles.
 * @param delta_t   Timestep [seconds].
 */
inline static void updateParticles(particle_t* __restrict particles,
                                    const int N, const double delta_t);
/**
 * Shows the state of the particles graphically.
 *
 * @param particles Information about every particle.
 */
static void showGraphics(particle_t* __restrict particles);
/**
 * Writes current state of all particles to output file "result.gal".
 *
 * @param particles  Information about every particle.
 * @param brightness Array with brightness information about every particle.
 * @param N          The total number of particles.
 */
static void writeOutput(particle_t* __restrict particles,
                  double* __restrict brightness,
                  const int N);

/*
static void printParticles(particle_t* __restrict particles,
                    double* __restrict brightness,
                    const int N);
*/

/**
 * Main function
 *
 */
int main(int argc, char const *argv[]) {

  // Check proper number of input arguments
  if (argc != 6) {
    printf("%s\n", "Input error: Expected 5 input arguments");
    return 1;
  }

  // Read input from command line
  const int N = atoi(argv[1]); // Nr of stars to simulate
  const char* filename = argv[2]; // Filename
  const int nsteps = atoi(argv[3]); // Nr of filesteps
  const double delta_t = atof(argv[4]); // Timestep
  const int graphics = atoi(argv[5]); // Graphics on/off as 1/0

  // Define some constants
  const double G = 100.0/N; // Gravitational constant
  const double eps0 = 0.001; // Plummer sphere constant
  // delta_t should be 10^-5 = 0.00001 passed as input

  // Create array with all particles
  particle_t particles[N]; // Use malloc? N can be large?
  double brightness[N];

  // Read data
  if (!readData(particles, brightness, filename, N)) return 1;
  //printParticles(particles, N);

  // Simulate movement
  simulate(particles, N, G, eps0, nsteps, delta_t);
  //printParticles(particles, N);

  // Show graphics
  if (graphics) showGraphics(particles);

  // Write new state of particles to file
  writeOutput(particles, brightness, N);

  return 0;
}

// Simulate the movement of the particles
void simulate(particle_t* __restrict particles, const int N,
              const double G, const double eps0,
              const int nsteps, const double delta_t) {

  int i;
  for (i = 0; i < nsteps; i++) {
    calculateForces(particles, N, G, eps0);
    updateParticles(particles, N, delta_t);
  }
}

// Calculates force exerted on every particle
inline void calculateForces(particle_t* __restrict particles, const int N,
                            const double G, const double eps0) {

  int i, j; // Loop variables
  double r = 0.0, r_x = 0.0, r_y = 0.0; // Vector
  double denom = 0.0; // Denominator
  for (i = 0; i < N; i++) {
    // Initialize forces
    particles[i].force_x = 0.0, particles[i].force_y = 0.0;
    // Loop
    for (j = 0; j < N; j++) { // case i==j is no problem as r will be 0.
                              // Checked timings, this is faster than two
                              // for-loops, skipping i==j
      // Calculate r-vector
      r_x = particles[i].x - particles[j].x;
      r_y = particles[i].y - particles[j].y;
      r = sqrt(r_x*r_x + r_y*r_y);
      // Calculate denominator
      denom = r + eps0;
      denom = 1/(denom*denom*denom); // 1 msec faster than using /demon below
      // Calculate force
      particles[i].force_x += particles[j].mass*r_x*denom;
      particles[i].force_y += particles[j].mass*r_y*denom;
    }
    particles[i].force_x *= -G*particles[i].mass;
    particles[i].force_y *= -G*particles[i].mass;
  }
}

// Update the particles
inline void updateParticles(particle_t* __restrict particles, const int N,
                            const double delta_t) {

  int i; // Loop variable
  double a_x = 0.0, a_y = 0.0; // Acceleration
  // Loop over all particles
  for (i = 0; i < N; i++) {
    // Calculate acceleration
    a_x = particles[i].force_x/particles[i].mass;
    a_y = particles[i].force_y/particles[i].mass;
    // Update velocity
    particles[i].v_x += delta_t*a_x;
    particles[i].v_y += delta_t*a_y;
    // Update position
    particles[i].x += delta_t*particles[i].v_x;
    particles[i].y += delta_t*particles[i].v_y;
  }
}

// Read data from file.
int readData(particle_t* __restrict particles,
              double* __restrict brightness,
              const char* filename, const int N) {

  // Open file
  FILE* fp = fopen(filename, "r");

  // Check file open went ok
  if (!fp) {
    printf("%s\n", "Error: couldn't open input file. Is it in directory?");
    return 0;
  }

  // Check file size is as expected
  fseek(fp, 0L, SEEK_END); // Seek end
  size_t fileSize = ftell(fp); // Get file size
  fseek(fp, 0L, SEEK_SET); // Reset seek to start of file
  if (fileSize != 6*N*sizeof(double)) { // File size not as expected?
    printf("%s\n", "Error: Input file size is not as expected. Is N correct?");
    return 0;
  }

  // Read file
  int i;
  for (i = 0; i < N; i++) {
    fread(&particles[i].x, sizeof(double), 1, fp);
    fread(&particles[i].y, sizeof(double), 1, fp);
    fread(&particles[i].mass, sizeof(double), 1, fp);
    fread(&particles[i].v_x, sizeof(double), 1, fp);
    fread(&particles[i].v_y, sizeof(double), 1, fp);
    fread(&brightness[i], sizeof(double), 1, fp);
  }

  // Close file
  if (fclose(fp)) {
    printf("%s\n", "Error: couldn't close input file.");
    return 0;
  }

  return 1;
}

// Show particles graphically
void showGraphics(particle_t* __restrict particles) {
  // Not implemented
}

// Write current state of all particles to file
void writeOutput(particle_t* __restrict particles,
                  double* __restrict brightness,
                  const int N) {

  // Create file to write
  FILE* fp = fopen("result.gal", "w");

  // Write to file
  int i;
  for (i = 0; i < N; i++) {
    fwrite(&particles[i].x, sizeof(double), 1, fp);
    fwrite(&particles[i].y, sizeof(double), 1, fp);
    fwrite(&particles[i].mass, sizeof(double), 1, fp);
    fwrite(&particles[i].v_x, sizeof(double), 1, fp);
    fwrite(&particles[i].v_y, sizeof(double), 1, fp);
    fwrite(&brightness[i], sizeof(double), 1, fp);
  }

  // Close file
  fclose(fp);
}

/*
// Print particles (for debugging)
void printParticles(particle_t* __restrict particles,
                    double* __restrict brightness, const int N) {

  int i;
  for (i = 0; i < N; i++) {
    printf("Particle %d:\n", i+1);
    printf("\t(x, y) = (%.2lf, %.2lf)\n", particles[i].x, particles[i].y);
    printf("\t(vx, vy) = (%.2lf, %.2lf)\n", particles[i].v_x, particles[i].v_y);
    printf("\t(force_x, force_y) = (%.2lf, %.2lf)\n",
            particles[i].force_x, particles[i].force_y);
    printf("\tmass = %lf\n", particles[i].mass);
    printf("\tbrightness = %lf\n", brightness[i]);
  }
}
*/
