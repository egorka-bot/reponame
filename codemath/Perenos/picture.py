import numpy as np
import matplotlib.pyplot as plt

def read_vectors_simple(filename):
    """
    Читает файл и возвращает список numpy массивов,
    где каждый массив соответствует строке в файле
    """
    vectors = []
    
    with open(filename, 'r') as file:
        for line in file:
            # Пропускаем пустые строки
            if line.strip():
                # Разделяем строку по пробелам и преобразуем в float
                numbers = [float(x) for x in line.strip().split()]
                vectors.append(np.array(numbers))
    
    return vectors

def read_one_vector(filename):
    with open(filename, 'r') as file:
        for line in file:
            # Пропускаем пустые строки
            if line.strip():
                # Разделяем строку по пробелам и преобразуем в float
                numbers = [float(x) for x in line.strip().split()]
    
    return np.array(numbers)

# Использование
filename = 'numbers.txt'
filenameLax = 'numbers_Lax.txt'
filename2 = 'x_axis.txt'
vectors = read_vectors_simple(filename)
vectorsLax = read_vectors_simple(filenameLax)
x_axis = read_one_vector(filename2)


    # Берем каждый 5-й вектор (индексы 0, 5, 10, 15, ...)
for i in range(0, len(vectors), 5):
    plt.figure(figsize=(12, 8))
    plt.plot(x_axis, vectors[i], label=f'vector[{i}], Ugolok', linewidth=1.5)
    plt.plot(x_axis, vectorsLax[i], label=f'vector[{i}], Lax', linewidth=1.5)

    plt.xlabel('x_axis')
    plt.ylabel('U values')
    plt.title(f'График U от x, time = CFL * h * {i}')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.5)
    plt.tight_layout()
    plt.show()
