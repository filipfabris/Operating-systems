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

#define START 0
#define END INT_MAX


#define ACTIVE 1
#define INACTIVE 0

using namespace std;

int varijabla;
int sig_thread;

int Id;
int* ZajednickaVarijabla;

int Id_2;
int* sig_proces;

void* Ulaz(void* x){
    int size = *(int*)x;
    srand(time(0));

    while (size > 0){

        if (sig_thread == INACTIVE && *sig_proces == INACTIVE){
            varijabla = rand() % 100 + 1;
            cout << "Ulaz: " << varijabla << endl;
            size--;
            sig_thread = ACTIVE;
            sleep(3);
        }

    }

    varijabla = END;
    return NULL;
}

void* Rad(void* x){

    while (varijabla != END){

        if (sig_thread == ACTIVE){

            *ZajednickaVarijabla = varijabla + 1;
            cout << "RAD: " << *ZajednickaVarijabla << endl;

            varijabla = START;

            *sig_proces = ACTIVE;
            sig_thread = INACTIVE;
            sleep(1);
        }
    }

    *ZajednickaVarijabla = END;
    return NULL;
}

void* Izlaz(void* x){
    while (*ZajednickaVarijabla != END){
        if (*sig_proces == ACTIVE){

            fstream file("ispis.txt", fstream::in | fstream::out | ios::app);
            file << *ZajednickaVarijabla << endl;
            file.flush();
            cout << "ZajednickaVarijabla IZLAZ: " << *ZajednickaVarijabla << endl;
            file.close();

            *ZajednickaVarijabla = START;
            *sig_proces = INACTIVE;
            sleep(1);

        }
    }
    return NULL;
}

void brisi(int sig){
    /* oslobađanje zajedničke memorije */
    (void)shmdt((char*)ZajednickaVarijabla);
    (void)shmctl(Id, IPC_RMID, NULL);

    (void)shmdt((char*)sig_proces);
    (void)shmctl(Id_2, IPC_RMID, NULL);
    exit(0);
}


int main(){

    //stvaranje ispis.txt
    fstream file("ispis.txt", fstream::in | fstream::out | fstream::trunc);
    file.close();

    Id = shmget(IPC_PRIVATE, sizeof(int), 0600);
    Id_2 = shmget(IPC_PRIVATE, sizeof(int), 0600);

    if (Id == -1)
        exit(1);

    if (Id_2 == -1){
        exit(1);
    }

    ZajednickaVarijabla = (int*)shmat(Id, NULL, 0);
    *ZajednickaVarijabla = START;
    sigset(SIGINT, brisi);

    sig_proces = (int*)shmat(Id_2, NULL, 0);
    *sig_proces = 1;
    sigset(SIGINT, brisi);

    int i;
    i = 2;


    //Prvi process
    if (fork() == 0){
        pthread_t thr_id_prvi[2];
        varijabla = 0;
        sig_thread = 0;
        // cout << "Unutra sam" << endl;
        if (pthread_create(&thr_id_prvi[0], NULL, Ulaz, &i) != 0){
            printf("Greska pri stvaranju dretve!\n");
            exit(1);
        }
        cout << "Pokrenuta ULAZNA DRETVA\n";

        if (pthread_create(&thr_id_prvi[1], NULL, Rad, NULL) != 0){
            printf("Greska pri stvaranju dretve!\n");
            exit(1);
        }
        cout << "Pokrenuta IZLAZNA DRETVA\n";
        pthread_join(thr_id_prvi[0], NULL);
        pthread_join(thr_id_prvi[1], NULL);
        exit(0);
    }


    //Drugi process
    pthread_t thr_id_drugi[1];
    cout << "Pokrenuta IZLAZNI PROCES\n";
    *sig_proces = START;
    if (pthread_create(&thr_id_drugi[0], NULL, Izlaz, &i) != 0){
        printf("Greska pri stvaranju dretve!\n");
        exit(1);
    }
    // cout << "Jesam li tu ikada " << endl;
    pthread_join(thr_id_drugi[0], NULL);



    (void)wait(NULL);
    (void)wait(NULL);
    brisi(0);

    return 0;
}