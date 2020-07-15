# Batería de pruebas
## Estructuras de control
### Bucle - while
- [x] Funciona
```d
function void ejemplo_while_1(){
    num a;
    a := 1;
    while (a <= 2) {
        print(a);
        a :+= 1;        
    }
}

function void ejemplo_while_2(){
    bool b;
    b := TRUE;
    while (b){
        print('b');
        b := FALSE;
    }
}
```
- [ ] No funciona
```d
function void ejemplo_while_3(){
    while (a){
        print(a);
        a :+= 1;  
    }
}

function void ejemplo_while_4(){
    while (b == FALSE){
        print(a);
    }
}
```
### Bucle - for
- [x] Funciona  
```d
function void ejemplo_for_1(){
    for (num i := 0; i < 5; i := i+1) {
        print(i);
    }
}

function void ejemplo_for_2(){
  num i;
    for (i := 0;  i < 5; i := i+1) {
        print(i);
    }
}
``` 
- [ ] No funciona
```d 
function void ejemplo_for_3(){
    num i := 0;
    for (i; i < 5; i = i+1) {
        print(i);
    }
}

function void ejemplo_for_4(){
    for (num i = 0; i + 5; i := i+1) {
        print(i);
    }
}
```
### Condicional - if
- [x] Funciona
```d
function void ejemplo_if_1(){
    num a := 0;
    if (a == 0){
        print("Nice");
    }
}
```
- [ ] No funciona
```d
function void ejemplo_if_2(){
    num a := 0;
    if (a = 0){
        print("Nice");
    }
}
``` 
