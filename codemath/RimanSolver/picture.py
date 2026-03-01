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
filee = 'e.txt'
filer = 'ro.txt'
fileu = 'u.txt'
filep = 'press.txt'

filex = 'x_axis.txt'
filet = 'time.txt'
vectorsE = read_vectors_simple(filee)
vectorsR = read_vectors_simple(filer)
vectorsU = read_vectors_simple(fileu)
vectorsP = read_vectors_simple(filep)
x_axis = read_one_vector(filex)
Times= read_one_vector(filet)


    # Берем каждый 100-й вектор (индексы 0, 5, 10, 15, ...)
for i in range(0, len(vectorsE), 100):
       # График для vectorsE
    plt.figure(figsize=(12, 8))
    plt.scatter(x_axis, vectorsE[i], label=Times[i], linewidth=1.5)
    plt.xlabel('x_axis')
    plt.ylabel('E values')
    plt.title(f'График E от x (t={Times[i]})')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.5)
    plt.tight_layout()
    plt.show()

    # График для vectorsR
    plt.figure(figsize=(12, 8))
    plt.scatter(x_axis, vectorsR[i], label=Times[i], linewidth=1.5)
    plt.xlabel('x_axis')
    plt.ylabel('Ro values')
    plt.title(f'График R от x (t={Times[i]})')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.5)
    plt.tight_layout()
    plt.show() 
       # График для vectorsU
    plt.figure(figsize=(12, 8))
    plt.scatter(x_axis, vectorsU[i], label=Times[i], linewidth=1.5)
    plt.xlabel('x_axis')
    plt.ylabel('U values')
    plt.title(f'График U от x (t={Times[i]})')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.5)
    plt.tight_layout()
    plt.show()

    # График для vectorsP
    plt.figure(figsize=(12, 8))
    plt.scatter(x_axis, vectorsP[i], label=Times[i], linewidth=1.5)
    plt.xlabel('x_axis')
    plt.ylabel('P values')
    plt.title(f'График P от x (t={Times[i]})')
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.grid(True, alpha=0.5)
    plt.tight_layout()
    plt.show()
