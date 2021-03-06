/*    This program generates N random coordinates in space (3D), and N random
    velocity vectors. It then iterates M times to update the locations based
    on the velocity.
    Finally, it outputs the sum of all coordinates as a checksum of the computation.
    Coordinates start in the range [-1000:1000] per dimension.
    Velocities are chosen from the range [-1:1] per dimension.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>


// Create a list of 'size' floating point numbers in the range [bound]
float* generate_random_list(uint_fast32_t size, uint_fast32_t bound) {
	float *list = malloc(sizeof(float) * (float)size);
	for (uint_fast32_t i=0; i < size; i++) {
		list[i] = (float)rand()/(float)(RAND_MAX/bound);
	}
	return list;
}

// Update location by velocity, one time-step
void update_coords(uint_fast32_t i, float* x, float* y, float* z, float* vx, float* vy, float* vz) {
	x[i] = x[i] + vx[i];
	y[i] = y[i] + vy[i];
	z[i] = z[i] + vz[i];
}

// Sums an array of floats; needed in replacement of Python sum()
float sum(float* a, uint_fast32_t num_elements)
{
	float sum;
	for (uint_fast32_t i = 0; i < num_elements; i++) {
		sum += a[i];
	}
	return sum;
}

// Main from original file copied to best tested multiple times
float testmain(uint_fast32_t N, uint_fast32_t M) {

	uint_fast32_t object_size = N;
	uint_fast32_t iters = M;

	srand(object_size);

	float* x = generate_random_list(object_size, 1000);
	float* y = generate_random_list(object_size, 1000);
	float* z = generate_random_list(object_size, 1000);
	float* vx = generate_random_list(object_size, 1);
	float* vy = generate_random_list(object_size, 1);
	float* vz = generate_random_list(object_size, 1);

	struct timespec requestStart, requestEnd;
	clock_gettime(CLOCK_MONOTONIC, &requestStart);
	for (uint_fast32_t i=0; i < iters; i++) {
		for (uint_fast32_t j=0; j < object_size; j++) {
			update_coords(j,x,y,z,vx,vy,vz);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &requestEnd);


	float timeTaken = (requestEnd.tv_sec + (requestEnd.tv_nsec / 1000000000.)) - (requestStart.tv_sec + (requestStart.tv_nsec / 1000000000.));
	free(x);
    	free(y);	
   	free(z);
    	free(vx);
 	free(vy);
 	free(vz);
	return(((1000000. * timeTaken) / (object_size * iters)));

}
//Compare two floats for qsort()
int compare_floats (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}


// Main:
int main()
{
	uint_fast32_t object_size = (uint_fast32_t) pow((double) 2,8);
	uint_fast32_t iters = (uint_fast32_t) pow((double) 2,16);

	float* output = calloc((float)object_size, sizeof(float));
	for (uint_fast32_t i = 0; i < 17; i++) {
		for (uint_fast32_t j = 0; j < 9; j++) {
			output[j] = testmain(object_size, iters);
		}
		qsort(output, 9, sizeof(float), compare_floats);
		printf("%f \n", output[4]);
		object_size *= 2;
		iters = (uint_fast32_t)(iters / 2);    

	}
	return 0;
}






