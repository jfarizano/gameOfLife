import os
import sys
import time
from termcolor import colored, cprint

def main():
  tablero = []

  with open("entrada.txt", 'r') as file:
    datos = file.readline()
  
  times = int(input("Cuantas veces? "))

  for i in range(times):
    os.system("./main.o")
    outputFile = open("entrada.txt", "w")
    outputFile.write(datos)        
    inputFile = open("salida.txt")

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

    time.sleep(0.1)

    if i < times-1:
      os.system("clear")

    outputFile.close()
    inputFile.close()

main()