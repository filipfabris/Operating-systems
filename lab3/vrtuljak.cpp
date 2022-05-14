#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <values.h>
#include <time.h>
#include <random>
#include <iostream>
#include <limits>
#include <bits/stdc++.h>
#include <semaphore.h>

//g++ vrtuljak.cpp -o vrtuljak -pthread

using namespace std;

int ID;
sem_t* sem1;

int ID2;
sem_t* sem2;

int ID3;
sem_t* sem3;

int ID4;
sem_t* sem4;


void Posjetitelj(){
    while (1){
        sem_wait(sem1);
        sleep(2);
        cout << "Udi i sjedi" << endl;

        sem_post(sem2);

        sem_wait(sem3);
        cout << "Ustani i izadi" << endl;

        sem_post(sem4);
    }
}

void* Vrtuljak(void* x){
    while (1){
        int size = *(int*)x;

        for (int i = 0; i < size; i++){
            sem_post(sem1);
        }

        for (int i = 0; i < size; i++){
            sem_wait(sem2);
        }

        cout << "Pokreni vrtuljak" << endl;

        cout << "Zaustavi vrtuljak" << endl;

        for (int i = 0; i < size; i++){
            sem_post(sem3);
        }

        for (int i = 0; i < size; i++){
            sem_wait(sem4);
        }
    }

    return NULL;
}

void brisi(int sig){
    /* oslobađanje zajedničke memorije */

    sem_destroy(sem1);
    sem_destroy(sem2);
    sem_destroy(sem3);
    sem_destroy(sem4);

    shmdt(sem1);
    shmdt(sem2);
    shmdt(sem3);
    shmdt(sem4);

    shmctl(ID, IPC_RMID, NULL);
    shmctl(ID2, IPC_RMID, NULL);
    shmctl(ID3, IPC_RMID, NULL);
    shmctl(ID4, IPC_RMID, NULL);

    exit(0);
}


int main(){

    ID = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
    sem1 = (sem_t*)shmat(ID, NULL, 0);
    sem_init(sem1, 1, 0);

    ID2 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
    sem2 = (sem_t*)shmat(ID2, NULL, 0);
    sem_init(sem2, 1, 0);

    ID3 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
    sem3 = (sem_t*)shmat(ID3, NULL, 0);
    sem_init(sem3, 1, 0);

    ID4 = shmget(IPC_PRIVATE, sizeof(sem_t), 0600);
    sem4 = (sem_t*)shmat(ID4, NULL, 0);
    sem_init(sem4, 1, 0);

    sigset(SIGINT, brisi);

    //Unutra proces koji stavra posjetitelje
    if (fork() == 0){
        // pthread_t thr_id_prvi[3];

        // for (int i = 0; i < 3; i++){
        //     cout << "Stvorena dretva POSJETITELJ" << endl;
        //     pthread_create(&thr_id_prvi[i], NULL, Posjetitelj, NULL);
        // }

        // for (int i = 0; i < 3; i++){
        //     pthread_join(thr_id_prvi[i], NULL);
        // }

        for (int i = 0; i < 3; i++){
            cout << "Stvoren PROCESS POSJETITELJ" << endl;
            if (fork() == 0){
                Posjetitelj();
                exit(0);
            }
        }

        for (int i = 0; i < 3; i++){
            (void)wait(NULL);
        }

        exit(0);
    }


    //Proces vrtuljak
    int broj_posjetitelja = 3;
    pthread_t thr_id_drugi;
    pthread_create(&thr_id_drugi, NULL, Vrtuljak, &broj_posjetitelja);
    pthread_join(thr_id_drugi, NULL);


    (void)wait(NULL);
    brisi(0);

    return 0;
}