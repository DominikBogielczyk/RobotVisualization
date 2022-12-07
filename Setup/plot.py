import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np

fig, plot = plt.subplots(2, 2, constrained_layout=True)
fig2, plot2 = plt.subplots(2, 2, constrained_layout=True)

x_list = [0] * 200
y_list = [0] * 200
uL_list = [0] * 200
uR_list = [0] * 200
ωL_list = [0] * 200
ωP_list = [0] * 200
ωLref_list = [0] * 200
ωPref_list = [0] * 200
xref_list = [0] * 200
yref_list = [0] * 200
t = np.linspace(0,20,200)


def animate(counter):
    data = open('robot_data.txt', 'r').read()
    lines = data.split('\n')
    
    counter %= 20
    
    #i>counter*10, because we want to update only new data - 10 new samples/sec
    for i, line in enumerate(lines):
        if len(line) > 1 and i>counter*10:
            x, y, uL, uR, ωL, ωP, ωLref, ωPref, xref, yref = line.split(';')
            x_list[i-1]=float(x)
            y_list[i-1]=float(y)
            uL_list[i-1]=float(uL)
            uR_list[i-1]=float(uR)
            ωL_list[i-1]=float(ωL)
            ωP_list[i-1]=float(ωP)
            ωLref_list[i-1]=float(ωLref)
            ωPref_list[i-1]=float(ωPref)
            xref_list[i-1]=float(xref)
            yref_list[i-1]=float(yref)

    global t
    t = [value+1 for value in t]

    plot[0][0].clear()
    plot[0][0].set_title('PRĘDKOŚĆ - KOŁO LEWE')
    plot[0][0].set_xlabel('t [s]')
    plot[0][0].set_ylabel('')
    plot[0][0].axis(xmin=min(t), xmax=max(t))
    plot[0][0].plot(t, ωL_list, 'b')
    plot[0][0].plot(t, ωLref_list, 'r')
    plot[0][0].legend(['ωL', 'ωLref'])
  
    plot[0][1].clear()
    plot[0][1].set_title('SYGNAŁ STERUJĄCY - KOŁO LEWE')
    plot[0][1].set_xlabel('t [s]')
    plot[0][1].set_ylabel('uL [V]')
    plot[0][1].axis(xmin=min(t), xmax=max(t))
    plot[0][1].plot(t, uL_list, 'g')

    plot[1][0].clear()
    plot[1][0].set_title('PRĘDKOŚCI PRAWEGO KOŁA')
    plot[1][0].set_xlabel('t [s]')
    plot[1][0].set_ylabel('')
    plot[1][0].axis(xmin=min(t), xmax=max(t))
    plot[1][0].plot(t, ωP_list, 'b')
    plot[1][0].plot(t, ωPref_list, 'r')
    plot[1][0].legend(['ωP', 'ωPref'])
   
    plot[1][1].clear()
    plot[1][1].set_title('SYGNAŁ STERUJĄCY - KOŁO PRAWE')
    plot[1][1].set_xlabel('t [s]')
    plot[1][1].set_ylabel('uR [V]')
    plot[1][1].axis(xmin=min(t), xmax=max(t))
    plot[1][1].plot(t, uR_list, 'g')
    

def animate2(counter):
    plot2[0][0].clear()
    plot2[0][0].set_title('POZYCJA X ROBOTA')
    plot2[0][0].set_xlabel('t [s]')
    plot2[0][0].set_ylabel('pozycja x [cm]')
    plot2[0][0].axis(xmin=min(t), xmax=max(t))
    plot2[0][0].grid()
    plot2[0][0].plot(t, x_list, 'b')
    plot2[0][0].plot(t, xref_list, 'r')
    plot2[0][0].legend(['x', 'x_ref'])
    
    plot2[0][1].clear()
    plot2[0][1].set_title('POZYCJA XY ROBOTA')
    plot2[0][1].set_xlabel('pozycja x [cm]')
    plot2[0][1].set_ylabel('pozycja y [cm]')
    plot2[0][1].axis(xmin=min(x_list)-50, xmax=max(x_list)+50)
    plot2[0][1].axis(ymin=min(y_list)-50, ymax=max(y_list)+50)
    plot2[0][1].grid()
    plot2[0][1].plot(x_list, y_list, 'g')
    
    plot2[1][0].clear()
    plot2[1][0].set_title('POZYCJA Y ROBOTA')
    plot2[1][0].set_xlabel('t [s]')
    plot2[1][0].set_ylabel('pozycja y [cm]')
    plot2[1][0].axis(xmin=min(t), xmax=max(t))
    plot2[1][0].grid()
    plot2[1][0].plot(t, y_list, 'b')
    plot2[1][0].plot(t, yref_list, 'r')
    plot2[1][0].legend(['y', 'y_ref'])
    
    plot2[1][1].clear()
    plot2[1][1].set_title('ROBOT W POKOJU')
    plot2[1][1].set_xlabel('pozycja x [cm]')
    plot2[1][1].set_ylabel('pozycja y [cm]')
    plot2[1][1].axis(xmin=-600, xmax=600)
    plot2[1][1].axis(ymin=-300, ymax=300)
    plot2[1][1].grid()
    plot2[1][1].plot(x_list, y_list, 'g')


a1 = animation.FuncAnimation(fig, animate, interval=1000)
a2 = animation.FuncAnimation(fig2, animate2, interval=1000)
plt.show()