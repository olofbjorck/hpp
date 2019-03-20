printf "\n\n*** Starting tests: nthreads = 1 ***\n\n"

printf "N = 10: "
time ./galsim 10 ../input_data/ellipse_N_00010.gal 100 0.00001 0.12 0 1
printf "N = 20: "
time ./galsim 20 ../input_data/ellipse_N_00020.gal 100 0.00001 0.12 0 1
printf "N = 40: "
time ./galsim 40 ../input_data/ellipse_N_00040.gal 100 0.00001 0.12 0 1
printf "N = 80: "
time ./galsim 80 ../input_data/ellipse_N_00080.gal 100 0.00001 0.12 0 1
printf "N = 150: "
time ./galsim 150 ../input_data/ellipse_N_00150.gal 100 0.00001 0.12 0 1
printf "N = 300: "
time ./galsim 300 ../input_data/ellipse_N_00300.gal 100 0.00001 0.12 0 1
printf "N = 600: "
time ./galsim 600 ../input_data/ellipse_N_00600.gal 100 0.00001 0.12 0 1
printf "N = 1500: "
time ./galsim 1500 ../input_data/ellipse_N_01500.gal 100 0.00001 0.12 0 1
printf "N = 2000: "
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 100 0.00001 0.12 0 1
printf "N = 3000: "
time ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.12 0 1
printf "N = 10000: "
time ./galsim 10000 ../input_data/ellipse_N_10000.gal 100 0.00001 0.12 0 1

printf "\n\n*** nthreads = 2 ***\n\n"

printf "N = 10: "
time ./galsim 10 ../input_data/ellipse_N_00010.gal 100 0.00001 0.12 0 2
printf "N = 20: "
time ./galsim 20 ../input_data/ellipse_N_00020.gal 100 0.00001 0.12 0 2
printf "N = 40: "
time ./galsim 40 ../input_data/ellipse_N_00040.gal 100 0.00001 0.12 0 2
printf "N = 80: "
time ./galsim 80 ../input_data/ellipse_N_00080.gal 100 0.00001 0.12 0 2
printf "N = 150: "
time ./galsim 150 ../input_data/ellipse_N_00150.gal 100 0.00001 0.12 0 2
printf "N = 300: "
time ./galsim 300 ../input_data/ellipse_N_00300.gal 100 0.00001 0.12 0 2
printf "N = 600: "
time ./galsim 600 ../input_data/ellipse_N_00600.gal 100 0.00001 0.12 0 2
printf "N = 1500: "
time ./galsim 1500 ../input_data/ellipse_N_01500.gal 100 0.00001 0.12 0 2
printf "N = 2000: "
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 100 0.00001 0.12 0 2
printf "N = 3000: "
time ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.12 0 2
printf "N = 10000: "
time ./galsim 10000 ../input_data/ellipse_N_10000.gal 100 0.00001 0.12 0 2

printf "\n\n*** nthreads = 4 ***\n\n"

printf "N = 10: "
time ./galsim 10 ../input_data/ellipse_N_00010.gal 100 0.00001 0.12 0 4
printf "N = 20: "
time ./galsim 20 ../input_data/ellipse_N_00020.gal 100 0.00001 0.12 0 4
printf "N = 40: "
time ./galsim 40 ../input_data/ellipse_N_00040.gal 100 0.00001 0.12 0 4
printf "N = 80: "
time ./galsim 80 ../input_data/ellipse_N_00080.gal 100 0.00001 0.12 0 4
printf "N = 150: "
time ./galsim 150 ../input_data/ellipse_N_00150.gal 100 0.00001 0.12 0 4
printf "N = 300: "
time ./galsim 300 ../input_data/ellipse_N_00300.gal 100 0.00001 0.12 0 4
printf "N = 600: "
time ./galsim 600 ../input_data/ellipse_N_00600.gal 100 0.00001 0.12 0 4
printf "N = 1500: "
time ./galsim 1500 ../input_data/ellipse_N_01500.gal 100 0.00001 0.12 0 4
printf "N = 2000: "
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 100 0.00001 0.12 0 4
printf "N = 3000: "
time ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.12 0 4
printf "N = 10000: "
time ./galsim 10000 ../input_data/ellipse_N_10000.gal 100 0.00001 0.12 0 4

printf "\n\n*** nthreads = 8 ***\n\n"

printf "N = 10: "
time ./galsim 10 ../input_data/ellipse_N_00010.gal 100 0.00001 0.12 0 8
printf "N = 20: "
time ./galsim 20 ../input_data/ellipse_N_00020.gal 100 0.00001 0.12 0 8
printf "N = 40: "
time ./galsim 40 ../input_data/ellipse_N_00040.gal 100 0.00001 0.12 0 8
printf "N = 80: "
time ./galsim 80 ../input_data/ellipse_N_00080.gal 100 0.00001 0.12 0 8
printf "N = 150: "
time ./galsim 150 ../input_data/ellipse_N_00150.gal 100 0.00001 0.12 0 8
printf "N = 300: "
time ./galsim 300 ../input_data/ellipse_N_00300.gal 100 0.00001 0.12 0 8
printf "N = 600: "
time ./galsim 600 ../input_data/ellipse_N_00600.gal 100 0.00001 0.12 0 8
printf "N = 1500: "
time ./galsim 1500 ../input_data/ellipse_N_01500.gal 100 0.00001 0.12 0 8
printf "N = 2000: "
time ./galsim 2000 ../input_data/ellipse_N_02000.gal 100 0.00001 0.12 0 8
printf "N = 3000: "
time ./galsim 3000 ../input_data/ellipse_N_03000.gal 100 0.00001 0.12 0 8
printf "N = 10000: "
time ./galsim 10000 ../input_data/ellipse_N_10000.gal 100 0.00001 0.12 0 8
