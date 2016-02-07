/*    This program generates N random coordinates in space (3D), and N random
    velocity vectors. It then iterates M times to update the locations based
    on the velocity.
    Finally, it outputs the sum of all coordinates as a checksum of the computation.
    Coordinates start in the range [-1000:1000] per dimension.
    Velocities are chosen from the range [-1:1] per dimension.
 */
#define _POSIX_C_SOURCE 199309L 

/* the above line was added as http://stackoverflow.com/questions/26769129/trying-to-use-clock-gettime-but-getting-plenty-of-undeclared-errors-from-ti suggested it would fix our library functions importing improperly...
*/ 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>


// Create a list of 'size' floating point numbers in the range [bound]
int32_t* generate_random_list(uint32_t size, uint16_t bound) {
	int32_t *list = malloc(sizeof(int32_t) * (int32_t)bound);
	for (uint32_t i=0; i < size; i++) {
		list[i] = rand();
	}
	return list;
}

// Update location by velocity, one time-step
void update_coords(uint32_t i, int32_t* x, int32_t* y, int32_t* z, int32_t* vx, int32_t* vy, int32_t* vz) {
	x[i] = x[i] + vx[i];
	y[i] = y[i] + vy[i];
	z[i] = z[i] + vz[i];
}

// Sums an array of floats; needed in replacement of Python sum()
int32_t sum(int32_t* a, uint32_t num_elements)
{
	int32_t sum = 0;
	for (uint32_t i = 0; i < num_elements; i++) {
		sum = sum + a[i];
	}
	return sum;
}

// Main:
int main(int argc, char* argv[]) {

	if (argc != 3) {
		printf("Required arguments: vector_length(N) and iterations_num(M)\n");
		exit(-1);
	}

	uint32_t object_size = *argv[1];
	uint32_t iters = *argv[2];

	srand(object_size);

	int32_t* x = generate_random_list(object_size, 1000);
	int32_t* y = generate_random_list(object_size, 1000);
	int32_t* z = generate_random_list(object_size, 1000);
	int32_t* vx = generate_random_list(object_size, 1);
	int32_t* vy = generate_random_list(object_size, 1);
	int32_t* vz = generate_random_list(object_size, 1);

	struct timespec requestStart, requestEnd;
	clock_gettime(CLOCK_MONOTONIC, &requestStart);
	for (uint32_t i=0; i < iters; i++) {
		for (uint32_t j=0; j < object_size; j++) {
			update_coords(j,x,y,z,vx,vy,vz);
		}
	}
	clock_gettime(CLOCK_MONOTONIC, &requestEnd);

	float chksum = sum(x,object_size) + sum(y,object_size) + sum(z,object_size);
	printf("Mean time per coordinate: %f us\n", ((requestEnd.tv_nsec - requestStart.tv_nsec) / (1000. * (object_size * iters))));
	printf("Final checksum is: %f\n", chksum);

}
