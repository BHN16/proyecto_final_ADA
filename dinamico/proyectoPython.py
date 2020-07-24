from tkinter import *
from PIL import Image
import numpy as np
import subprocess
from pathlib import Path
import os


class IMG:
    def __init__(self):
        self.link = None
        self.im = None
        self.pixels = None
        self.xsize = None
        self.ysize = None
        self.cmd = None
        self.pathInicio = None
        self.fileInicio = None
        self.llenar = None
        



def CrearMatricesIniciales():

    link1 = input("escriba el nombre de la imagen1 ")
    link2 = input("escriba el nombre de la imagen2 ")

    im = Image.open(Path(link1))
    im2 = Image.open(Path(link2))
    
    pixels = im.load()
    pixels2 = im2.load()
    

    xsize, ysize = im.size
    xsize2, ysize2 = im2.size

    

    cmd1 = "touch matrizDibujo1.txt"
    cmd2 = "touch matrizDibujo2.txt"

    if(subprocess.call(cmd1, shell=True)=="0"):
        print("carcasa de matriz 1 creada")
    if(subprocess.call(cmd2, shell=True)=="0"):
        print("carcasa de matriz 2 creada")


    pathInicio = "matrizDibujo1.txt"
    pathFinal = "matrizDibujo2.txt"

    fileInicio = open(pathInicio,'w')
    fileFinal = open(pathFinal,'w')


    for i in range(xsize):
        llenar = ""
        llenar2 = ""
        for j in range(ysize):
            RGB = pixels[i,j]
            RGB2 = pixels2[i,j]
            temp_val = int(RGB[0]*0.299 + RGB[1]*0.587 + RGB[2]*0.114)
            temp_val2 = int(RGB2[0]*0.299 + RGB2[1]*0.587 + RGB2[2]*0.114)
            if(temp_val > 127):
                llenar = llenar + "1 "
                pixels[i,j] = (255,255,255)
            else:
                llenar = llenar + "0 "
                pixels[i,j] = (0,0,0)

            if(temp_val2 > 127):
                llenar2 = llenar2 + "1 "
                pixels2[i,j] = (255,255,255)
            else:
                llenar2 = llenar2 + "0 "
                pixels2[i,j] = (0,0,0)


        llenar=llenar+"\n"
        llenar2 = llenar2 + "\n"
        fileInicio.write(llenar) 
        fileFinal.write(llenar2)

    fileInicio.close()
    fileFinal.close()
     
    im.save("B_W"+link1)
    im2.save("B_W"+link2)


def LeerMatrices():
     
    

    pathFinal = "matrizDibujo2.txt"

    path, dirs, files = next(os.walk("/home/lusho/Documents/Eduardo/5tociclo/ADA/pruebaADA/proyecto_final_ADA/dinamico/TEST"))
    file_count = len(files)
    print(files)
    print(dirs)
    devolver = []

    for f in files:
        nuevo = f.split("_")
        nuevo[0] = int(nuevo[0])
        devolver.append({'orden' : int(nuevo[0]), 'name' : f})

    devolver.sort(key=lambda x: x.get('orden'))
    
    yuriInicio = Image.open("B_Wyurisonriendo.jpg") 
    yuriInicio.show()    

    lista = [None]*len(files)
    for curf in devolver:
        w=0;
        with open(path+"/"+curf['name']) as fp:
            print("VECES")
            lista[w] = Image.open("yuriDurmiendo.jpeg");
            pixels3 = lista[w].load()
            xsize3, ysize3 = lista[w].size
            line = fp.readline()
            i=0
            while line:
                line = fp.readline()
                nuevo = line.split(' ')
                for j in range(0,len(nuevo)-1):
                    if(nuevo[j] == '0'):
                        pixels3[i,j] = (0,0,0)
                    else:
                        pixels3[i,j] = (255,255,255)
                i=i+1
            fp.close() 
            lista[w].show()
            w+=1
    yuriFinal = Image.open("B_WyuriDurmiendo.jpeg")
    yuriFinal.show()

print("choose your case \n")
print("Crear las matrices digite 1\n")
print("Generar las imagenes diginte 2\n")
caso = input()

if(caso == "1"):
    CrearMatricesIniciales()
elif(caso == "2"):
    LeerMatrices()


