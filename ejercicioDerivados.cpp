/**
 * Implementación del cálculo de PI por medio de método montecarlo
 * a través de computación paralela 
 * Implementando el modelo de paso de mensajes 
 * a través de MPI
 * 
*/

#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;


void mostrarDataArray(int v[10][10]);  

int main(int argc, char * argv[])
{    

    int metodo = atoi(argv[1]);        
    int rank;      // Rango del proceso actual.
    int process;   // Número de procesos totales
    MPI_Init(&argc, &argv); //Iniciando entorno MPI
    MPI_Comm_size(MPI_COMM_WORLD, &process);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    
    int a[10][10];
    int valor = 0;    
    for(int i =0;i<10; i++)
    {        
        for(int y =0; y <10; y++)
        {
            a[i][y] = valor++;
        }
    }


    int x,y,z,i,j;
    if(metodo == 1)
    {
        /*Envío columan por columna*/
        /*Comienza en la fila 0 y varía entre proceso.*/
        x = 10; y = 1; z = 10; i = 0; j = rank-1;
    }  
    if(metodo ==2 )
    {      
        /*Envío de elementos separados por 5 posiciones*/  
        /*Se envían trozos*/
        x = 20; y = 1; z = 5; i = 0 ; j =  rank -1 ;
    }    
    if(metodo == 3)
    {
        /*Envio por dos conjuntos.*/
        /*Se envían números pares en un proceso y número impares en el segundo*/
        x = 50; y = 1; z= 2;  i = 0; j = rank-1;
    }
    if(metodo ==4 )
    {
        /*Envío en dos partes.*/
        /*El primer proceso enviará una diagonal de la matriz  \ */
        /*El segundo proceso enviará la otra diagonal de la matriz / */
        x= 10, y = 1; z =11 - (rank-1)*2 ; i = 0 ; j = (rank-1)*9 ;
    }

    if(rank == 0) // Proceso root, padre.
    {
        mostrarDataArray(a);
        int b[10][10];
        for(int x1 = 0 ; x1<10; x1++)
        {
            for(int x2 = 0 ;x2<10;x2++)
            {
                b[x1][x2] = 0;
            }
        }
        MPI_Status status; // Estado de la conexión
        for(int tmp =0; tmp < process- 1 ; tmp++)
        {
            printf("Esperando mensaje del proceso %d \n",tmp);
            MPI_Recv(&b[tmp*(10/(process-1))],x,MPI_INT,tmp+1, 0, MPI_COMM_WORLD, &status); 
            printf("Se han recibido %d datos",x);           
        }
       
        mostrarDataArray(b);
    }
    else
    {                  
        MPI_Datatype vector_fila;       // Creamos un nuevo tipo de dato        
        MPI_Type_vector(x,y,z, MPI_INT, &vector_fila); // Creamos un nuevo tipo de vector
        MPI_Type_commit(&vector_fila);  // Registramos en MPI el nuevo tipo
        MPI_Send(&a[i][j], 1, vector_fila, 0,0, MPI_COMM_WORLD); // Enviamos la data          
        MPI_Type_free(&vector_fila);
    }
    MPI_Finalize();
}

void mostrarDataArray(int b[10][10])
{
    printf("\n");
    /*Imprimos la data recolectada*/
    for(int y = 0 ; y < 10; y ++)
    {
        printf("|");
        for(int x = 0 ; x < 10 ; x++)
        {
            printf("%d|",b[y][x]);
        }
        printf("\n");
    }
}