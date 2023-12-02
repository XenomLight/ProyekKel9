#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <conio.h>
#include <stdio.h>
#include "cargame.h"
#include "tebakangka.h"
#include "tebakkata.h"
#include <windows.h>
#include <time.h>
#include <algorithm>
#include <iomanip>
#include <windows.h>
using namespace std;

struct Pemain {
    string id, password;
    int skor;
};

Pemain pemain;

void login();
void printdelay(const string &teks, int tunda);
bool cekData(const string &username, const string &password);
void menu_user(string userid,int skor);
int menu_game(string userid,int skor);
void leaderboard(string userid,int skoruser);
int log_out(string userid,int skor);
int refreshpoint(string userid);
int main() {
    login();
 
}

void printdelay(const string &teks, int tunda) {
    for (char c : teks) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(tunda));
    }
}

void login() {
    bool adaga;
    string id, password;
    do {
        adaga = false;
        system("cls");  
        printdelay("\n\t================\n", 1);
        printdelay("\t=W E L C O M E=", 1);
        printdelay("\n\t================\n\n", 1);

        cout << "\n\tUsername: ";
        getline(cin, id);
        cout << "\n\tPassword: ";
        getline(cin, password);

        adaga = cekData(id, password);

        if (adaga) {
            break;
        }
    } while (!adaga);
}

bool cekData(const string &id, const string &password) {
    ifstream data("datauser.txt");
    if (data.is_open()) {
         while (getline(data, pemain.id, ':') &&
                getline(data, pemain.password, ':') &&
                (data >> pemain.skor)) {
             data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (id == pemain.id && password == pemain.password) {
                system("cls");
                cout << "\tBerhasil Login!\n";
                     this_thread::sleep_for(chrono::milliseconds(1000));
                data.close();
                menu_user(pemain.id, pemain.skor);
                return true;
            }
        }
        data.close();
    } else {
        cout << " Data gagal dibuka! ";
        this_thread::sleep_for(chrono::milliseconds(2000));
        return false;
    }

    cout << "\n\tLogin gagal! Username tidak ada atau password salah.\n";
            this_thread::sleep_for(chrono::milliseconds(1000));
    return false;
}

void menu_user(string userid, int skor){

    int pilih;
    while(true){
    system("cls");
    cout << "\tSelamat datang " << userid << "!\t\t\tTotal Score: "<<skor;
    cout<<"\n\n\tMenu:";
    cout<<"\n\t1. Main Game (farming skor)\n";
    cout<<"\t2. Leaderboard\n";
    cout<<"\t3. Log out";
    cout<<"\n\t4. Refresh Point";
    cout<<"\n\t=========================";
    cout<<"\n Pilihan : ";
    pilih = getche();
    if(pilih == '1'||pilih== '2' ||pilih=='3'||pilih=='4'){
        break;
    }
    else{
        cout<<"\n\nHanya pilih pilihan yang tersedia!";
                this_thread::sleep_for(chrono::milliseconds(2000));
        }
    }

    switch (pilih){
        case '1' :
            menu_game(userid,skor);
            break;
        case '2':
            leaderboard(userid,skor);
            break;
        case '3': 
            log_out(userid,skor);
            break;
        case '4':
            refreshpoint(userid);
            break;
    }
}

int menu_game(string userid,int skor){
    int pilih;
    do{
        system("cls");
    cout<<"\t=================\t\tUserID: "<<userid<<endl;
    cout<<"\t>>>>>>Games<<<<<<\n";
    cout<<"\t=================\n";
    cout<<"\n\t1. Dodge the car\n";
    cout<<"\t2. Tebak angka\n";
    cout<<"\t3. Game Egi";
    cout<<"\n\t4. Kembali ke halaman utama";
    cout<<"\n\t=========================";

    pilih = getche();
    }while(pilih!='1'&&pilih!='2'&&pilih!='3'&&pilih!='4');
    bool back;
    switch(pilih){
        case '1':
           	back = mainmenu(userid,skor);
            menu_game(userid,skor);
            break;
        case '2':
            back = maintebak(userid,skor);
            menu_game(userid,skor);
            break;
        case '3':
            back = mainkata(userid,skor); 
            menu_game(userid,skor);
            break;
        case '4':
            menu_user(userid,skor);
            break;

    }
    
    return 1;
}



void leaderboard(string userid,int skoruser){
    int x=0,ranklogin;
    vector<int> scorerank;
    vector<string> userrank;
        ifstream data("datauser.txt");
    if (data.is_open()) {
        Pemain pemain;

        while (getline(data, pemain.id, ':') &&
               getline(data, pemain.password, ':') &&
               (data >> pemain.skor)) {
            data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!scorerank.empty() && !userrank.empty()) {
                auto it = upper_bound(scorerank.begin(), scorerank.end(), pemain.skor, greater<int>());
                size_t index = distance(scorerank.begin(), it);
                scorerank.insert(it, pemain.skor);
                userrank.insert(userrank.begin() + index, pemain.id);
            } else {
                scorerank.push_back(pemain.skor);
                userrank.push_back(pemain.id);
            }

        }
        data.close();
        for(int a =0;a<userrank.size();a++){
            x++;
            if(userrank[a]==userid){
                ranklogin=x;
                break;
            }
        
        }
    } else {
        cout << " Data gagal dibuka! ";
        this_thread::sleep_for(chrono::milliseconds(2000));
        
    }
   system("cls");
   cout<<"_________________________________________________________\n";
   cout<<"|*||*||*|=======================================|*||*||*|\n";
    cout <<"|*||*||*|\t\tLeaderboard\t\t|*||*||*|\n";
    cout<<"\\v/\\v/\\v/"<<"======================================="<<"\\v/\\v/\\v/";
    cout << "\t\t"<< "User ID: " <<userid<<'\t' << "Score: "<<skoruser<<'\t'<<"Rank: "<<ranklogin;
    cout << "\n\t Rank"<<"\t\t"<<setw(15)<<left<<"UserID   "<<"\t  "<<"Score\n";

       for (int i = 0; i <20 ; i++) {
        cout <<"\t"<<setw(3)<<right<<i+1<<" \t\t"<< setw(15) << left<<userrank[i] <<"\t    "<< scorerank[i] << '\n'<<endl;
       }
    cout<<"\n\tTekan tombol apapun untuk kembali ke halaman awal!";
    getch();
    menu_user(userid,skoruser);
}


int log_out(string userid,int skor){
    system("cls");
    cout << "\n\tLogging out";
    printdelay("...",100);
    cout<<endl;
    this_thread::sleep_for(chrono::milliseconds(2000));

    cout << "\n\tLogged out successfully! Bye "<<userid<<" !\n";
    this_thread::sleep_for(chrono::milliseconds(2000));

    main();
    return 0; 
}

int refreshpoint(string userid){
    ifstream data("datauser.txt");
    if (data.is_open()) {
         while (getline(data, pemain.id, ':') &&
                getline(data, pemain.password, ':') &&
                (data >> pemain.skor)) {
             data.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (userid == pemain.id) {
                
                data.close();
                menu_user(userid,pemain.skor);
            
            }
        }
        data.close();
    } else {
        cout << " Data gagal dibuka! ";
        this_thread::sleep_for(chrono::milliseconds(2000));
        return 0;
    }
    return 0;
}

