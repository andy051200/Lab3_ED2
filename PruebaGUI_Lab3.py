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
from tkinter import *

'''------------------------------------------------------------------------------
-----------------------DEFINICION DE FUNCIONES-----------------------------------
------------------------------------------------------------------------------'''
class Window(Frame):
    def __init__(self, master=None):
        Frame.__init__(self,master)
        self.master=master
        self.init__window()
    
    def init__window(self):                                     #se define la clase
        self.master.title("GUI Lab3, Electronica Digital 2")    #se nombra ventana
        self.pack(fill=BOTH, expand=1)                          #parametros a llenar
        conectar= Button(self, text="Conectar", command=self.salida)                   #nombre del boton
        conectar.place(x=100, y=10)                            #nombre del boton

    #en esta parte se definen las acciones del boton
    def salida(self):
        exit()
        

'''------------------------------------------------------------------------------
-------------------------IMPORTAR LIBRERIAS--------------------------------------
------------------------------------------------------------------------------'''

root=Tk()
root.geometry("400x300")                                        #dimensiones de ventana
app=Window(root)            
root.mainloop()                                                 #se enloopa la funcion