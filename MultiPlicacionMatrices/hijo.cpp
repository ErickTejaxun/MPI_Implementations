#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main(int argc, char * argv[])
{
    int rank, ack , n = 10;    
    MPI_Init(&argc, &argv);         //Iniciando MPI
    MPI_Comm parent;                // Intercomunicador creado por el padre. 
    MPI_Comm_get_parent(&parent);   // Obtenemos el comunicador del padre

    if(parent == MPI_COMM_NULL)
    {
        printf("No existe el padre creado\n");
        return 0;
    }


    MPI_Comm_rank(parent,&rank);                            // Obtenemos el rango del proceso actual.       
    MPI_Recv(&n,1,MPI_INT,0,0,parent, MPI_STATUS_IGNORE);   // Recibimos el tamaño de la matriz
    printf("%d) tamaño recibido %d\n",rank,n);              
    /*Aquí no crearemos otro tipo de dato derivado ya que almacenaremos*/
    int A[n][n]; // Matriz de n*n
    int C[n];    // Valores de la columna en forma de fila
    int R[n];    // Fila de resultados
    for(int x = 0; x < n; x++){C[x]=0;} 

    MPI_Recv(&A,n*n, MPI_INT, 0, 0, parent, MPI_STATUS_IGNORE); // Recibimos la matriz completa.    
    MPI_Recv(&C,n,   MPI_INT, 0, 0, parent, MPI_STATUS_IGNORE); // Recibimos la columna   
    
    
    /*printf("|");
    for(int i= 0 ; i < n ; i++){printf("%d|",C[i]);}
    printf("\n");  
    */
    
    
    for(int i = 0; i < n; i++)
    {
        int resultado = 0;
        for(int j = 0 ; j< n; j++)
        {
            resultado = resultado + A[i][j] * C[j];
        }
        R[i] = resultado;        
    }
    

    /*printf("Resultado hijo %d\n",rank);
    printf("\n|");
    for(int i= 0 ; i < n ; i++){printf("%d|",R[i]);}
    printf("\n");
    */
    /*Ahora enviamos el resultado de la multiplicación*/
    MPI_Send(&R,n,MPI_INT,0,0,parent);

    MPI_Recv(&ack, 1, MPI_INT, 0,0, parent, MPI_STATUS_IGNORE);// Esperamos hasta que el padre confirme que se ha recibido el mensaje. Tipo TCP/IP
    
    MPI_Finalize();    
    return 0;
}