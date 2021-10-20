/*******************************************************************/ 
/*   Descricao programa:                                           */ 
/*   Soma de 0 a 100000                                            */ 
/*******************************************************************/ 
 
/************************************/
/* Fonte: Material de Fundamentos   */
/* de sistemas operacionais         */
/* Autor: Cristiano Bonato Both     */
/* UNISINOS - São Leopoldo          */
/************************************/

#include <stdio.h>
unsigned long somaNumeros() {
  int i = 0;
  unsigned long soma = 0;
  while(i < 100000) {
    soma += i;
    i++;
  }
  return soma;
}
int main() {
  unsigned long soma;
  soma = somaNumeros();
  printf("%lu\n", soma);
  return 0;
}