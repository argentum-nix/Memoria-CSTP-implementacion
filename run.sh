#!/bin/bash

# Valores de a, b y c
a=(8)
b=(102 9 23 345 8919 10 34 29 11 784 379 9 999 78 23 4576 98 32 66 666 1000)
c=(0.9)

# Archivo para guardar la salida
output_file="output.txt"

# Ejecutar el comando y guardar los valores en el archivo de salida
for a_val in "${a[@]}"; do
  for c_val in "${c[@]}"; do
    for b_val in "${b[@]}"; do
      echo "Ejecutando con a=$a_val, b=$b_val, c=$c_val..."
      output=$(./CSTPSolver ./instances/ 1 network 1 1 "$a_val" "$b_val" "$c_val" C)
      echo "$output"
      echo "$output" >> "$output_file"
      echo "Finalizado."
    done
  done
done
