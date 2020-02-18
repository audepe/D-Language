# Declaración del lenguaje D
Documento en el que se establecen las bases del lenguaje a desarrollar durante el curso de la asignatura Procesadores del Lenguaje.

## Descripción
* Declaraciones de variables numéricas: {tipo} {nombre}; 
  *Ejemplo: num x;*
* Asignación de variables: {nombre} := {valor}; 
  *Ejemplo: x := 1;*
* Estructuras de control: for,while,if,else.
  *Ejemplo:*
  ```
    for ({asignación}; {condición}; {actualización}) {
        Código
    }
  ```
  ```
    while ({condición}) {
        Código
    }
  ```
  ```
    if ({condición}) {
        Código
    } else {
        Código
    }
  ```
* Operaciones de entrada y salida: 
  *Ejemplo:*
  ```
    print({variable});
    print({"ristra"});
  ```
  ```
  input({variable donde se almacena});
  ```
* Declaración de funciones:
  *Ejemplo:*
  ```
  function {tipo de resultado} {nombre} ({parámetros}){
      Código
  }
  ```
  
## Programa de ejemplo _Fibonacci_

