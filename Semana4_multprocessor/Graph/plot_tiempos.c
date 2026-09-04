/*
 * plot_tiempos.c
 *
 * Genera un grafico de "numero de hilos vs. tiempo real" a partir de un
 * archivo de datos, usando gnuplot.
 *
 * Este programa puede graficar dos conjuntos de datos distintos:
 *
 *   threading -> lee "tiempos_threading.txt"
 *                columnas: hilos, cpu-affinity, cpu-naive
 *                salida:   tiempos_threading.png
 *
 *   scaling   -> lee "tiempos_scaling.txt"
 *                columnas: hilos, matmul_tiled_openmp, softmax_openmp
 *                salida:   tiempos_scaling.png
 *
 * Requiere tener gnuplot instalado:
 *   sudo apt install gnuplot
 *
 * Compilar:
 *   gcc -Wall -Wextra -O2 plot_tiempos.c -o plot_tiempos
 *
 * Ejecutar:
 *   ./plot_tiempos threading
 *   ./plot_tiempos scaling
 *
 * Si no se pasa argumento, el programa pregunta interactivamente.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *nombre;       /* identificador ("threading" / "scaling") */
    const char *archivo_dat;  /* archivo .txt de entrada */
    const char *archivo_png;  /* archivo .png de salida */
    const char *titulo;       /* titulo del grafico */
    const char *serie1;       /* nombre de la serie de la columna 2 */
    const char *serie2;       /* nombre de la serie de la columna 3 */
} dataset_t;

static const dataset_t DATASETS[] = {
    {
        "threading",
        "tiempos_threading.txt",
        "tiempos_threading.png",
        "Numero de hilos vs. tiempo real (threading)",
        "cpu-affinity",
        "cpu-naive"
    },
    {
        "scaling",
        "tiempos_scaling.txt",
        "tiempos_scaling.png",
        "Numero de hilos vs. tiempo real (scaling)",
        "matmul_tiled_openmp",
        "softmax_openmp"
    }
};

#define NUM_DATASETS (sizeof(DATASETS) / sizeof(DATASETS[0]))

static void mostrar_opciones(void) {
    printf("Conjuntos de datos disponibles:\n");
    for (size_t i = 0; i < NUM_DATASETS; i++) {
        printf("  %zu) %s  (%s)\n", i + 1, DATASETS[i].nombre, DATASETS[i].archivo_dat);
    }
}

static const dataset_t *buscar_dataset(const char *nombre) {
    for (size_t i = 0; i < NUM_DATASETS; i++) {
        if (strcmp(DATASETS[i].nombre, nombre) == 0) {
            return &DATASETS[i];
        }
    }
    return NULL;
}

static void generar_grafico(const dataset_t *ds) {
    FILE *data = fopen(ds->archivo_dat, "r");
    if (data == NULL) {
        fprintf(stderr, "Error: no se encontro '%s' en el directorio actual\n",
                ds->archivo_dat);
        exit(EXIT_FAILURE);
    }
    fclose(data);

    FILE *gp = popen("gnuplot", "w");
    if (gp == NULL) {
        fprintf(stderr, "Error: no se pudo abrir gnuplot. "
                        "Instalalo con: sudo apt install gnuplot\n");
        exit(EXIT_FAILURE);
    }

    fprintf(gp, "set terminal pngcairo size 900,600 noenhanced font 'Arial,11'\n");
    fprintf(gp, "set output '%s'\n", ds->archivo_png);
    fprintf(gp, "set title '%s'\n", ds->titulo);
    fprintf(gp, "set xlabel 'Numero de hilos'\n");
    fprintf(gp, "set ylabel 'Tiempo real (s)'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set key top left\n");
    fprintf(gp, "set xtics 1\n");
    fprintf(gp, "plot '%s' using 1:2 with linespoints title '%s' lw 2 pt 7, "
                "'%s' using 1:3 with linespoints title '%s' lw 2 pt 7\n",
                ds->archivo_dat, ds->serie1,
                ds->archivo_dat, ds->serie2);

    pclose(gp);

    printf("Grafico generado: %s\n", ds->archivo_png);
}

int main(int argc, char **argv) {
    const dataset_t *ds = NULL;

    if (argc > 1) {
        ds = buscar_dataset(argv[1]);
        if (ds == NULL) {
            fprintf(stderr, "Opcion invalida: '%s'\n\n", argv[1]);
            mostrar_opciones();
            return EXIT_FAILURE;
        }
    } else {
        char entrada[64];

        mostrar_opciones();
        printf("Ingrese el nombre del conjunto a graficar (threading/scaling): ");

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) {
            fprintf(stderr, "Entrada invalida\n");
            return EXIT_FAILURE;
        }

        entrada[strcspn(entrada, "\n")] = '\0';

        ds = buscar_dataset(entrada);
        if (ds == NULL) {
            fprintf(stderr, "Opcion invalida: '%s'\n", entrada);
            return EXIT_FAILURE;
        }
    }

    generar_grafico(ds);

    return EXIT_SUCCESS;
}