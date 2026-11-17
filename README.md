# Range Tree 2D, Layered Range Tree e Interval Tree en C++

## 📂 Estructura
- `apps/app_main.cpp` → menú principal (datos predefinidos, consultas y comparativas)
- `include/comun.h` → `Punto`, `Intervalo`, `Metricas` y búsqueda binaria
- `include/range_tree.h` / `src/range_tree.cpp` → Range Tree 2D
- `include/layered_range_tree.h` / `src/layered_range_tree.cpp` → Layered Range Tree (fractional cascading)
- `include/interval_tree.h` / `src/interval_tree.cpp` → Interval Tree (búsqueda de un query point)
- `bin/` → ejecutable, `obj/` → archivos objeto, `Makefile` → compilación

## 📊 Complejidades
| Estructura | Construcción | Espacio | Consulta |
|---|---|---|---|
| Range Tree 2D | O(n log n) | O(n log n) | O(log² n + k) |
| Layered Range Tree | O(n log n) | O(n log n) | O(log n + k) |
| Interval Tree | O(n log n) | O(n) | O(log n + k) |

(k = cantidad de resultados)

## 🚀 Compilar y ejecutar
```bash
make
./bin/app_trees
```
