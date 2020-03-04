# Batería de pruebas






## Estructuras de control
### while
```
//Funciona
num a;
a := 1;
while (a <= 2) {
    print(a);
    a :+= 1;        
}

//Funciona
bool b;
b := TRUE;
while (b){
    print('b');
    b := FALSE;
}

//No funciona
while (a){
    print(a);
    a :+= 1;  
}

//No funciona
while (b == FALSE){
    print(a);
}

```
### for
```
  // Funciona  
  num i;
  for (i := 0; i < 5; i := i+1) {
      print(i);
  }
  
  // No funciona
  for (num i := 0 ;  i < 5; i := i+1) {
      print(i);
  }
  
  // No funciona
  num i;
  for (i = 0; i < 5; i := i+1) {
      print(i);
  }
  
  // No funciona
  num i;
  for (i = 0; i + 5; i := i+1) {
      print(i);
  }
```
### if
```
// Funciona
num a := 0;
if (a = 0){
    print("Nice");
}

// No funciona
num a := 0;
if (a == 0){
    print("Nice");
}


``` 
