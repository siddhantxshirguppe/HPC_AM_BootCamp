#include<stdio.h>
#include "mpi.h"
#include <string.h>
#define N 3

void printMatrix(int mat[N][N], int n)
{
    for(int i = 0;i<n;i++)
    {
        for(int j =0;j<n;j++)
        {
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    int rank, size;
    int A[N][N], B[N][N], C[N][N], local_A[N], local_C[N];
    int i, j, k;

    int  tag, ierror;
    MPI_Status status;
    char send_message[20], recv_message[20];

    MPI_Init(&argc,&argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("number of processes: %d\n",size);
    printf("my rank:%d\n",rank);

    tag = 100;
    strcpy(send_message, "Hello-Participants");

    if(rank == 0)
    {
        int cnt = 0;
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {

                A[i][j] = cnt;
                B[i][j] = i;
                cnt++;
            }
        }

        printf("Matrix A--->\n");
        printMatrix(A,N);
        printf("Matrix B--->\n");
        printMatrix(B,N);


    }
    else 
    {
        printf("nonroot proceess waiting for matrix data..");
    }


    // Scatter rows of A to different processes
    printf("scattering row data to all the porcesses\n");
    MPI_Scatter(A, N*N/size, MPI_INT, local_A, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);
    printf("printing the received matrix row: N: %d SIZE:%d\n",N,size);
    for(int a = 0;a<N*N/size;a++)
    {
        printf("%d ",local_A[a]);        
    }

    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);




int cols = N;
int rows = N;
printf("Perform local matrix multiplication: %d %d\n",rows,cols);
for (i = 0; i < cols; i++) {
    local_C[i] = 0;
    for (j = 0; j < rows; j++) {
        //printf("local_A:%d  bmatrix:%d",local_A[j],B[i][j]);
        local_C[i] +=(local_A[j] * B[j][i]);
    }
}

    printf("printing computed row: from process: %d :\n",rank);
    for(int a = 0;a<N*N/size;a++)
    {
        printf("%d ",local_C[a]);        
    }


    // Gather local results to construct the final C matrix
    MPI_Gather(local_C, N*N/size, MPI_INT, C, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);


    // Print the result on rank 0
    if (rank == 0) {
        printf("\nMatrix C (Result of A * B):\n");
        printMatrix(C, N);
    }


    MPI_Finalize();

}