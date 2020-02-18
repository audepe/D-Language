---
noteId: "980e35a0525411ea9e7ffbecb2872bd0"
tags: []

---

# Declaración del lenguaje D
Documento en el que se establecen las bases del lenguaje a desarrollar durante el curso de la asignatura Procesadores del Lenguaje.

---
## Variables
El lenguaje permitirá el uso tanto de variables locales como globales. 
Las variables globales deben crearse al principio del código.
No se permitirá la asignación a variables de tipos de datos distintos al de la variable, ni siquiera en los numéricos,. 
* Declaraciones de variables numéricas: **{tipo} {nombre};**   
  *Ejemplo: num x;*
* Asignación de variables: **{nombre} := {valor};**  
  *Ejemplo: x := 1;*

### Tipos de variables

* **num**:  Número entero.
* **num_float**: Número float.
* **bool**: boolean con valor *TRUE* o *FALSE*.
* **char**: Caracter único
* **string**: Ristra de caracteres.


---
## Operaciones aritméticas
Solo se permitiran operaciones aritméticas entre variables o inmediatos del mismo tipo. 
Toda operación debe acabar en punto y coma.


| Símbolo | Descipción | Ejemplo |
| ------ |---------| ------|
| ':=' | Asignación | a := 2; |
| '+' | Suma | a := a + 2; |
| '-' | Resta | a := a - 2; |
| '*' | Multiplicación | a := a * 2; |
| '/' | División | a := a/1; |
| '%' | Resto  | a := a % 1; |
| '=' | Comparación igual que | a = 1; |
| '!=' | Comparación distinto que  | a != 1; |
| ':+=' | Suma y asignación | a :+= 1; |
| ':-=' | Resta y asignación | a :-= 1; |
| ':*=' | Multiplicación y asignación | a :*= 1; |
| ':*=' | División y asignación | a :*= 1; |
| ':%=' | Resto y asignación  | a :%= 1; |

---
## Funciones 
Todas las funciones deben estar declaradas y especificadas antes de su uso.
Las funciones deben ser declaradas con la siguiente estructura:

  *Ejemplo:*
  ```
  function {tipo de resultado} {nombre} ({parámetros}){
      Código
  }
  ```
En donde el tipo de resultado es el tipo de variable que va a retornar. En caso de no retornar ningún valor se debe escribir void.

El código debe incluir una función de nombre *main()* que será la primera función en ejecutarse.

---

## Operaciones de entrada y salida:

  Las operaciones de salida *print* deben incluir entre paréntesis la variable o el inmediato a imprimir. 
  ```
    print({variable});
    print({"ristra"});
  ```
  Las operaciones de entrada deben incluir entre paréntesis la variable tipo *string* que almacenará el resultado.
  ```
  input({variable donde se almacena});
  ```

  ---
## Estructuras de control

* for  
  *Ejemplo:*
  ```
    for ({asignación}; {condición}; {actualización}) {
        Código
    }
  ```
  while  
  *Ejemplo:*
  ```
    while ({condición}) {
        Código
    }
  ```
  if  
  *Ejemplo:*
  ```
    if ({condición}) {
        Código
    } else {
        Código
    }
  ```


  

