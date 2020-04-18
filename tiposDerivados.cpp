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

int main(int argc, char * argv[])
{    
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
            a[i][y] = ++valor;
        }
    }

    if(rank == 0) // Proceso root, padre.
    {
        int b[10][10];
        MPI_Status status; // Estado de la conexión
        for(int i =0; i < 10 ; i++)
        {
            /**
             * Recibimos cada fila que envía cada uno de los procesos hijos
             * Y los almacenamos en la fila correspondiente a la matriz local
             * que se ha declarado.
             * 
             * MPI_Recv es bloqueante, y como estamos indicando en i+0 que va a esperar
             * a los envíos de los procesos hijo en orden, a matriz estará en orden.
            */
            MPI_Recv(&b[i],10,MPI_INT,i+1, 0, MPI_COMM_WORLD, &status);            
        }
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
    else
    {
        int tipoEnvio = 0; // 0 es por fila, 1 por fila
        if(tipoEnvio == 0 )
        {
            int ejercicio = 0; 
            /**
             * 0 = enviar por columna
             * 1 = enviar por dos columnas consecuitvas
             * 2 = enviar de modo \
             * 3 = enviar de modo /
            */
            switch (ejercicio)
            {
                case 0:
                    MPI_Datatype vector_columnas;       // Creamos un nuevo tipo de dato
                    MPI_Type_vector(10,1,10, MPI_INT, &vector_columnas); // Creamos un nuevo tipo de vector
                    MPI_Type_commit(&vector_columnas);  // Registramos en MPI el nuevo tipo
                    MPI_Send(&a[0][rank-1], 1, vector_columnas, 0,0, MPI_COMM_WORLD);
                    MPI_Type_free(&vector_columnas);    // Eliminamos el tipo de MPI                 
                    break;
                case 1:
                    MPI_Datatype vector_columnas_doble;       // Creamos un nuevo tipo de dato
                    MPI_Type_vector(20,1,20, MPI_INT, &vector_columnas_doble); // Creamos un nuevo tipo de vector
                    MPI_Type_commit(&vector_columnas_doble);  // Registramos en MPI el nuevo tipo
                    MPI_Send(&a[0][rank-1], 1, vector_columnas_doble, 0,0, MPI_COMM_WORLD);
                    MPI_Type_free(&vector_columnas_doble);    // Eliminamos el tipo de MPI                 
                    break;                
                    break;
            }
           
        }
        else
        {
            MPI_Datatype vector_fila;       // Creamos un nuevo tipo de dato
            MPI_Type_vector(10,1,10, MPI_INT, &vector_fila); // Creamos un nuevo tipo de vector
            MPI_Type_commit(&vector_fila);  // Registramos en MPI el nuevo tipo
            MPI_Send(&a[rank-1][0], 1, vector_fila, 0,0, MPI_COMM_WORLD); // Enviamos la data
            MPI_Type_free(&vector_fila);
            /*En este caso podemos que al ejecutar que nos dará errores
            * Ya que MPI_Send va a enviar todo lo que esté en memoria CONTINUA            
            */ 
        }
    }
    MPI_Finalize();
}