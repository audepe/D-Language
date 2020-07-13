char c;

function void error(){
    //puts
    endops();
}

function num expression(num aux){
    num resultado;
    char op := '+';
    if ((c == '+') || (c == '-')) {
        op := c;
        
    }
    resultado := termino();
    if (op == '-') {
        resultado := neg(resultado);
    }
    while ((c == '+') || (c == '-')){
        op := c;
        
        aux := termino();
        if(op == '+'){
            resultado := (aux + resultado);
            //break;
        } else {
            if(op == '-') {
                resultado :-= aux;
                //break;
            }
        }
    }
    return resultado;
}

function num factor(){
    num resultado := 0;
    if (c == '('){
        resultado := expression(resultado);
        if (c == ')'){
            resultado := -resultado;    
        } else { 
            error();
        }
        return resultado;   
    }else{
        return numero();
    }
}

function num termino(){
    num resultado;
    num aux;
    char op;
    resultado := factor();
    while ((c == '*') || (c == '/')){
        op := c;
        
        aux := factor(); 
        if(op == '*'){
            resultado :*= aux;
            //break;
        } else {
            if(op == '/') {
                resultado :/= aux;
                //break;
            }
        }
    }
    return resultado;

}
function void main(){    
    num resultado;
    
    while(c != '\n' ){
        resultado := expression(resultado);
        if(c == '\n'){
            print(resultado);
            
        } else {
            error();
        }
    }
}
