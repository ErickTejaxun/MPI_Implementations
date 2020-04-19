#include <mpi.h> // Librería MPI
#include <stdio.h> 
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
    int size, myrank, numProcSpw;
    MPI_Comm interComm; /*Comunicador que se crea al ejecutar los procesos hijos*/
    char cadena[100];
    int ack, length, i; 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);      //Número de procesos lanzados
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);     // Identificador del proceso actual.
    printf("Número de hijos que desea crear : ");
    scanf("%d", &numProcSpw); //Le preguntamos al usuario cuántos procesos hijos quiere lanzar.
    
    /*Creamos a los procesos hijos.
    * El primer parametro al lanzar este programa será la ruta al proceso hijo.
    * */
    MPI_Comm_spawn(argv[1], MPI_ARGV_NULL, numProcSpw, 
    MPI_INFO_NULL, 0, MPI_COMM_SELF, &interComm, MPI_ERRCODES_IGNORE);

    for(int i = 0 ; i < numProcSpw ; i++)
    {
        MPI_Recv(&length, 1, MPI_INT, i, 0, interComm, MPI_STATUS_IGNORE); //Preguntamos por el tamaño del mensaje a recibir
        MPI_Recv(cadena, length, MPI_CHAR, i, 0, interComm, MPI_STATUS_IGNORE); //Esperamos a que nos envien el mensaje
        printf("Proceso padre recibiendo cadena '%s' del hijo %d \n",cadena,i);
        MPI_Send(&ack, 1,MPI_INT,i,0,interComm);
    }
    MPI_Finalize();
    return 0;
} 