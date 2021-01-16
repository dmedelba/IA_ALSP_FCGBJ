# IA: ALSP resuelto con FC + GBJ en C++

## Instrucciones para ejecutar:

- Ejecutar comando *make*, se debe estar dentro de la ruta del proyecto.
- Ejecutar *make run*
- El programa comenzará solicitando la instancia a evaluar, se debe ingresar su nombre sin su extensión.
    - Ejemplo: airland1
- El algoritmo realizará la búsqueda de la solución óptima. 

- Se puede detener la ejecución del algoritmo con **CTRL +  C**, dicha interrupción imprimirá por pantalla la mejor solución (con su costo) encontrada hasta ese momento, la cantidad de instanciaciones, retornos y chequeos.

    - Dicha información también se puede observar almacenada en el archivo **solucion_NOMBREINSTANCIA.txt** dentro de la carpeta del proyecto.
- Una vez finalizado el algoritmo entregará por pantalla la mejor solución con su correspondiente asignación de tiempo y costo.

- Para volver a ejecutar otra instancia o la misma, se recomienda realizar el comando *make clean*, el cual eliminará la compilación anterior y los archivos .txt creados de otras ejecuciones.

````
Importante: 
- Todas las instancias a probar deben estar dentro de la carpeta /Instancias 
- Proyecto desarrollado en macOS, y probado en macOS y Linux.
- Dentro de la carpeta /Instancias existe una instancia pequeñallamada airland_test.txt, la cual se ejecuta de manera rápida, demostrando el correcto funcionamiento de FC+GBJ.
````
