/*******************************************************************/ 
/*   Descricao programa:                                           */ 
/*   Jantar 5 filosofos                                            */ 
/*******************************************************************/ 
 
/*********************************/
/* Autor: Ederson Luis Posselt   */
/* Data: 25/03/05                */
/*********************************/
#include<stdio.h> 
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define thread_num 5 
#define max_refeicoes 40 
 
/* initial status of chopstick */ 
/* 0 - garfo em uso            */ 
/* 1 - garfo desocupado        */ 
int garfos[thread_num]={1,1,1,1,1}; 
 
/* mutex_locks para cada garfo*/ 
pthread_mutex_t m[thread_num];  
 
/* ID Filosofos */ 
int p[thread_num]={0,1,2,3,4}; 
 
/* numero de refeicoes consumidadas pelos filosofos */ 
int numRefeicoes[thread_num]={0,0,0,0,0}; 
int numTentativas[thread_num]={0,0,0,0,0}; 

/* contador para o numero de refeicoes */ 
int refeicoesCount = 0; 
 
 
void *filosofo(void *); /* declaracao rotina filosofos */ 
 
 
int main() 
{ 
    pthread_t tid[thread_num];
    void *status;
    int i,j; 

    srand( (long)time(NULL) );
 
    /* criando 5 threads para representar os filosofos */ 
    for (i = 0; i < thread_num; i++) 
    { 
        if(pthread_create( tid + i, 0, filosofo, p + i ) != 0) 
	    { 
	        perror("Erro ao criar thread...."); 
	        exit(1); 
	    } 
    } 
 
    /* join nas 5 threads */ 
    for (i = 0; i < thread_num; i++) 
    { 
        if(pthread_join(tid[i], &status ) != 0) 
        { 
            perror("thr_join() failure."); 
            exit(1); 
        } 
    }  
     
    printf("\n"); 
    printf("------------------------------------------------------\n");
    printf("          Estatistica do jantar dos filosofos         \n");
    printf("------------------------------------------------------\n");

    for(i=0; i<thread_num; i++) 
        printf("Filosofo %d tentou comer %d mas só comeu %d refeicoes.\n", i,numTentativas[i] ,numRefeicoes[i]); 
     
     
    return (0); 
} 
 
 
void *filosofo(void  *arg) 
{  
    int  sub = *(int*)arg; 
 
    //Enquanto menor que o maximo de refeicoes
    while( refeicoesCount < max_refeicoes ) 
    { 
        numTentativas[sub]++;
        printf("Filosofo %d: Eu estou tentando comer!\n", sub); 
 
        /* lock garfo esquerdo */ 
        pthread_mutex_lock( m + sub ); 
         
        if( garfos[sub] == 1 ) 
        { /* the left is available */ 
            printf("Filosofo %d: esquerda=%d\n",sub,garfos[sub]);  
            printf("Filosofo %d: Peguei o garfo esquerdo!\n", sub); 
 
            garfos[sub]=0; /* set the left chopstick unavailable */ 
             
            pthread_mutex_unlock( m + sub ); /* unlock the left chopstick */ 
 
            /* lock the right chopstick */ 
            pthread_mutex_lock( m + ((sub+1)%thread_num) ); 
             
            if( garfos[(sub+1)%thread_num]==1 ) 
            { /* the right is available */ 
                printf("Filosofo %d: Direita=%d\n", sub,
                                    garfos[(sub+1)%thread_num]);
                
                /* set the right chopstick unavailable */ 
                garfos[(sub+1)%thread_num]=0; 
                 
                /* unlock the right chopstick */ 
                pthread_mutex_unlock( m + ( (sub+1) % thread_num) ); 
		 
		printf("Filosofo %d: Peguei os dois garfos!\n", sub); 
                printf("Filosofo %d: Eu estou comendo!\n\n", sub); 
		
                numRefeicoes[sub]++; 
		refeicoesCount++; 
                usleep(rand() % 3000000); /* eating time */ 
 
                /* Libera o garfo da direita e o garfo da esquerda */ 
                pthread_mutex_lock( m + sub );  
                pthread_mutex_lock( m + ( (sub+1) % thread_num) ); 
 
                garfos[sub]=1;   /*Liberando o garfo */ 
                
                /* liberando o garfo da direita */
                garfos[(sub+1)%thread_num]=1;  
 
                /* unlock no garfo da direita e no da esquerda */ 
                pthread_mutex_unlock ( &m[sub]); 
                pthread_mutex_unlock (&m[(sub+1)%thread_num]); 
		
                usleep(rand() % 3000000); /* determinado tempo */ 
            } 
            else 
            { /* garfo da direita em uso */ 
                printf("Filosofo %d: Direito=%d\n", 
                       sub, garfos[(sub+1)%thread_num]);	 
                printf("Filosofo %d:Nao pode pegar garfo direito!\n\n", sub); 
 
                /* unlock no garfo da direita */ 
                pthread_mutex_unlock( &m[(sub+1)%thread_num] ); 
 
                /* lock no garfo da esquerda */ 
                pthread_mutex_lock( &m[sub] ); 
                 
                /* liberando o garfo da esquerda*/ 
                garfos[sub]=1; 
 
                /* unlock no garfo da esquerda */ 
                pthread_mutex_unlock( &m[sub] ); 

                /* dando um sleep */ 
                usleep(rand() % 3000000); 
            } 
        }     
        else 
        { /* garfo da esquerda ocupado */ 
            printf("Filosofo %d: esquerda=%d\n",sub,garfos[sub]);  
            printf("Filosofo %d: Não pode pegar o garfo esquerdo!\n\n",
                   sub); 
 
            /* unlock no garfo da esquerda */ 
            pthread_mutex_unlock( &m[sub] ); 
             
           /* dando um sleep */ 
            usleep(rand() % 3000000);
        } 
	
       sched_yield();  
    } 
    printf("Filosofo %d Terminou de jantar!\n", sub); 
    pthread_exit(0); 
} 

