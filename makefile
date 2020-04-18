# Makefile para compilación del programa MPI


ejecutar2:
	mpiexec -n 11 ./ejercicio 1
	mpiexec -n 6  ./ejercicio 2
	mpiexec -n 3  ./ejercicio 3
	mpiexec -n 3  ./ejercicio 4 

tiposDerivados2: ejercicioDerivados.cpp
	mpic++ ejercicioDerivados.cpp -o ejercicio

tiposDerivados: tiposDerivados.cpp
	mpic++ tiposDerivados.cpp -o tiposDerivados

padre: puntopunto.cpp
	mpic++ puntopunto.cpp -o puntopunto

ejecutar1:
	mpiexec -n 8 ./puntopunto

ejecutarTipos:
	mpiexec -n 11 ./tiposDerivados

clean:
	rm puntopunto tiposDerivados ejercicio
