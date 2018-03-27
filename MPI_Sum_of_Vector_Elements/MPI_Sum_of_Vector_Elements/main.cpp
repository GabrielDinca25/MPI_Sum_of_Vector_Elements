#include <mpi.h>
#include <stdio.h>

#define NUMBER_OF_ELEMENTS 16
#define MASTER 0

int main(int argc, char** argv) {

	int v[NUMBER_OF_ELEMENTS], numprocs, procid, len, partner, message;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
	{
		v[i] = i;
	}

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	MPI_Get_processor_name(hostname, &len);

	int step = NUMBER_OF_ELEMENTS / numprocs;

	int partial_sum = 0;
	printf("Processor: %d, Elements are: ", procid);
	for(int i = procid * step ; i < procid * step + step; i++)
	{	
		partial_sum += v[i];
		printf(" %d ", v[i]);
	}
	printf("-- Sum of the elements is: %d \n", partial_sum);

	int total_sum;
	MPI_Reduce(&partial_sum, &total_sum, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

	if (procid == MASTER)
	{
		printf("MASTER: The total sum of the arrays's elements is: %d \n", total_sum);
	}

	MPI_Finalize();

}