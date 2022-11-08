import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

fig, plot = plt.subplots(2, 2, constrained_layout=True)

x_list = [0] * 100
y_list = [0] * 100
v_list = [0] * 100
ω_list = [0] * 100
ωL_list = [0] * 100
ωP_list = [0] * 100
t = np.linspace(0,10,100)


def animate(i):
    data = open('robot_data.txt', 'r').read()
    lines = data.split('\n')
    
    for i, line in enumerate(lines):
        if len(line) > 1 and i>0:
            x, y, v, ω, ωL, ωP = line.split(';')
            x_list[i-1]=float(x)
            y_list[i-1]=float(y)
            v_list[i-1]=float(v)
            ω_list[i-1]=float(ω)
            ωL_list[i-1]=float(ωL)
            ωP_list[i-1]=float(ωP)

    global t
    t = [value+1 for value in t]


    plot[0][0].clear()
    plot[0][0].set_title('POZYCJA XY ROBOTA')
    plot[0][0].set_xlabel('pozycja x [cm]')
    plot[0][0].set_ylabel('pozycja y [cm]')
    plot[0][0].axis(xmin=-600, xmax=600)
    plot[0][0].axis(ymin=-300, ymax=300)
    plot[0][0].grid()
    plot[0][0].plot(x_list, y_list, 'b.')

    plot[0][1].clear()
    plot[0][1].set_title('PRĘDKOŚĆ LINIOWA ROBOTA')
    plot[0][1].set_xlabel('t [s]')
    plot[0][1].set_ylabel('v [cm/s]')
    plot[0][1].axis(xmin=min(t), xmax=max(t))
    plot[0][1].plot(t, v_list, 'g-.')

    plot[1][0].clear()
    plot[1][0].set_title('PRĘDKOŚCI KÓŁ ROBOTA')
    plot[1][0].set_xlabel('t [s]')
    plot[1][0].set_ylabel('')
    plot[1][0].axis(xmin=min(t), xmax=max(t))
    plot[1][0].plot(t, ωL_list, 'y-.')
    plot[1][0].plot(t, ωP_list, 'k-.')
    plot[1][0].legend(['ωL', 'ωP'])
   
    plot[1][1].clear()
    plot[1][1].set_title('PRĘDKOŚĆ OBROTOWA ROBOTA')
    plot[1][1].set_xlabel('t [s]')
    plot[1][1].set_ylabel('omega')
    plot[1][1].axis(xmin=min(t), xmax=max(t))
    plot[1][1].plot(t, ω_list, 'r-.')


a = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()