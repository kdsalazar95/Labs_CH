## Instrucciones para graficar

### Instalar

```bash
# Opvion 1
sudo apt install gnuplot-qt

# Opcion 2
sudo apt install gnuplot-nox

# Verificar
gnuplot --version
```
*__Nota:__ Para este laboratorio se instalo la opcion 1.*

## Graficar

```bash
# Compilar
gcc -Wall -Wextra -O2 plot_tiempos.c -o plot_tiempos

# Ejecutar
./plot_tiempos threading
./plot_tiempos scaling
```

Se generara una imagen en formato .png del grafico en este mismo directorio.