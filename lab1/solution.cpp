#include <iostream>
#include <bits/stdc++.h>
#include <unistd.h>
#include <signal.h>
#include <cmath>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);

int broj = 0;


int main(){

    struct sigaction act;

    act.sa_handler = obradi_dogadjaj;
    sigemptyset(&act.sa_mask);
    //sigaddset(&act.sa_mask, SIGTERM);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);

    act.sa_handler = obradi_sigterm;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);

    act.sa_handler = obradi_sigint;
    sigaction(SIGINT, &act, NULL);

    printf("Program s PID=%d krenuo s radom\n", getpid());
    fstream status("./status.txt");
    fstream obrada("./obrada.txt");
    if (status.is_open()){
        status >> broj;
        status.close();
        if (broj == 0){
            if (obrada.is_open()){
                obrada >> broj;
                while (!obrada.eof()){
                    obrada >> broj;
                }
                broj = sqrt(broj);
                obrada.clear();
            } else{
                cout << "Greska kod otvaranja obrada.txt";
            }
        }
        fstream status("./status.txt", ios::trunc | ios::in | ios::out);
        status << 0 << endl;

        while (1){
            broj++;
            int x = broj * broj;
            obrada.seekg(0, ios::end);
            obrada << endl;
            obrada << x;
            obrada.flush();
            cout << "Trenutni x: " << broj * broj << endl;
            sleep(5);
        }
    } else{
        cout << "Greska kod otvaranja status.txt";
    }
    status.close();
    obrada.close();
    return 0;
}

void obradi_dogadjaj(int sig){
    cout << broj << endl;
}
void obradi_sigterm(int sig){
    fstream status("./status.txt", ios::trunc | ios::in | ios::out);
    printf("Primio signal SIGTERM\n");

    if (status.is_open()){
        status << broj << endl;
    } else{
        cout << "Pogreška signal SIGTERM";
    }
    status.close();
    exit(1);
}

void obradi_sigint(int sig){

    //Treba li na sigint signal u obradu upisat 0?
        // fstream status("./status.txt", ios::trunc | ios::in | ios::out);
        // if (status.is_open()){
        //     status << 0 << endl;
        // } else{
        //     cout << "Pogreška signal SIGTERM";
        // }
        // status.close();

    printf("Primio signal SIGINT, prekidam rad\n");
    exit(1);
}