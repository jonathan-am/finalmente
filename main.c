#include <main.h>
#include <processador.c>

void main(){  
   while(true) {
/*
#
##EXEMPLO DE IDENTIFICAÇÃO
#
#USE 0 1 PARA IDENTIFICAR, 2 PARA NEGAR O CHAR
parametro;  11,00|01,11|10,01|11,10
tradução;    d+a   c+d   b+c   d+b
bancada;          A B   |   D C
                  C D   |   B A
#                  
#    TODO: CODIGO;           
*/
      if(input(start)==1) {         
         int arr[16] = {1,1,0,0,0,1,1,1,1,0,0,1,1,1,1,0};
         process_all(arr);
      }
      if(input(stop)==1) {
         int arr[16] = {0,0,1,1,0,1,0,0,2,2,2,2,2,2,2,2};
         process_all(arr);
      }
   }
}
