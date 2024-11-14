## Link del Repositorio 

https://github.com/junx21/Gestion_de_la_memoria.git


## Explicación paso a paso:

1. Importamos las bibliotecas necesarias para la asignación de memoria y la creación de procesos.

2. Creamos una constante `SIZE` para determinar el tamaño de la región de memoria compartida que queremos asignar.

3. En la función principal, llamamos a la función `mmap()` para asignar una región de memoria que pueda ser compartida entre procesos. Los argumentos de esta función son los siguientes:
   - `NULL`: Le estamos pidiendo al sistema operativo que elija la dirección de inicio de la región de memoria.
   - `SIZE`: Es el tamaño de la región que queremos asignar.
   - `PROT_READ|PROT_WRITE`: Estamos indicando que la región de memoria puede ser leída y escrita.
   - `MAP_SHARED|MAP_ANONYMOUS`: Estas banderas indican que la región de memoria será compartida entre procesos y que no estará respaldada por ningún archivo.
   - `-1`: Este argumento sería el descriptor de archivo si la memoria estuviera respaldada por un archivo, pero como usamos `MAP_ANONYMOUS`, podemos poner -1.
   - `0`: Este sería el desplazamiento si la memoria estuviera respaldada por un archivo. Como no es el caso, ponemos 0.

4. Comprobamos si la asignación fue exitosa. Si `mmap()` devuelve `MAP_FAILED`, significa que hubo un error.

5. Creamos un nuevo proceso utilizando la función `fork()`. Esta función devuelve el PID del proceso hijo al proceso padre y 0 al proceso hijo.

6. Luego, comprobamos el valor devuelto por `fork()`. Si es menor que 0, hubo un error al crear el nuevo proceso.

7. Si el valor devuelto es 0, significa que estamos en el proceso hijo. En este caso, leemos y mostramos la memoria compartida y luego liberamos la memoria utilizando la función `munmap()`.

8. Si el valor devuelto por `fork()` es mayor que 0, significa que estamos en el proceso padre. En este caso, escribimos en la memoria compartida, esperamos a que el proceso hijo termine y luego liberamos la memoria.
