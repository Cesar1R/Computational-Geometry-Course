import numpy as np
import matplotlib.pyplot as plt 
import random
import ast
import subprocess
import time 
import os

def sort_ConvexHull(puntos:np.ndarray) -> np.ndarray:
    centroide = np.mean(puntos, axis=0)
    def angulo(p):
        return np.arctan2(p[1] - centroide[1], p[0] - centroide[0])

    puntos_ordenados = sorted(puntos, key=angulo)
    return np.array(puntos_ordenados)

def plot_ConvexHull(puntos:np.ndarray, 
                    convex_hull:np.ndarray, 
                    time: float,
                    title: str) -> None:
    x, y = zip(*convex_hull)

    x += (x[0], )
    y += (y[0], )

    plt.plot(x, y, 'r-', label='Envolvente Convexa')
    plt.scatter(*zip(*puntos), color = 'blue', label='puntos')
    plt.legend()
    plt.axis('equal')
    plt.title(f"Envolvente convexa de {title}. Tiempo: {time:4f} s")
    plt.show(block=False)



def main():

    ###Generacion de puntos
    k_samples = 30
    lower_x, lower_y = 20, 0
    upper_x, upper_y = 100, 50
    puntos = []
    for i in range(k_samples):
        x = random.uniform(lower_x, upper_x)
        y = random.uniform(lower_y, upper_y)
        puntos.append((x, y))

    algorithms = ["gift_wrapping"]
    files_dir = "io_files"
    implementatios_dir = "convexHull_implementations"
    os.makedirs(files_dir, exist_ok=True)

    for algorithm in algorithms: 
        print("Ejecucion de: ", algorithm)
        #Escritura de los puntos

        path_out = os.path.join(files_dir, algorithm + "_in.txt" ) 
        path_in = os.path.join(files_dir, algorithm + "_out.txt")


        with open(path_out, 'w') as file:
            file.write("[ ")
            for i, (x, y) in enumerate(puntos):
                coma = "," if i < len(puntos) - 1 else ""
                file.write(f" ({x:.4f}, {y:.4f}) {coma} ")
            file.write("]\n")

        ### Ejecucion del codigo en python 
        start = time.time()
        path_ex = "./" + os.path.join(implementatios_dir, algorithm)
        result = subprocess.run([path_ex, path_out, path_in], capture_output = True, text = True)
        end = time.time()
        totalTime = end - start 
        print("STDOUT: ", result.stdout)
        print("STDERR: ", result.stderr)

        #Lectura de resultado

        with open(path_in, 'r') as file:
            conten = file.read()

        lista_puntos = ast.literal_eval(conten)
        puntos_np = np.array(lista_puntos) 

        ###Procesamiento y ploteo de las
        sorted_ConvexHull = sort_ConvexHull(puntos_np)
        plot_ConvexHull(puntos, sorted_ConvexHull, totalTime, algorithm)


if __name__ == "__main__":
    main()