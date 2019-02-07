// RUN BY:
// ./galsim 2 circles_N_2.gal 5 0.00001 1
// ./galsim 10 ellipse_N_00010.gal 5 0.00001 1
// time ./galsim 10000 input_data/ellipse_N_10000.gal 100 0.00001 1

// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Define particle
// Vet inte om det är snabbare för alla beräkningar att ha separate arrayer för
// allt, än att ha en enda array med massa particle structs. Tänkte att vi
// kan börja med att implementera med en struct och få allting att fungera,
// sedan jobba på optimeringen.
// Array of structs AoS eller Struct of Arrays SoA?
// Denna order är important! Vi använder x, v_x och force, mass ofta
// efter varandra.
// brightness i separat array eftersom den inte används
typedef struct particle {
	double x, v_x; // x-position x and x-velocity vx
	double y, v_y; // y-position y and y-velocity vy
  double force_x, force_y; // Force exerted on particle
	double mass;  // particle mass.
  //double brightness; // particle brightness
} particle_t;


// Read input data
int readData(particle_t* __restrict particles,
              double* __restrict brightness,
              const char* filename, const int N);
// Simulate the movement of the particles
void simulate(particle_t* __restrict particles, const int N,
              const double G, const double eps0,
              const int nsteps, const double delta_t);
// Calculates plummer spheres force
inline void calculateForces(particle_t* __restrict particles, const int N,
                            const double G, const double eps0);
// Update the particle states with one time step
inline void updateParticles(particle_t* __restrict particles, const int N,
                            const double delta_t);
// Displays graphically the state of the particles
void showGraphics(particle_t* __restrict particles);
// Saves final positions and velocities to result.gal
void writeOutput(particle_t* __restrict particles,
                  double* __restrict brightness,
                  const int N);
// Print for debugging. Can choose to print first n particles.
void printParticles(particle_t* __restrict particles,
                    double* __restrict brightness,
                    const int N);

// Main function
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
  if (!readData(particles, brightness, filename, N)) return 0;
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

// TODO: Check if at end of file? Currently reads a new particle with vals = 0

  if (fp) {
    // Read file
    int i;
    for (i = 0; i < N; i++) {
      fread(&particles[i].x, sizeof(double), 1, fp);
      fread(&particles[i].y, sizeof(double), 1, fp);
      fread(&particles[i].mass, sizeof(double), 1, fp);
      fread(&particles[i].v_x, sizeof(double), 1, fp);
      fread(&particles[i].v_y, sizeof(double), 1, fp);
      //fread(&particles[i].brightness, sizeof(double), 1, fp);
      fread(&brightness[i], sizeof(double), 1, fp);
    }

    // Close file
    fclose(fp);

    return 1;
  } else {
    printf("%s\n", "Error reading file: doesn't exist.");
    return 0;
  }
}

// Show particles graphically
void showGraphics(particle_t* __restrict particles) {

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
    //fwrite(&particles[i].brightness, sizeof(double), 1, fp);
    fwrite(&brightness[i], sizeof(double), 1, fp);
  }

  // Close file
  fclose(fp);
}

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
