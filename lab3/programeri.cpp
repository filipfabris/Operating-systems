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
#define N 3

//g++ programeri.cpp -o programeri -pthread

using namespace std;



pthread_mutex_t m;
pthread_cond_t red[2];

int brojac[2] = {0, 0}; //Broje koliko je progamera u restoranu


//bool vrsta = 0;     //Trenutno u restoranu
//0 - WINDOWS
//1 - LINUX

int ceka[2] = {0, 0}; //Koliko ih ceka u redu

int siti[2] = {0,0}; //Koliko ih se je najelo od zadnje smijene


void udji(bool p_vrsta){
    pthread_mutex_lock(&m);
    ceka[p_vrsta]++;
    while (brojac[1 - p_vrsta] > 0 || siti[p_vrsta] >= N && ceka[1 - p_vrsta] > 0){
        pthread_cond_wait(&red[p_vrsta], &m);
    }
    brojac[p_vrsta]++;
    ceka[p_vrsta]--;
    siti[1 - p_vrsta] = 0;
    // cout << "Programer " << p_vrsta << " usao u restoran" << endl;
    pthread_mutex_unlock(&m);
}

void izadi(bool p_vrsta){
    pthread_mutex_lock(&m);

    brojac[p_vrsta]--;
    siti[p_vrsta]++;

    if (brojac[p_vrsta] == 0){
        pthread_cond_signal(&red[1 - p_vrsta]);
    }
    cout << "Programer " << p_vrsta << " se je najeo" << endl;
    pthread_mutex_unlock(&m);
}


void* Posjetitelj(void* x){
    bool moja_vrsta = *(bool*)x;
    while (1){
        udji(moja_vrsta);
        sleep(2);
        izadi(moja_vrsta);
    }
    return NULL;
}


int main(){

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&red[0], NULL);
    pthread_cond_init(&red[1], NULL);


    int brojDretvi = 3;

    pthread_t thr_windows[brojDretvi];
    pthread_t thr_linux[brojDretvi];

    int windowsP = 0;
    int linuxP = 1;

    for (int i = 0; i < brojDretvi; i++){
        pthread_create(&thr_windows[i], NULL, Posjetitelj, &windowsP); //Windowss
        pthread_create(&thr_linux[i], NULL, Posjetitelj, &linuxP); //Linux
    }

    for (int i = 0; i < brojDretvi; i++){
        pthread_join(thr_windows[i], NULL);
        pthread_join(thr_linux[i], NULL);
    }




    return 0;
}