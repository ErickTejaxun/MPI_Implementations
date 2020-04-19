/**
 * Erick Tejaxun
 * MPI Multiplicación de matrices
 * Comptucación Paralela
*/

#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char * argv[])
{
    int rank, size, ack = 0 , n = 10;
    MPI_Init(&argc, &argv);    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm interComm; // Comunicador de este proceso como padre.
    


    if(argc>2){n = atoi(argv[2]);}
    int A[n][n], B[n][n]; //Declaración de matrices
    int Resultado[n][n];  // Declaracion de la matriz resultado
    for(int i = 0 ; i< n; i++)
    {
        for(int j=0 ; j< n; j++)
        {
            A[i][j] = (j+1) + (i*n);
            B[i][j] = (j+1) + (i*n);
            Resultado[i][j] = 0;
        }
    }

    /*Creamos el tipo derivado columna*/
    MPI_Datatype tipoColumna;                      
    MPI_Type_vector(n,1,n,MPI_INT, &tipoColumna); 
    MPI_Type_commit(&tipoColumna);

    /*Creamos los procesos hijos*/
    MPI_Comm_spawn(argv[1],MPI_ARGV_NULL,n,
        MPI_INFO_NULL,0,MPI_COMM_SELF,&interComm,MPI_ERRCODES_IGNORE);

    printf("Se han lanzado %d procesos.\n",n);


    /*Ahora enviamos a cada nuevo proceso hijo 
    * 1. El tamaño de las dimensiones
    * 2. La Matriz A completa. 
    * 3. La columna i-esima de la matriz B
    * */
    for(int i =0 ; i < n; i++)
    {        
        MPI_Send(&n,1,MPI_INT,i,0,interComm);           // Enviamos el tamaño de n
        MPI_Send(&A,n*n,MPI_INT,i,0, interComm);        // Enviamos la matriz A        
        MPI_Send(&B[0][i],1,tipoColumna,i,0,interComm); // Enviamos la columna i-esima                
    }


    /*Recolectamos los resultados*/
    for(int i = 0 ; i< n; i++)
    {
        MPI_Recv(&Resultado[0][i],1,tipoColumna,i,0,interComm,MPI_STATUS_IGNORE);        
    }
    
    for(int i = 0 ;  i< n ; i++)
    {
        MPI_Send(&ack,1,MPI_INT,i,0,interComm); // Terminamos a cada uno de los hijos. 
    }

    /*Ahora imprimos la matriz resultante*/
    printf("\nEl resultado es: \n");
    for(int i = 0 ; i< n; i++)
    {
        printf("|");
        for(int j=0 ; j< n; j++)
        {
            printf("%d|",Resultado[i][j]);
        }
        printf("\n");
    }   
    MPI_Type_free(&tipoColumna);  
    MPI_Finalize();
    return 0;
}