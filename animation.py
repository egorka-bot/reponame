import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import struct

# Чтение бинарного файла
def read_solution(filename):
    with open(filename, 'rb') as f:
        # Читаем размеры
        dims = struct.unpack('QQ', f.read(16))  # Q для size_t (8 байт)
        N_t, N_h = dims
        
        # Читаем данные
        data = np.frombuffer(f.read(), dtype=np.float64)
        data = data.reshape(N_t, N_h)
        
    return data, N_t, N_h

# Загружаем данные
solution, N_t, N_h = read_solution('solution.bin')
print(f"Загружено: {N_t} временных шагов, {N_h} пространственных точек")

# Создаем координаты
x = np.linspace(0, 1, N_h)
t = np.linspace(0, 1, N_t)

# Настройка графика
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

# Верхний график - анимация профиля
line, = ax1.plot(x, solution[0], 'b-', linewidth=2)
ax1.set_xlim(0, 1)
ax1.set_ylim(0, 12)
ax1.set_xlabel('x')
ax1.set_ylabel('U')
ax1.set_title('Эволюция профиля')
ax1.grid(True, alpha=0.3)
ax1.axhline(y=10, color='r', linestyle='--', alpha=0.5, label='Граничное условие')
ax1.legend()

# Текст с текущим временем
time_text = ax1.text(0.02, 0.95, '', transform=ax1.transAxes, 
                     bbox=dict(boxstyle='round', facecolor='wheat', alpha=0.5))

# Нижний график - пространственно-временная диаграмма
im = ax2.imshow(solution, aspect='auto', origin='lower', 
                extent=[0, 1, 0, 1], cmap='viridis')
ax2.set_xlabel('x')
ax2.set_ylabel('t')
ax2.set_title('Пространственно-временная диаграмма')
plt.colorbar(im, ax=ax2, label='U')

# Линия, показывающая текущий временной срез на нижнем графике
current_time_line = ax2.axhline(y=0, color='r', linewidth=1)

def animate(frame):
    # Обновляем верхний график
    line.set_ydata(solution[frame])
    time_text.set_text(f'Время: t = {t[frame]:.3f}')
    
    # Обновляем линию на нижнем графике
    current_time_line.set_ydata([t[frame], t[frame]])
    
    return line, time_text, current_time_line

# Создаем анимацию
anim = FuncAnimation(fig, animate, frames=N_t, interval=20, blit=True)

# Сохраняем анимацию (раскомментируйте для сохранения)
# anim.save('solution_animation.mp4', writer='ffmpeg', fps=30, dpi=200)
# anim.save('solution_animation.gif', writer='pillow', fps=30)

plt.tight_layout()
plt.show()

# Дополнительно: несколько профилей в разные моменты времени
plt.figure(figsize=(10, 6))
time_steps = [0, N_t//4, N_t//2, 3*N_t//4, N_t-1]
for step in time_steps:
    plt.plot(x, solution[step], label=f't = {t[step]:.3f}', linewidth=2)
plt.xlabel('x')
plt.ylabel('U')
plt.title('Профили в разные моменты времени')
plt.legend()
plt.grid(True, alpha=0.3)
plt.ylim(0, 12)
plt.show()

# Статистика
print(f"\nСтатистика:")
print(f"Максимальное значение: {np.max(solution):.3f}")
print(f"Минимальное значение: {np.min(solution):.3f}")
print(f"Среднее значение: {np.mean(solution):.3f}")
