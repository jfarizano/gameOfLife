import os
import sys
import subprocess
import time
from termcolor import colored, cprint

def main():
  with open("entrada.txt", 'r') as file:
    datos = file.readline()
  
  times = int(input("Cuantas veces? "))

  for i in range(times):
    os.system("./main.o entrada.txt salida.txt")
    inputFile = open("salida.txt", "r")
    outputFile = open("entrada.txt", "w")
    outputFile.write(datos)        

    os.system("clear")
    for line in inputFile.readlines():
      outputFile.write(line)
      strLine = ""
      for j in range(len(line)):
        if line[j] == 'X':
          strLine += colored(line[j], "grey") + " "
        elif line[j] == 'O':
          strLine += colored(line[j], "yellow") + " "
      print(strLine)
    print(i)

    outputFile.close()
    inputFile.close()
    time.sleep(0.05)

main()