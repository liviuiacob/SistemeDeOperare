#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include "a2_helper.h"

pthread_mutex_t mutex1, mutex2;
sem_t s1, s2;
sem_t *s3, *s4;

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t varCond= PTHREAD_COND_INITIALIZER;
pthread_cond_t varCond2= PTHREAD_COND_INITIALIZER;

int nrThreads=0;
int thClosed = 0;
int soSculat=0;

typedef struct params1{
    int i;
    int process;
} params;

void* thread_fn23_process5(void *args)
{
    
    params* numar= (params*) args;
    if (numar->i==3)
        pthread_mutex_lock(&mutex1);

    /*
    if (numar->i==2)
        sem_wait(s3);
    */


    info (BEGIN, 5, numar->i);

    if (numar->i==1){
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);
    }

    info (END, 5, numar->i);

    /*
    if (numar->i==2){
        sem_post(s3);
        sem_post(s4);
    }
    */

    if (numar->i==3)
        pthread_mutex_unlock(&mutex2);


    return NULL;
}

void* thread_fn24(void *args)
{
    //11 nu are voie decat la 5 care ruleaza
    
    params* numar= (params*) args;
    
    sem_wait(&s1);
    info(BEGIN, 4, numar->i);

    sem_wait(&s2);
    nrThreads++;
    sem_post(&s2);
    
    //work
    if (numar->i==11){

        soSculat=1;
        while(1){
            sem_wait(&s2);
            if (nrThreads==5 || thClosed==49){
                info(END, 4, numar->i);
                nrThreads--;
                soSculat=0;
                sem_post(&s2);
                break;
            }
            sem_post(&s2);  
        }
    }else{
        while(soSculat);
        sem_wait(&s2);
        nrThreads--;
        thClosed++;
        info(END, 4, numar->i);
        sem_post(&s2);
        
    }
    sem_post(&s1);
    sem_destroy(&s1);
    


    return NULL;
}

/*
void* thread_fn241(void *args)
{
    //11 nu are voie decat la babenco sa iasa
    
    params* numar= (params*) args;

    sem_wait(&s1);

    pthread_mutex_lock(&lock1);
    nrThreads=nrThreads+1;
    pthread_mutex_unlock(&lock1);
    info (BEGIN, 4, numar->i);

    
    if (nrThreads==5 && soSculat==1)
        pthread_cond_signal(&varCond);
    
    
    pthread_mutex_lock(&lock1);
    nrThreads=nrThreads-1;
    pthread_mutex_unlock(&lock1);

    info(END, 4, numar->i);
    
    sem_post(&s1);
    
    return NULL;
}
void* thread_fn2411(void *args)
{
    //11 nu are voie decat la babenco sa iasa
    
    //params* numar= (params*) args;
    sem_wait(&s1);
    
    pthread_mutex_lock(&lock1);
    soSculat=1;
    nrThreads=nrThreads+1;
    pthread_mutex_unlock(&lock1);
    info (BEGIN, 4, 11);

    pthread_mutex_lock(&lock1);
    while (!(nrThreads==5 && soSculat==1)){
        pthread_cond_wait(&varCond, &lock1);
    }
    soSculat=0;
    nrThreads=nrThreads-1;
    pthread_mutex_unlock(&lock1);

    info(END, 4, 11);
    
    sem_post(&s1);
    
    return NULL;
}
*/
/*
void* thread_fn242(void *args)
{
    //11 nu are voie decat la babenco sa iasa
    
    params* numar= (params*) args;

    sem_wait(&s1);

    pthread_mutex_lock(&lock1);
    nrThreads=nrThreads+1;
    pthread_mutex_unlock(&lock1);
    info (BEGIN, 4, numar->i);
    if (numar->i==11){
        soSculat=1;
        pthread_mutex_lock(&lock1);
        while (!(nrThreads==5 && soSculat==1)){
            pthread_cond_wait(&varCond, &lock1);
        }
        soSculat=0;
        nrThreads=nrThreads-1;
        pthread_mutex_unlock(&lock1);
        info(END, 4, numar->i);
    }else{

    
    if (nrThreads==5 && soSculat==1)
        pthread_cond_signal(&varCond);
    
    
    pthread_mutex_lock(&lock1);
    nrThreads=nrThreads-1;
    pthread_mutex_unlock(&lock1);

    info(END, 4, numar->i);
    }

    
    sem_post(&s1);
    
    return NULL;
}
*/




void* thread_fn25_process3(void *args)
{
    //3.2 trebe sa termine inainte ca 5.2 sa inceapa
    //5.2 trebe sa termine inainte ca 3.3 sa inceapa

    params* numar= (params*) args;

    if (numar->i==3)
        pthread_mutex_lock(&mutex1);

    info (BEGIN, 3, numar->i);

    if (numar->i==2){
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);
    }

    info (END, 3, numar->i);

    if (numar->i==3)
        pthread_mutex_unlock(&mutex2);


    return NULL;

    /*
    params*numar= (params*) args;
    
    if (numar->i == 3)
        sem_wait(s4);

    info(BEGIN, 3, numar->i);

    info(END, 3, numar->i);

    if (numar->i == 2)
        sem_post(s3);

    return NULL;
    */
}

int main(){
    init();
    //aici incepe p1
    info(BEGIN, 1, 0);
    
    pid_t pid2=-1, pid3=-1, pid4=-1, pid5=-1, pid6=-1, pid7=-1;

    pid2=fork();
    if(pid2 == -1) {
        perror("Could not create process p2");
        return -1;
    } else if(pid2 == 0) {
        //aici incepe p2
        info(BEGIN, 2, 0);
        pid3=fork();
        if (pid3==-1){
            perror("Could not create process p3");
            return -1;
        }
        else if(pid3==0) {
            //aici incepe p3
            info (BEGIN, 3, 0);

            pthread_t tid[5];
            params p[5];

            s3=sem_open("sem3", O_CREAT | O_EXCL , 0644, 1);
            sem_unlink("sem3");
            
            for (int i=0; i<5; i++){
                p[i].i=i+1;
                pthread_create(&tid[i], NULL, thread_fn25_process3, &p[i]);
            }
            for (int i=0; i<5; i++)
                pthread_join(tid[i], NULL);



            pid4=fork();
            if (pid4==-1){
                perror("Could not create process p4");
                return -1;
            }
            else if (pid4==0){
                //aici incepe p4
                info(BEGIN, 4, 0);

                pthread_t tid[50];
                params p[50];
                //sem_init(&s1, 0, 5);
         
                /*
                for (int i=0; i<50; i++){
                    p[i].i=i+1;
                    if (i==10){
                        
                        pthread_create(&tid[i], NULL, thread_fn2411, &p[i]);
                    }else
                        pthread_create(&tid[i], NULL, thread_fn241, &p[i]);
                }
                for (int i=0; i<50; i++)
                    pthread_join(tid[i], NULL);
                */
             //   /*
                //asta merge cu threadfn24
                sem_init(&s1, 0, 5);
                if(sem_init(&s2, 0, 1) != 0) {
                    perror("Could not init the semaphore");
                    return 0;
                }

                for (int i=0; i<50; i++){
                    p[i].i=i+1;
                    pthread_create(&tid[i], NULL, thread_fn24, &p[i]);
                }
                for (int i=0; i<50; i++)
                    pthread_join(tid[i], NULL);
             //   */



                info(END, 4, 0);
                //s-a terminat p4
            }
            else{
                //suntem in p3
                waitpid(pid4, NULL, 0);
                info(END, 3, 0);
                //s-a terminat p3
            }
        }
        else{
            //suntem in p2
            pid6=fork();
            if (pid6==-1){
                perror("Could not create process p6");
                return -1;
            }
            else if (pid6==0){
                //aici incepe p6
                info (BEGIN, 6, 0);

                pid7=fork();
                if (pid7==-1){
                    perror("Could not create process p7");
                    return -1;
                }
                else if (pid7==0){
                    //aici incepe p7
                    info (BEGIN, 7, 0);



                    info(END, 7, 0);
                    //s-a terminat p7
                }
                else{
                    //suntem in p6
                    waitpid(pid7, NULL, 0);
                    info(END, 6, 0);
                    //s-a terminat p6
                }
            }
            else{
                //suntem in p2
                waitpid(pid3, NULL, 0);
                waitpid(pid6, NULL, 0);
                info (END, 2, 0);
                //s-a terminat p2
            }
        }
        
    } 
    else{
        //suntem in p1
        pid5=fork();
        if (pid5==-1){
            perror("Could not create process p5");
            return -1;
        }
        else if (pid5==0){
            //aici incepe p5
            info (BEGIN, 5, 0);
            
            
            pthread_t tid[4];
            pthread_mutex_init(&mutex1, 0);
            pthread_mutex_init(&mutex2, 0);
            pthread_mutex_lock(&mutex1);
            pthread_mutex_lock(&mutex2);
            
            params p[4];
            s4=sem_open("sem4", O_CREAT | O_EXCL , 0644, 1);
            sem_unlink("sem4");
            
            for (int i=0; i<4; i++){
                p[i].i=i+1;
                pthread_create(&tid[i], NULL, thread_fn23_process5, &p[i]);
            }
            for (int i=0; i<4; i++)
                pthread_join(tid[i], NULL);

            
            info(END, 5, 0);
            //s-a terminat p5
        }
        else{
            //suntem in p1
            waitpid(pid2, NULL, 0);
            waitpid(pid5, NULL, 0);
            info(END, 1, 0);
            //s-a terminat p1
        }
    }
    
    return 0;
}
