import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

fig, plot = plt.subplots(2, 2, constrained_layout=True)

plot[0][0].set_title('POZYCJA XY ROBOTA')
plot[0][0].set_xlabel('pozycja x [cm]')
plot[0][0].set_ylabel('pozycja y [cm]')
plot[0][0].axis(xmin=-600, xmax=600)
plot[0][0].axis(ymin=-300, ymax=300)
plot[0][0].grid()

plot[0][1].set_title('PRĘDKOŚĆ LINIOWA ROBOTA')
plot[0][1].set_xlabel('t [s]')
plot[0][1].set_ylabel('v [cm/s]')

plot[1][0].set_title('PRĘDKOŚCI KÓŁ ROBOTA')
plot[1][0].set_xlabel('t [s]')
plot[1][0].set_ylabel('')

plot[1][1].set_title('PRĘDKOŚĆ OBROTOWA ROBOTA')
plot[1][1].set_xlabel('t [s]')
plot[1][1].set_ylabel('omega')

loaded_lines = 0
x_list = []
y_list = []
v_list = []
ω_list = []
ωL_list = []
ωP_list = []


def animate(i):
    data = open('robot_data.txt', 'r').read()
    lines = data.split('\n')

    global loaded_lines

    for i, line in enumerate(lines):
        if i > loaded_lines and len(line) > 1:
            x, y, v, ω, ωL, ωP = line.split(';')
            x_list.append(float(x))
            y_list.append(float(y))
            v_list.append(float(v))
            ω_list.append(float(ω))
            ωL_list.append(float(ωL))
            ωP_list.append(float(ωP))

    t = np.linspace(0, len(v_list) / 10, len(v_list))
    loaded_lines = len(v_list)

    plot[0][0].plot(x_list, y_list, 'b.')

    plot[0][1].axis(xmin=0, xmax=max(t))
    plot[0][1].plot(t, v_list, 'g-.')

    plot[1][0].axis(xmin=0, xmax=max(t))
    plot[1][0].plot(t, ωL_list, 'y-.')
    plot[1][0].plot(t, ωP_list, 'k-.')
    plot[1][0].legend(['ωL', 'ωP'])
   
    plot[1][1].axis(xmin=0, xmax=max(t))
    plot[1][1].plot(t, ω_list, 'r-.')


a = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()