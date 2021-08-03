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
import tkinter as tk        
from tkinter import *

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
from tkinter import *
root = Tk()

#variable is stored in the root object
root.counter = 0

def clicked():
    root.counter += 1
    L['text'] = 'Button clicked: ' + str(root.counter)
        
b = Button(root, text="Click Me", command=clicked)
b.pack()

L = Label(root, text="No clicks yet.")
L.pack()

root.mainloop()

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


