# Proyecto 1: Convex Hull
El proyecto consta de tres implementaciones principales, son algoritmos que calculan la cubierta convexa de un conjunto de puntos dado. 

Las tres implementaciones se basan en la libreria geo_prop, en geo_prop.h se encuentran las declaracinoes de las estructuras * point * y * vec * que almacenan un punto y un vector, respectivamente, en dos dimensiones. Esta libreria incluye operaciones entre vectores de utilidad, como el calculo del producto cruz, la estimacion del angulo entre dos vectores, norma del vector, entre otras. 

## Implementaciones 
Implemente los algoritmos
    1. Gift wrapping
    1. Grahan Scan
    1. Monotone Chain

El archivo makefile cuenta con las instrucciones para compilar y generar los ejecutables de los tres algoritmos, para ejecutarlo solo es necesario hacer uso del comando * make * en terminal. Si se desea compilar unicamente uno de los algoritmos se puede utilizar el mismo archivo con el nombre del algoritmo * make  <nombre del algoritmo> *. 

## Ejecucion 
La ejecucion se hace por medio del archivo PlotConvexHull.py, este archivo contiene la logica de generar puntos 100 aleatorios, en el recuadro region de $[-100, 100] x [-100, 100]$. Ejecuta el archivo makefile y ejecuta los tres codigos con el mismo conjunto de puntos. Genera un gift para cada uno de los algoritmos del proceso que siguieron. 

Los paquetes instalados para python se encuentran en el archivo requeriments.txt