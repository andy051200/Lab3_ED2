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
import serial
'''------------------------------------------------------------------------------
-----------------------DEFINICION DE OBJETOS-----------------------------------
------------------------------------------------------------------------------'''
window = tk.Tk()
 
'''------------------------------------------------------------------------------
-----------------------DEFINICION PUERTO SERIAL-----------------------------------
------------------------------------------------------------------------------''' 
#DEFINICION DE PUERTO SERIAL
port1=serial.Serial('COM1')                                 #declarar puerto serial y braudeaje
port1.baudrate = 9600                                       #set Baud rate to 9600
port1.bytesize = 8                                          # Number of data bits = 8
port1.parity   ='N'                                         # No parity
port1.stopbits = 1                                          # Number of Stop bits = 1

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
window.title("GUI Lab3, Electronica Digital 2")
window.minsize(400,300)

#BOTON PARA CONECTAR 
def clickMe():
    label.configure(text= name.get())
    port1.write(name.get())
 
def salida():
    exit()

titulo=tk.Label(window,text = "GUI para laboratorio 3, Electrónica Digital 2") #texto como titulo de GUI
titulo.place(x=90, y=20)                                                       #ubicacion en la pantalla

#CUADRO DE TEXTO PARA CONTADOR PC-> PIC
label = tk.Label(window, text = "Ingrese el valor del PortB MasterPic")        #texto para el cuadro de texto
label.place(x=10,y=50)                                                         #ubicacion del texto para contador
name = tk.StringVar()                                                          #comando para iniciar el cuadro de texto
nameEntered = tk.Entry(window, width = 15, textvariable = name)
nameEntered.place(x=50, y=70)
print(nameEntered)

#BOTON PARA DESCONECTAR GUI
desconectar=tk.Button(window, text="Desconectar", command=exit)                 #texto para boton de desconectar
desconectar.place(x=140, y=100)                                                 #ubicacion para el boton

#BOTON DE MANDAR POR UART A PIC
boton_enviar=tk.Button(window,text="Enviar",command=clickMe)
boton_enviar.place(x=150,y=70)

'''------------------------------------------------------------------------------
---------------------------------MAIN LOOP---------------------------------------
------------------------------------------------------------------------------'''
window.mainloop()