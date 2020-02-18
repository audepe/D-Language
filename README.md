---
noteId: "980e35a0525411ea9e7ffbecb2872bd0"
tags: []

---

# Declaración del lenguaje D
Documento en el que se establecen las bases del lenguaje a desarrollar durante el curso de la asignatura Procesadores del Lenguaje.

## Tipos de variables
* **num**:  Número entero.
* **num_float**: Número float.
* **bool**: boolean con valor *TRUE* o *FALSE*.
* **char**: Caracter único


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
Todas las funciones deben estar declaradas y especificadas antes de su uso.s
  *Ejemplo:*
  ```
  function {tipo de resultado} {nombre} ({parámetros}){
      Código
  }
  ```
  
## Programa de ejemplo _Fibonacci_

