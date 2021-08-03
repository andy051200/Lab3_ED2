'''------------------------------------------------------------------------------
Autor: Andy Bonilla
Programa: laboratorio 3
Creado: 1 de agosto de 2021    
Descripcion: un laboratoria bien fumado tbh pero chilero
intefaz gráfica para el laboratorio de comunicacion SPI
-------------------------------------------------------------------------------'''

'''------------------------------------------------------------------------------
-------------------------IMPORTAR LIBRERIAS--------------------------------------
------------------------------------------------------------------------------'''
import tkinter as tk            #se importa libreria de GUI
from tkinter import *           #se importa libreria de GUI
import serial                   #se importa libreria de comunicacion serial
'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
from tkinter import *           #se llama la funcion
root = Tk()                     #se le da nombre al objeto principal

#char wenas[15]

#DEFINICION DE PUERTO SERIAL
#port1=serial.Serial("COM3",9600)        #declarar puerto serial y braudeaje

#variable is stored in the root object
root.counter = 0                #se declara una variables en el objeto

#se define funcion para sumar
def plus_clicked():                                          #se define funcion para sumar
    root.counter += 1
    L['text'] = 'Contador verga: ' + str(root.counter)
    port1.write(hex(31))   #se manda 1 en ascii

#se define funcion para restar
def minus_clicked():                                          #se define funcion para sumar
    root.counter -= 1
    L['text'] = 'Contador verga: ' + str(root.counter)
    port1.write(hex(32))   #se manda 1 en ascii

def pot1():
    cuenta_port=port1.read(1)
    if (cuenta_port==hex(32))

#TITULO
titulo=tk.Label(root,text = "GUI para laboratorio 3, Electrónica Digital 2") #texto como titulo de GUI
titulo.place(x=90, y=20)

#titulo de la ventana
root.title("GUI Lab3, Electronica Digital 2")   #le pones titulo al objeto
root.minsize(400,300)                           #le decis el tamaño a la ventana

#texto para indicar que putas hacen los botones
label = tk.Label(root, text = "Contador para el Portx del PIC")        #texto para el cuadro de texto
label.place(x=115,y=50)

#boton de suma
b1 = Button(root, text="Suma", command=plus_clicked)
b1.place(x=150, y=75)

#boton de resta
b2 = Button(root, text="Resta", command=minus_clicked)
b2.place(x=200,y=75)

#POTENCIOMETRO1
#texto indicador
label1 = tk.Label(root, text = "Valor potenciometro 1")        #texto para el cuadro de texto
label1.place(x=70,y=110)                                                         #ubicacion del texto para contador
pot1=tk.LabelFrame(root, text="valor")
pot1.place(x=70,y=125)

#POTENCIOMETRO2
#texto indicador
label2 = tk.Label(root, text = "Valor potenciometro 2")        #texto para el cuadro de texto
label2.place(x=210,y=110)                                                         #ubicacion del texto para contador


L = Label(root, text="No clicks yet.")
L.pack()

root.mainloop()

#sprintf(s,'contador1%3.2F',)
'''window = tk.Tk()
 
window.title("GUI Lab3, Electronica Digital 2")
window.minsize(400,300)

#BOTON PARA CONECTAR 

 
def salida():
    exit()

titulo=tk.Label(window,text = "GUI para laboratorio 3, Electrónica Digital 2") #texto como titulo de GUI
titulo.place(x=90, y=20)                                                       #ubicacion en la pantalla

#CUADRO DE TEXTO PARA CONTADOR PC-> PIC
label = tk.Label(window, text = "Ingrese el valor del PortB MasterPic")        #texto para el cuadro de texto
label.place(x=10,y=50)                                                         #ubicacion del texto para contador
boton_suma=boton_enviar=tk.Button(window,text="Sumar")
boton_suma.place(x=10,y=80)
boton_resta=boton_enviar=tk.Button(window,text="Resta")
boton_resta.place(x=60,y=80)

#BOTON DE MANDAR POR UART A PIC
boton_enviar=tk.Button(window,text="Enviar",command=salida)
boton_enviar.place(x=150,y=80)

#ESPACIO PARA MOSTRAR 

window.mainloop()'''


