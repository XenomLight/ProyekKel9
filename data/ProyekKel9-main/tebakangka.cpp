#include <iostream>
#include <cstdlib>
#include <ctime>
#include "tebakangka.h" 
#include "cargame.h"
#include <string>
#include <stdio.h>
#include <conio.h>
#include <limits>
#include <fstream>
#include <thread>
#include <chrono>
using namespace std;

void playGame(string userid,int skor);
void displayInstructions();

bool maintebak(string userid,int skor) {
    char choice;

    do {
    
        system("cls");
        cout << "\n\t\t\t=====<<< Game Tebak Angka >>>=====" << endl;
        cout << "\n\t1. Mulai Game" << endl;
        cout << "\t2. Petunjuk" << endl;
        cout << "\t3. Keluar" << endl;
        cout << "\tPilihan: ";
        choice = getche();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case '1':
                playGame(userid,skor);
                break;
            case '2':
                displayInstructions();
                break;
            case '3':
                cout << "\n\tTerima kasih telah bermain. Sampai jumpa!" << endl;

                break;
            default:
                cout << "\n\tPilihan tidak valid. Silakan coba lagi." << endl;
        }

    } while (choice != '3');

    return true;
}

void playGame(string userid,int skor) {
            system("cls");
    srand(static_cast<unsigned int>(time(0)));

    int totalScore = 0;
    int totalErrors = 0;

    while (true) {
        int randomNumber = rand() % 10 + 1;

        int score = 0;
        int errors = 0;

        while (errors < 3) {
            cout << "\n\tTebak angka (1-10): ";

            int guess;
            cin >> guess;

            if (guess == randomNumber) {
                cout << "\n\tTebakan benar! +2 poin" << endl;
                score += 2;
                totalScore += score;
                break; 
            } else {
                cout << "\n\tTebakan salah." << endl;
                ++errors;

                if (guess < randomNumber) {
                    cout << "\t<???> || Hint: Angka lebih besar dari " << guess << "      || <???>"<< endl;
                } else {
                    cout << "\t<???> || Hint: Angka lebih kecil dari " << guess << "      || <???>"<< endl;
                }

                cout << "\t<!!!> || Kesalahan ke-" << errors << ". Percobaan tersisa "<<3 - errors << " || <!!!>" << endl;
            }
        }

        if (errors == 3) {
            cout << "\n\tGame over. Anda telah melakukan 3 kesalahan." << endl;
            cout << "\tAngka yang benar adalah " << randomNumber << endl;
            cout << "\tSkor akhir untuk sesi ini: " << totalScore << " poin" << endl;
            updatedata(userid,totalScore);
            cout<<"\t\tTekan tombol apapun untuk kembali";
	        getch();
            break; 
        }

        cout << "\tSkor total: " << totalScore << " poin" << endl;

        cout << "\tPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}

void displayInstructions() {
    system("cls");
    cout << "\t\t\t    =====<<< Petunjuk >>>=====" << endl;
    cout << "\n\t1. Tebak angka antara 1 dan 10." << endl;
    cout << "\t2. Setiap tebakan yang benar mendapatkan 10 poin." << endl;
    cout << "\t3. Jika tebakan salah sebanyak 3 kali berurutan, maka permainan berakhir." << endl;
    cout << "\t4. Jika tebakan benar setelah melakukan 1 atau 2 kali kesalahan, maka \n\t   riwayat kesalahan akan dihapus." << endl;
    cout << "\t5. Skor total akan diakumulasikan setiap putaran." << endl;
    cout << "\n\t\t\t    ==========================" << endl;
    	cout<<"\t\t\tTekan tombol apapun untuk kembali";
	getch();
}
