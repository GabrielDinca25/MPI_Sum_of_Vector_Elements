#include <mpi.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

#define NUMBER_OF_ELEMENTS 16
#define MASTER 0

int v[NUMBER_OF_ELEMENTS];
int main(int argc, char** argv) {

	int numprocs, procid, len, partner, message;
	int subvector[NUMBER_OF_ELEMENTS];
	
	char hostname[MPI_MAX_PROCESSOR_NAME];
	for (int i = 0; i < NUMBER_OF_ELEMENTS; i++)
	{
		v[i] = i;
	}

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	MPI_Get_processor_name(hostname, &len);
	MPI_Status status;

	int const step = NUMBER_OF_ELEMENTS / numprocs;
	//printf("step: %d, number of elements: %d, numprocs: %d \n", step, NUMBER_OF_ELEMENTS, numprocs);

	int partial_sum = 0;
	int total_sum = 0;
	
	if (procid == MASTER)
	{
		for (int i = 0; i < step; i++)
		{
			total_sum += v[i];
		}
		for (int i = 1; i < numprocs; i++)
		{
			int lo = i * step;
			int hi = lo + step;
			int j = 0;
			for (int i = lo; i < hi; i++)
			{
				subvector[j] = v[i];
				j++;
			}
			MPI_Send(&subvector, step, MPI_INT, i, i, MPI_COMM_WORLD);
		}

		for (int i = 1; i < numprocs; i++)
		{
			MPI_Recv(&partial_sum, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
			total_sum += partial_sum;
		}

		printf("MASTER: The total sum of the arrays's elements is: %d \n", total_sum);
	}
	else
	{
		MPI_Recv(&subvector, step, MPI_INT, MASTER, procid, MPI_COMM_WORLD, &status);

		printf("Processor: %d, adding: ", procid);
		for(int i = 0; i < step; i++)
		{
			printf("+ %d ", subvector[i]);
			partial_sum += subvector[i];
		}
		printf(" = %d", partial_sum);

		MPI_Send(&partial_sum, 1, MPI_INT, MASTER, procid, MPI_COMM_WORLD);
	}

	MPI_Finalize();

}