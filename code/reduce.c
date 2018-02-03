#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size;
    double* array = NULL;
    const int N = 1024*1024*100;
    double mysum[1];
    double globalsum[1];
    struct timeval  t1, t2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int myN = N / size;

    array = (double*)malloc(sizeof(double)*myN);
    for(int i = 0; i != myN; ++i)
        array[i] = rank+1;

    gettimeofday(&t1, NULL);

    mysum[0] = 0.0;
    for(int i = 0; i != myN; ++i)
        mysum[0] += array[i];

    MPI_Reduce(mysum, globalsum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    gettimeofday(&t2, NULL);
    double time_spent = (double) (t2.tv_usec - t1.tv_usec) / 1000000 + (double) (t2.tv_sec - t1.tv_sec);

    double expected_result = 0;
    for(int i = 0; i != size; ++i)
        expected_result += (i+1)*myN;

    if(rank == 0) {
        if(expected_result != globalsum[0])
            printf("Wrong result, expected %f, got %f\n", expected_result, globalsum[0]);
        printf("C Sum time: %f s\n", time_spent);
    }

    free(array);
    MPI_Finalize();
    return 0;
}
