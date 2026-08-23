# Reporte de los resultados obtenidos

## Recursos utilizados

En este laboratorio se utilizaron los *intrinsics* consultados en [Intel](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=mul_ps&ig_expand=4687):

- `_mm256_mul_ps`
- `_mm256_extractf128_ps`
- `_mm_add_ps`
- `_mm_hadd_ps`
- `_mm_cvtss_f32`
- `_mm256_loadu_ps`

Para completar las funciones:

- `simd_mul_ps()`
- `simd_reduce_add_ps()`
- `dot_product_avx2()`

## Instrucciones de ejecución

Para correr los programas de multiplicación se utilizaron las siguientes instrucciones:

```bash
# 1. Compilar antes de ejecutar
make

# 2. Ejecutar multiplicación escalar
./matmul_scalar 1

# 3. Ejecutar multiplicación vectorial (AVX2)
./matmul_avx2 1
```

## Resultados obtenidos

| Métrica | Multiplicación escalar | Multiplicación vectorial |
|---|---|---|
| Tamaño matriz | 2048x2048 | 2048x2048 |
| Repeticiones | 1 | 1 |
| Checksum | 86972906452.000000 | 86972906452.000000 |
| C[0][0] | 26800.500000 | 26800.500000 |
| C[1023][1023] | 26836.500000 | 26836.500000 |
| Operaciones | 17179869184 | 17179869184 |
| Tiempo | 9.666072 segundos | 2.848346 segundos |
| Rendimiento | 1.777337 GFLOP/s | 6.031525 GFLOP/s |



## Calculo del Speedup y la eficiencia

De acuerdo con la Ley de Amdahl:

1.  __Speedup (S)__

Donde: 
- T₁ = tiempo escalar 
- Tₚ = tiempo vectorial:

$$S = \frac{T_1}{T_p} = \frac{9.666072}{2.848346} \approx 3.394$$

2. __Eficiencia (η)__

Se utiliza n = 8, el cual corresponde a la constante `AVX_FLOATS` del código, que representa la cantidad de floats de 32 bits que caben en un registro `__m256` de 256 bits.

Calculando la eficiencia:
$$\eta = \frac{S}{n} = \frac{3.394}{8} \approx 0.424$$

Esto equivale a una eficiencia del 42.4%.

## Referencia

Las ecuaciones y conceptos utilizados en este análisis fueron consultados en el 
material de clases del curso *Computación Heterogénea*, impartido por el 
profesor Luis G. León-Vega, PhD.:

**Fuente:** León-Vega, L. G. (2026). *Fundamentos de arquitecturas de CPU* 
(Capítulo 1, pp. 41-42) [Material de clase](https://github.com/kdsalazar95/Labs_CH/blob/feature/simd/Semana3_simd/Documentacion/Ley_Amdahl.pdf). Curso Computación Heterogénea, 
Instituto Tecnológico de Costa Rica.
