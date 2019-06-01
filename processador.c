#include <stdio.h>
#include <string.h>
#use rs232(uart1, baud = 9600)

//#include "estrutura.c"
//#include "processador.c"

#define x pin_d0 //COORD X
#define y pin_d1 //COORD Y
#define z pin_d2 //COORD Z
#define start pin_a1 //BUTTON START
#define stop pin_a2 //BUTTON STOP
#define ventosa pin_d5 //AC VENTOSA
#define sa_x pin_a4 //SENSOR AVANCADO X
#define sr_x pin_a3 //SENSOR RETORNADO X
#define sa_y pin_e0 //SENSOR AVANCADO Y
#define sr_y pin_a5 //SENSOR RETORNADO Y
#define sa_z pin_e2 //SENSOR AVANCADO Z
#define sr_z pin_e1 //SENSOR RETORNADO Z
#define buzzer pin_c1 //BUZZER

void set_ventosa();
void get_ventosa();

void process(char get_c, char set_c);
void process_all(char);

 int set(int,int);
 int get(int,int);
void reset();
/*int etapa_START;
int etapa_STOP;
int a;
int b;
int c;
int d;  //##CODIGO ANTIGO

void inicial();
void reset();*/
//##BUZZERS
 int b_V(int);//BUZZER 20MS
 int b_Q(int);//BUZZER 40MS
 int b_C(int);//BUZZER 100MS
 void b_S(int s);//BUZZER 1000MS / 1 S
/*##ACOES
void get_xr_ya(); // PEGA X RETORNADO Y AVANÇADO
void get_xr_yr(); // PEGA X RETORNADO Y RETORNADO
void get_xa_yr(); // PEGA X AVANCADO Y RETORNADO
void get_xa_ya(); // PEGA X AVANCADO Y AVANCADO
//##set
void set_xr_ya(); // SETA X RETORNADO Y AVANCADO
void set_xr_yr(); // SETA X RETORNADO Y RETORNADO
void set_xa_yr(); // SETA X AVANCADO Y RETORNADO
void set_xa_ya(); // SETA X AVANCADO Y AVANCADO*/
   int i;
 int gx;
 int gy;
 int sx;
 int sy;
/*
#
###~~MESA PNEUMATICA POS;
#
#     XR_YR    XA_YR
#       A        B
#     
#     XR_YA    XA_YA
#       C        D
#
#EXEMPLO; A+B ~ C+D ## B A 
#                      D C
*/

/*

   A = 0 | 0
   B = 1 | 0
   C = 0 | 1
   D = 1 | 1
   
   D+A|C+D|B+C|D+B
   01234567891011121314
   
*/

void process_all(int param[]) {
//1234567891011121314
   int g =0;
   int s =0;
   int g_x;
   int g_y;
   int s_x;
   int s_y;
   for(g=0; g < 12; g+4) {  // 0 4 8 12 
      for(s=2; s < 14; s+4) { // 2 6 10 14
         if(param[g]!=2 || param[s]!=2) {
         g_x = param[g];
         g_y = param[g+1];
         s_x = param[s];
         s_y = param[s+1];
         get(g_x, g_y);
         set(s_x, s_y);
         }
      }      
   }
   b_S(1);
}


void process(char get_c, char set_c) {
    switch(get_c) {
      case "a":
      get(0,0);
      case "b":
      get(1,0);
      case "c":
      get(0,1);
      case "d":
      get(1,1);
    }
    switch(set_c) {
      case "a":
      set(0,0);
      case "b":
      set(1,0);
      case "c":
      set(0,1);
      case "d":
      set(1,1);
    }
}


void get_ventosa(){
   b_V(1);
   output_high(z);
   while(input(sa_z)==0);
   delay_ms(200);
   b_Q(2);
   output_high(ventosa);
   delay_ms(500);
   b_V(1);
   output_low(z);
   while(input(sr_z)==0);
   delay_ms(500);
   reset();
}

void set_ventosa(){   
   b_V(1);
   output_high(z);
   while(input(sa_z)==0);
   delay_ms(200);
   b_Q(2);
   output_low(ventosa);
   delay_ms(200);
   b_V(1);
   output_low(z);
   while(input(sr_z)==0);
   delay_ms(500);
   reset();
}

int set(int s_x, int s_y){
    int sx = s_x;
    int sy = s_y;
   if(sx==1) {//CHECA SE X DEVE SER AVANÇADO
   b_V(1);
      output_high(x);//AVANÇA X
      while(input(sa_x)==0);
      delay_ms(300);
      if(sy==1) {//CHECA SE Y DEVE SER AVANÇADO
         b_V(1);
         output_high(y);//AVANÇA Y
         while(input(sa_y)==0);
         delay_ms(300);
         set_ventosa();//SETA BLOCO
          sx = 2;
          sy = 2;
      }else if(sy==0) {//CHECA SE Y DEVE SER RETORNADO
         delay_ms(300);
         set_ventosa();//SETA BLOCO
          sx = 2;
          sy = 2;
      }
   }else if(sx==0) {//CHECA SE X DEVE SER RETORNADO
      if(sy==1) {//CHECA SE Y DEVE SER AVANÇADO
      b_V(1);
         output_high(y);//AVANÇA Y
         while(input(sa_y)==0);
         delay_ms(300);
         set_ventosa();//SETA BLOCO
          sx = 2;
          sy = 2;
      }else if(sy==0) {//CHECA SE Y DEVE SER RETORNADO
         delay_ms(300);
         set_ventosa();//SETA BLOCO
          sx = 2;
          sy = 2;
      }
   }
   return 0;
}

int get(int c_x, int c_y){
    int gx = c_x;
    int gy = c_y;
   if(gx == 1) {//CHECA SE X DEVE SER AVANÇADO 
   b_V(1);
      output_high(x);//AVANÇA X
      delay_ms(300);
      while(input(sa_x)==0);
         if(gy==1) {//CHECA SE Y DEVE SER AVANÇADO
         b_V(1);
          output_high(y);//AVANÇA Y
          delay_ms(300);
          while(input(sa_y)==0);
          delay_ms(300);
          get_ventosa();//PEGA O BLOCO
           gx = 2;
          gy = 2;
      }else if(gy==0) {//CHECA SE Y DEVE SER RETORNADO
         delay_ms(300);
          get_ventosa();//PEGA O BLOCO
           gx = 2;
          gy = 2;
      }
   }else if(gx==0){//CHECA SE X DEVE SER RETORNADO
      if(gy==1) {//CHECA SE Y DEVE SER AVANÇADO
      b_V(1);
         output_high(y);//AVANÇA Y
         delay_ms(300);
         while(input(sa_y)==0);
         delay_ms(300);
         get_ventosa();//PEGA O BLOCO
          gx = 2;
          gy = 2;
      }else if(gy==0){//CHECA SE Y DEVE SER RETORNADO
         delay_ms(300);
         get_ventosa();//PEGA O BLOCO
          gx = 2;
          gy = 2;
      }
   }
   return 0;
}

void reset() {
   delay_ms(300);
   if(input(sa_x)==1) {
   b_V(1);
      output_low(x);
      delay_ms(500);
   }
   if(input(sa_y)==1) {
      b_V(1);
      output_low(y);
      delay_ms(500);
   }
}

int b_V(int v){// parametro q retorna quantidade de vezes executa o codigo
   int i = 0;
   for(i = 0; i < v; v--) {
      output_high(buzzer);
      delay_ms(20);
      output_low(buzzer);
   }
   return 0;
}

int b_Q(int q){
   int i = 0;
   for(i = 0; i < q; q--) {
   output_high(buzzer);
   delay_ms(40);
   output_low(buzzer);
   }
   return 0;
}

int b_C(int c){
   int i = 0;
   for(i = 0; i < c; c--) {
   output_high(buzzer);
   delay_ms(100);
   output_low(buzzer);
   }
   return 0;
}

void b_S(int s){
   int i = 0;
   for(i = 0; i < s; s--) {
   output_high(buzzer);
   delay_ms(1000);
   output_low(buzzer);
   }
}
