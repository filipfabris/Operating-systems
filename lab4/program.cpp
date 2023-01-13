#include <iostream>
#define N 2
#define M 4

using namespace std;

//N procesa
//disk
//okvir
//tablica


class okvir{

public:
    int brojOkvira = 0;
    int brojProcesa = 0;
    int array[5] = {0};
    string s = nullptr;


    okvir(){

    }

    okvir(int brojOkvira, int brojProcesa){
        this->brojOkvira = brojOkvira;
        this->brojProcesa = brojProcesa;
    }

};

class tablica{

public:
    struct element{
        int brojOkvira = 0;
        int bitPrisutnosti = 0;
    };

    element tablicaPrevodenja[1024] = {0};

};



int main(){
    okvir disk[N * M];

    for (int i = 0; i < N * M; i++){
        disk[i] = okvir(0, 0);
    }

    




    cout << sizeof(okvir);








    return 0;
}