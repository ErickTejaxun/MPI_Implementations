/**
 * Ejemplo de implementación de comunación punto a punto. 
 * El proceso padre envía a los hijos data para ser procesada
 * a través de comunicación síncrona, es decir, bloqueante.  
*/
#include <stdio.h>
#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[])
{        
    int k; //Rango del proceso
    int p; //Número de procesos
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&k); //Obtenemos el rango del proceso actual.
    MPI_Comm_size(MPI_COMM_WORLD,&p); //Número de procesos lanzados en el mismo comunicador.
    int number_amount;
    if(k==0)
    {
       	cout<<"Soy el proceso padre de "<< p<<endl;
	    char * nombreCPU;
	    int strlen = 20;
	    MPI_Get_processor_name(nombreCPU, &strlen);
	    cout<<"CPU: \t"<<nombreCPU<<endl;
                    
        /*Comenzamos a enviar información*/
        const int MAX_NUMBER = 100;
        int numbers[MAX_NUMBER];
        for(int i =0 ; i < MAX_NUMBER; i++)
        {
            numbers[i] =i;
        }
        srand(time(NULL));        
        for(int x = 1; x < p ;x++)
        {
            /*Definimos de manera aleatoria el número de datos a enviar*/
            number_amount = (rand()/ (float) RAND_MAX)* MAX_NUMBER; 
            MPI_Send(numbers, number_amount, MPI_INT, x,0, MPI_COMM_WORLD);
            printf("0 envía %d numbers to %d \n",number_amount, x);
        }
    }    
    else 
    {
        MPI_Status status;
        MPI_Probe(0,0,MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &number_amount);
        int * number_buf =(int*)malloc(sizeof(int) * number_amount);
        MPI_Recv(number_buf, number_amount, MPI_INT, 0,0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        printf("%d dynamically received %d number from 0\n",k,number_amount);
        free(number_buf);
    }
    MPI_Finalize();
}

