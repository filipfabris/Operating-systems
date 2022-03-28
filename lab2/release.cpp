
#include <stdio.h>
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



int main(int argc, char const* argv[]){

    int i;
    for (i = 1; i < MAXLONG; i++){
        if (shmctl(i, IPC_RMID, NULL) != -1)
            printf("Obrisao zajednicku memoriju %d\n", i);

        if (semctl(i, 0, IPC_RMID, 0) != -1)
            printf("Obrisao skup semafora %d\n", i);

        if (msgctl(i, IPC_RMID, NULL) != -1)
            printf("Obrisao red poruka %d\n", i);
    }
    return 0;
}
