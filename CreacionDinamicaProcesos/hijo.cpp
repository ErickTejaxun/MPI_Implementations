#include <mpi.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char* argv[])
{
    int ack, length;
    char cadena[100];

    MPI_Comm parent; // Comunicador que ha creado el padre. Este nos permite saber como comunicarnos con él
    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parent);
    if(parent == MPI_COMM_NULL)
    {
        printf("No existe padre creado\n");
        return 0;
    }

    int rank;
    MPI_Comm_rank(parent, &rank);

    sprintf(cadena, "Proceso %d envia Hola al proceso padre", rank);
    length = strlen(cadena);
    length++;    

    MPI_Send(&length, 1, MPI_INT, 0,0, parent); /*Le enviamos al proceso padre ,0, el tamaño de la cadena a través del comunicador*/
    MPI_Send(cadena, length, MPI_CHAR,0,0,parent); /*Le enviamos la cadena al padre*/
    MPI_Recv(&ack, 1, MPI_INT, 0,0, parent, MPI_STATUS_IGNORE);// Esperamos hasta que el padre confirme que se ha recibido el mensaje. Tipo TCP/IP
    MPI_Finalize();
    return 0; 
}