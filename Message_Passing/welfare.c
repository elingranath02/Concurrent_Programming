#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_VALUE 40
#define ARRAY_LENGTH 20



int randoms(int max) {
    int r = 0;
    while (r == 0) {
        r = rand() % max;
    }
    return r;
}

int *generateSet(int array []){

    for(int i = 0; i < ARRAY_LENGTH; i++){
        array[i] = 0;

        while(array[i] == 0){
            int random = randoms(MAX_VALUE);
            int j = 0;

            while (j < i){
                if(array[j] == random){
                    break;    
                }else j++;
            }
            if(j == i){
                array[i] = random;
            }
            
        }
    }
    return array;  
}

int checkArray(int array [], int number){
    for(int i = 0; i < ARRAY_LENGTH; i++){
        if(array[i]== number){
            return 1;
        }
    }return 0;
}

void printAll(int array[], int ran){
    for(int i = 0; i < ARRAY_LENGTH;i++){
        printf("%d from rank:%d\n", array[i], ran);
        
    }
    printf("\n");
}


int main (int argc, char** argv){
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    int *IBM = malloc(ARRAY_LENGTH * sizeof(int));
    int *Columbia = malloc(ARRAY_LENGTH * sizeof(int));
    int *Welfare = malloc(ARRAY_LENGTH * sizeof(int)); 

    if(rank == 0){
        printf("Hello World from process %d of %d\n", rank, size);
        generateSet(IBM);
        printAll(IBM, rank);
        int *receive0 = malloc(sizeof(int));
        int *found0 = malloc(sizeof(int));

        for(int i = 0; i < ARRAY_LENGTH; i++){
            
            MPI_Send(&IBM[i], 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            
            MPI_Recv(found0, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if(*found0 == 1){
                printf("Number %d exists in IBM array\n", IBM[i]);
                printf("\n");
            }
            
        }
        free(receive0);
        free(found0);
    }

    if(rank == 1){
        printf("Hello World from process %d of %d\n", rank, size);
       
        generateSet(Columbia);
        printAll(Columbia, rank);
        int *receive1 = malloc(sizeof(int));
        int *found1 = malloc(sizeof(int));

        for(int i = 0; i < ARRAY_LENGTH; i++){

            MPI_Recv(receive1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            *found1 = checkArray(Columbia,*receive1);

            MPI_Send(receive1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
            MPI_Send(found1, 1, MPI_INT, 2, 2, MPI_COMM_WORLD);
            
            MPI_Recv(found1, 1, MPI_INT, 2, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(*found1 == 1){
                printf("Number %d exists in Columbia array\n", *receive1);
                printf("\n");
            }
            MPI_Send(found1, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

        }
    free(receive1);
    free(found1);
    }
    

    if(rank==2){
        printf("Hello World from process %d of %d\n", rank, size);
        generateSet(Welfare);
        printAll(Welfare, rank);

        int *receive2 = malloc(sizeof(int));
        int *found2 = malloc(sizeof(int));
        int *found3 = malloc(sizeof(int));

        for(int i = 0; i < ARRAY_LENGTH; i++){
            MPI_Recv(receive2, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(found2, 1, MPI_INT, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if(*found2 == 1){
                *found3 = checkArray(Welfare, *receive2);
            }else {
                *found3 = 0;
            }
        
            if (*found3 == 1)
                {
                    printf("Number %d exists in Welfare array\n", *receive2);
                    printf("\n");
                }
            MPI_Send(found3, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    
        }
        free(found2);
        free(receive2);

    }

    MPI_Finalize();

    free(IBM);
    free(Columbia);
    free(Welfare);
    


    return 0;
}