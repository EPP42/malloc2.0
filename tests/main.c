#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
          char *tab[10] = {NULL}; 
          for (int i = 0; i < 10; i++)
                {
                    tab[i] = realloc(NULL, 500);
                    for ( int j = 0; j < 100; j++)
                          {
                              tab[i][j] = j; 
//                              printf( ":::::::: ADD : %p ", tab[i]); 
                          }
                    tab[i] = realloc(tab[i],200); 
                      for ( int j = 0; j < 100; j++)
                          {
                              printf("tab [%d][%d] = %d  j = %d ",i,j,tab[i][j],j); 
                              assert(tab[i][j] == j); 
                          }
                }
          for (int i = 0; i < 10; i++)
                    free(tab[i]); 
          return 0;
}
