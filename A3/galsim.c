// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main function
int main(int argc, char const *argv[]) {

  // Check proper number of input arguments
  if (argc != 6) {
    printf("%s\n", "Input error: Expected 5 input arguments");
    return 1;
  }

  // Read input from command line
  const int N = atoi(argv[1]);
  char* filename[strlen(argv[2])]; strcpy(*filename, argv[2]);
  const int nsteps = atoi(argv[3]);
  const double delta_t = atof(argv[4]);
  const int graphics = atoi(argv[5]);

  // Define some constants
  const double G = 100/N;
  const double eps0 = 0.001;
  // delta_t should be 10^-5 = 0.00001 passed as input

  return 0;
}
