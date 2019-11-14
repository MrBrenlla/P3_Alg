#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define MAX_ITER 32001
#define MIN_ITER 250
#define K 1000
#define TAM 256000

typedef struct {
  int datos[TAM];
  int nodos;
} monticulo;


void crearMonticulo(int v[], int n, monticulo *m);
int consultarMenor(const monticulo *m);
void quitarMenor(monticulo *m);
void afundir(monticulo * m, int n);
void inicializar_semilla();
void aleatorio(int v [], int n);
void ascendente(int v [], int n);
void descendente(int v [], int n);
void tempos(int n, void (*ord)(int v [], int n),void (*ini)(int v[], int n),
double cotainf, double cotaax, double cotasup);
void ordenar();

/*
--------------------------------------------------------------------------------
*/
void afundir(monticulo * m, int n){
  int pos, min, aux;
  pos=n*2;
  if(pos+1<m->nodos){
    if (m->datos[pos+1]<m->datos[pos]) ++pos;
     min=m->datos[pos];
  }
  if(pos<m->nodos){
    if(min<m->datos[n]){
      aux=m->datos[n];
      m->datos[n]=m->datos[pos];
      m->datos[pos]=aux;
      afundir(m,pos);
    }
  }
}
/*
--------------------------------------------------------------------------------
*/
void crearMonticulo(int v[], int n, monticulo *m){
  int i;
  m->nodos=n;
  for (i=0; i<n;i++) m->datos[i]=v[i];
  for(i = (n-1)/2; i>-1; i--){
    afundir(m,i);
  }
}
/*
--------------------------------------------------------------------------------
*/
int consultarMenor(const monticulo *m){
  return m->datos[0];
}
/*
--------------------------------------------------------------------------------
*/
void quitarMenor(monticulo *m){
  int aux;
  aux=m->datos[0];
  m->nodos-=1;
  m->datos[0]=m->datos[m->nodos];
  m->datos[m->nodos]=aux;
  afundir(m,0);
}
/*
--------------------------------------------------------------------------------
*/
void ord_mont(int v [], int n){
  int i;
  monticulo * m = malloc(sizeof(monticulo));
  crearMonticulo (v,n, m);
  for(i=0 ; i<n ; i++){
    v[i] = consultarMenor(m);
    quitarMenor(m);
  }
  free(m);
}
/*
--------------------------------------------------------------------------------
*/
void creador(int v[], int n){
  monticulo * m = malloc(sizeof(monticulo));
  crearMonticulo(v,n,m);
  free(m);
}
/*
--------------------------------------------------------------------------------
*/
double microsegundos() { /* obtiene la hora del sistema en microsegundos */
struct timeval t;
if (gettimeofday(&t, NULL) < 0 )
return 0.0;
return (t.tv_usec + t.tv_sec * 1000000.0);
}
/*
--------------------------------------------------------------------------------
*/
void inicializar_semilla() {
  srand(time(NULL));
}
/*
--------------------------------------------------------------------------------
*/
void aleatorio(int v [], int n) {
  /* se generan nomeros pseudoaleatorio entre -n y +n */
  int i, m=2*n+1;
  for (i=0; i < n; i++)
  v[i] = (rand() % m) - n;
}
/*
--------------------------------------------------------------------------------
*/
void ascendente(int v [], int n) {
  int i;
  for (i=0; i < n; i++)
  v[i] = i;
}
/*
--------------------------------------------------------------------------------
*/
void descendente(int v [], int n) {
  int i;
  for (i=0; i < n; i++)
  v[i] = n-i;
}
/*
--------------------------------------------------------------------------------
*/
void tempos(int n, void (*ord)(int v [], int n),void (*ini)(int v[], int n),
double cotainf, double cotaax, double cotasup){
  double ta, tb;
  int v[n] , i;
  char aviso=' ';
  double total1 ;
  ini(v,n);
  //printf("3\n" );
  ta = microsegundos();
  ord(v,n);
  tb = microsegundos();
  total1 = tb-ta;
  if (total1<500){
    aviso='*';
    ta = microsegundos();
    for ( i =0 ; i<K ; i++){
      ini(v,n);
      ord(v,n);
    }
    tb = microsegundos();
    total1 = tb-ta;
    ta = microsegundos();
    for ( i =0 ; i<K ; i++){
      ini(v,n);
    }
    tb = microsegundos();
    total1 = (total1-(tb-ta))/K;
  }

  printf("%15i%c %15.3f %15.6f %15.6f %15.6f \n", n , aviso , total1,
  total1/cotainf, total1/cotaax,total1/cotasup);
}
/*
--------------------------------------------------------------------------------
*/
void crear(){
  int h;
    printf("\nOrdenacion de un vector aleatorio\n\n");
    printf("%16s %15s %15s %15s %15s\n", "n","t(n)","t(n)/n", "t(n)/n^1.2",
     "t(n)/n^1.4");
    for ( h = MIN_ITER; h<MAX_ITER ; h = h*2){
       tempos(h,creador,aleatorio,pow(h,1),pow(h,1.2),pow(h,1.4));
     }
}
/*
--------------------------------------------------------------------------------
*/
void ordenar(){
  int h;
  printf("\nOrdenacion de un vector ordenado\n\n");
  printf("%16s %15s %15s %15s %15s\n", "n","t(n)", "t(n)/n", "t(n)/n^1.135",
   "t(n)/n^1.3");
  for ( h = MIN_ITER; h<MAX_ITER ; h = h*2){
     tempos(h,ord_mont,ascendente,h,pow(h,1.145),pow(h,1.3));
   }
   printf("\nOrdenacion de un vector inversamente ordenado\n\n");
   printf("%16s %15s %15s %15s %15s\n", "n","t(n)", "t(n)/n", "t(n)/n^1.125",
    "t(n)/n^1.3");
   for ( h = MIN_ITER; h<MAX_ITER ; h = h*2){
      tempos(h,ord_mont,descendente,h,pow(h,1.125),pow(h,1.3));
    }
    printf("\nOrdenacion de un vector aleatorio\n\n");
    printf("%16s %15s %15s %15s %15s\n", "n","t(n)","t(n)/n", "t(n)/n^1.2",
     "t(n)/n^1.4");
    for ( h = MIN_ITER; h<MAX_ITER ; h = h*2){
       tempos(h,ord_mont,aleatorio,pow(h,1),pow(h,1.2),pow(h,1.4));
     }
}
/*
--------------------------------------------------------------------------------
*/
int main(){
  inicializar_semilla();
  crear();
  ordenar();
}
