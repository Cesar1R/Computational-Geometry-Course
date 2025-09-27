import numpy as np
import matplotlib.pyplot as plt 
import random
import ast
import subprocess
import time 
import os
import glob
from matplotlib.animation import FuncAnimation
import ast

def create_progress_animation(algorithm_name, process_dir, puntos_originales, output_gif=None):
    """
    Crea una animación del progreso del algoritmo de envolvente convexa
    """
    
    # Obtener la ruta de los archivos de progreso
    progress_path = os.path.join(process_dir, algorithm_name)
    
    if not os.path.exists(progress_path):
        print(f"Directorio de progreso no encontrado: {progress_path}")
        return
    
    # Obtener todos los archivos .txt de progreso
    progress_files = sorted(
        glob.glob(os.path.join(progress_path, "*.txt")),
        key=lambda x: int(''.join(filter(str.isdigit, os.path.basename(x))) or 0)
    )
    
    if not progress_files:
        print(f"No se encontraron archivos de progreso en: {progress_path}")
        return
    
    print(f"Encontrados {len(progress_files)} archivos de progreso")
    
    # Leer todos los datos de progreso
    progress_data = []
    iteration_numbers = []
    
    for file_path in progress_files:
        try:
            with open(file_path, 'r') as file:
                content = file.read().strip()
            
            # Extraer número de iteración del nombre del archivo
            filename = os.path.basename(file_path)
            iteration = int(''.join(filter(str.isdigit, filename)) or 0)
            iteration_numbers.append(iteration)
            
            # Parsear los puntos del convex hull
            points_list = []
            if content.startswith('[') and content.endswith(']'):
                try:
                    # Evaluar literalmente el contenido
                    parsed_content = ast.literal_eval(content)
                    if isinstance(parsed_content, list):
                        for point in parsed_content:
                            if isinstance(point, (list, tuple)) and len(point) == 2:
                                points_list.append([float(point[0]), float(point[1])])
                    elif isinstance(parsed_content, tuple):
                        # Si es una tupla de puntos
                        for point in parsed_content:
                            if hasattr(point, '__len__') and len(point) == 2:
                                points_list.append([float(point[0]), float(point[1])])
                except:
                    # Fallback: parsing manual
                    content_clean = content[1:-1].replace('(', '').replace(')', '')
                    points_str = content_clean.split(',')
                    for i in range(0, len(points_str)-1, 2):
                        try:
                            x = float(points_str[i].strip())
                            y = float(points_str[i+1].strip())
                            points_list.append([x, y])
                        except:
                            continue
            
            progress_data.append(np.array(points_list))
            print(f"Archivo {filename}: {len(points_list)} puntos")
            
        except Exception as e:
            print(f"Error leyendo archivo {file_path}: {e}")
            continue
    
    if not progress_data:
        print("No se pudieron cargar datos de progreso")
        return
    
    # Configurar la figura
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    fig.suptitle(f'Progreso del Algoritmo: {algorithm_name.replace("_", " ").title()}', 
                 fontsize=16, fontweight='bold')
    
    # Convertir puntos originales a numpy
    puntos_array = np.array(puntos_originales)
    
    # Configurar subplot 1: Puntos originales
    ax1.scatter(puntos_array[:, 0], puntos_array[:, 1], color='blue', alpha=0.7, s=30, label='Puntos')
    ax1.set_xlabel('X')
    ax1.set_ylabel('Y')
    ax1.set_title('Puntos Originales')
    ax1.grid(True, alpha=0.3)
    ax1.axis('equal')
    ax1.legend()
    
    # Configurar subplot 2: Animación del progreso
    ax2.set_xlabel('X')
    ax2.set_ylabel('Y')
    ax2.grid(True, alpha=0.3)
    ax2.axis('equal')
    ax2.set_title('Progreso del Algoritmo')
    
    # Inicializar elementos vacíos CORREGIDOS
    scatter_points = ax2.scatter(puntos_array[:, 0], puntos_array[:, 1], 
                               color='blue', alpha=0.5, s=30, label='Puntos')
    
    # Inicializar con arrays vacíos de la forma correcta
    scatter_hull = ax2.scatter([], [], color='red', s=80, label='Vértices Hull')
    line_hull, = ax2.plot([], [], 'r-', linewidth=2, label='Envolvente Convexa')
    text_iteration = ax2.text(0.02, 0.98, '', transform=ax2.transAxes, fontsize=12,
                             verticalalignment='top', 
                             bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
    
    ax2.legend()
    
    def init():
        """Inicialización CORREGIDA"""
        # Puntos originales ya están ploteados
        # Inicializar hull vacío
        scatter_hull.set_offsets(np.empty((0, 2)))  # Forma correcta para array vacío
        line_hull.set_data([], [])
        text_iteration.set_text('')
        return scatter_points, scatter_hull, line_hull, text_iteration
    
    def update(frame):
        """Actualizar la animación para cada frame"""
        if frame >= len(progress_data):
            return scatter_points, scatter_hull, line_hull, text_iteration
            
        hull_points = progress_data[frame]
        iteration = iteration_numbers[frame]
        
        # Limpiar el plot del hull anterior
        if len(hull_points) > 0:
            # Cerrar el polígono para la línea
            hull_closed = np.vstack([hull_points, hull_points[0]])
            line_hull.set_data(hull_closed[:, 0], hull_closed[:, 1])
            
            # Actualizar scatter de vértices
            scatter_hull.set_offsets(hull_points)
        else:
            # Si no hay puntos, limpiar
            line_hull.set_data([], [])
            scatter_hull.set_offsets(np.empty((0, 2)))
        
        # Actualizar texto de iteración
        text_iteration.set_text(f'Iteración: {iteration}\nVértices: {len(hull_points)}')
        
        # Título dinámico
        ax2.set_title(f'Progreso - Frame {frame+1}/{len(progress_data)}')
        
        return scatter_points, scatter_hull, line_hull, text_iteration
    
    # Crear animación
    try:
        animation = FuncAnimation(fig, update, frames=len(progress_data),
                                init_func=init, blit=False, interval=800, repeat=True)
        
        # Guardar como GIF si se especifica
        if output_gif:
            from matplotlib.animation import PillowWriter
            writer = PillowWriter(fps=2, bitrate=1800)
            animation.save(output_gif, writer=writer)
            print(f"Animación guardada como: {output_gif}")
        
        plt.tight_layout()
        plt.show()
        
        return animation
        
    except Exception as e:
        print(f"Error creando animación: {e}")
        # Mostrar al menos el resultado final
        plot_final_progress(progress_data[-1], puntos_originales, algorithm_name)
        return None

def sort_ConvexHull(puntos:np.ndarray) -> np.ndarray:
    centroide = np.mean(puntos, axis=0)
    def angulo(p):
        return np.arctan2(p[1] - centroide[1], p[0] - centroide[0])

    puntos_ordenados = sorted(puntos, key=angulo)
    return np.array(puntos_ordenados)

def plot_ConvexHull(puntos: list, 
                    convex_hull: np.ndarray, 
                    time: float,
                    title: str) -> None:
    
    # Convert puntos to numpy array if it's a list of tuples
    puntos_array = np.array(puntos)
    
    # Plot 1: Only the points
    plt.figure(figsize=(10, 5))
    
    plt.subplot(1, 2, 1)  # 1 row, 2 columns, first plot
    plt.scatter(puntos_array[:, 0], puntos_array[:, 1], color='blue', label='Puntos')
    plt.axis('equal')
    plt.grid(True, alpha=0.3)
    plt.title(f"Puntos - {title}")
    plt.legend()
    plt.xlabel('X')
    plt.ylabel('Y')
    
    # Plot 2: Points with convex hull
    plt.subplot(1, 2, 2)  # 1 row, 2 columns, second plot
    
    # Close the convex hull polygon
    x, y = zip(*convex_hull)
    x_closed = x + (x[0],)
    y_closed = y + (y[0],)
    
    plt.plot(x_closed, y_closed, 'r-', linewidth=2, label='Envolvente Convexa')
    plt.scatter(puntos_array[:, 0], puntos_array[:, 1], color='blue', alpha=0.7, label='Puntos')
    plt.scatter(x, y, color='red', s=50, zorder=5, label='Vértices')
    
    plt.axis('equal')
    plt.grid(True, alpha=0.3)
    plt.title(f"Envolvente Convexa - {title}\nTiempo: {time:.4f} s")
    plt.legend()
    plt.xlabel('X')
    plt.ylabel('Y')
    
    plt.tight_layout()
    plt.show()


def main():

    try:
        build_result = subprocess.run(["make"], capture_output=True, text=True)
        if build_result.returncode != 0:
            print("Build failed!")
            print(build_result.stderr)
            exit(1)
        else:
            ###Generacion de puntos
            print("Region [0, 100] x [0, 100]\n")
            print("Cantidad de puntos: ")
            # k_samples = int(input())
            k_samples = 100
            lower_x, lower_y = -100, -100
            upper_x, upper_y = 100, 100
            puntos = []
            for i in range(k_samples):
                x = random.uniform(lower_x, upper_x)
                y = random.uniform(lower_y, upper_y)
                puntos.append((x, y))

            algorithms = ["gift_wrapping", "graham_scan", "monotone_chain"]
            files_dir = "io_files"
            os.makedirs(files_dir, exist_ok=True)

            process_dir = "proces_images"
            os.makedirs(process_dir, exist_ok=True)

            implementatios_dir = "convexHull_implementations"

            for algorithm in algorithms: 
                print("Ejecucion de: ", algorithm)
                #Escritura de los puntos

                path_out = os.path.join(files_dir, algorithm + "_in.txt" ) 
                path_in = os.path.join(files_dir, algorithm + "_out.txt")
                path_process = process_dir + "/" + algorithm


                with open(path_out, 'w') as file:
                    file.write("[ ")
                    for i, (x, y) in enumerate(puntos):
                        coma = "," if i < len(puntos) - 1 else ""
                        file.write(f" ({x:.4f}, {y:.4f}) {coma} ")
                    file.write("]\n")

                os.makedirs(path_process, exist_ok=True)
                ### Ejecucion del codigo en python 
                start = time.time()
                path_ex = "./" + os.path.join(implementatios_dir, algorithm)
                result = subprocess.run([path_ex, path_out, path_in, path_process], capture_output = True, text = True)
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
                # plot_ConvexHull(puntos, sorted_ConvexHull, totalTime, algorithm)

                if path_process != "" and os.path.exists(path_process):
                    print("Creando animación del progreso...")
                    
                    # Opción 1: Animación completa
                    gif_path = f"{process_dir}/{algorithm}_animation.gif"
                    create_progress_animation(algorithm, process_dir, puntos, gif_path)
                    
    finally:    
        print("Cleaning")
        subprocess.run(["make", "clean"], capture_output=True, text=True)


if __name__ == "__main__":
    main()