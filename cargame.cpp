#include<iostream>
#include<conio.h>
#include<dos.h>
#include <windows.h>
#include <time.h>
#include "cargame.h"
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>
#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
using namespace std;
 
 void setcursor(bool visible, DWORD size);
 void play(string userid);
 void instructions();
bool mainmenu(string userid,int score)
{
	setcursor(0,0);
	srand( (unsigned)time(NULL));

	do{
		system("cls");
		cout<<"\t-------------------------- \tUserID: "<<userid<<endl;
		cout<<" \t|      Dodge The Car     | \n";
		 cout<<" \t--------------------------\n";
		cout<<"\t1. Mulai game";
		cout<<"\n\t2. Instruksi";
		cout<<"\n\t3. Kembali ke menu utama";
		char op = getche();

		if( op=='1') play(userid);
		else if( op=='2') instructions();
		else if( op=='3') break;
		else{
			cout<<"\n\nHanya Pilih opsi yang tersedia!! >:(";
		     this_thread::sleep_for(chrono::milliseconds(1000));
		}
	}while(1);
	return 1;
}
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = { ' ','O','O',' ',
					'O','O','O','O',
					' ','O','O',' ',
					'O','O','O','O' };

int carPos = WIN_WIDTH/2;
int score = 0;

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
void drawBorder(){
	for(int i=0; i<SCREEN_HEIGHT; i++){
		for(int j=0; j<17; j++){
			gotoxy(0+j,i); cout<<"�";
			gotoxy(WIN_WIDTH-j,i); cout<<"�";
		}
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(SCREEN_WIDTH,i); cout<<"�";
	}
}
void genEnemy(int ind){
	enemyX[ind] = 17 + rand()%(33);
}
void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"[**]";
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<" [] ";
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"[**]";
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<" VV ";
	}
}
void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]); cout<<"    ";
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    ";
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    ";
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    ";
	}
}
void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 1;
	genEnemy(ind);
}
/////////////START OF UPDaTE DATA==================================
struct datauser{
	string datauserid;
	string pass;
	string skordata;
};

datauser dat;


int jumlahdata(){
    ifstream data("datauser.txt");
    int itung = 0;
    string baris;

    if (data.is_open()) {
        while (getline(data, baris)) {
 
                itung++;
        }
        data.close();
        
    }else {
        cout << "File tidak ada." << endl;
    }

    return itung;
}
void updatedata(string userid,int skorakhir){
	int jumlah = jumlahdata();

	fstream file;
	file.open("datauser.txt", ios::in | ios::out);

    if (!file.is_open()) {
        cerr << "Error membuka file!" << endl;
            this_thread::sleep_for(chrono::milliseconds(2000));
    }

    ofstream tempFile("tempfile.txt", ios::out);

    file.seekg(0, ios::beg);

	for (int x = 1; x <= jumlah; x++) {
		getline(file, dat.datauserid, ':');
		getline(file, dat.pass, ':');
		getline(file, dat.skordata);
		

		if (dat.datauserid == userid) {
			int updateskor = stoi(dat.skordata);
			skorakhir+= updateskor;
			tempFile << dat.datauserid << ":" << dat.pass << ":" << skorakhir << endl;
		} else {
			tempFile << dat.datauserid << ":" << dat.pass << ":" << dat.skordata << endl;
		}
		if (file.eof()) {
			break; 
		}
	}
		file.close();
		tempFile.close();

		remove("datauser.txt");
		if (rename("tempfile.txt", "datauser.txt") != 0) {
			cerr << "\n\tError menimpa file!" << endl;
		}
}

/////////////END OF UPDaTE DATA==================================
void drawCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<car[i][j];
		}
	}
}
void eraseCar(){
	for(int i=0; i<4; i++){
		for(int j=0; j<4; j++){
			gotoxy(j+carPos, i+22); cout<<" ";
		}
	}
}

int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9  ){
			return 1;
		}
	}
	return 0;
}
void gameover(int skor){
	system("cls");
	cout<<endl;
	cout<<"\t\t--------------------------"<<endl;
	cout<<"\t\t-------- Game Over -------"<<endl;
	cout<<"\t\t--------------------------"<<endl<<endl;
	cout<<"\t\tSkor Akhir: "<<skor<<endl;
	cout<<"\t\tTekan tombol apapun untuk kembali";
	getch();
}
void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"Score: "<<score<<endl;
}

void instructions(){

	system("cls");
	cout<<" Instruksi";
	cout<<"\n ----------------";
	cout<<"\n Hindari mobil dengan bergerak kiri dan kanan. ";
	cout<<"\n\n tekan 'a' untuk bergerak ke kiri";
	cout<<"\n tekan 'd' untuk bergerak ke kanan";
	cout<<"\n tekan 'escape' untuk keluar";
	cout<<"\n\ntekan tombol apapun untuk kembali ke menu";
	getch();
}

void play(string userid){
	int skorakhir;
	carPos = -1 + WIN_WIDTH/2;
	score = 0;
	enemyFlag[0] = 1;
	enemyFlag[1] = 0;
	enemyY[0] = enemyY[1] = 1;

	system("cls");
	drawBorder();
	updateScore();
	genEnemy(0);
	genEnemy(1);
	
	gotoxy(WIN_WIDTH + 7, 2);cout<<"Hindari Mobil Game";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"----------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"----------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"Kontrol ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------- ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<"   Tombol A - Kiri";
	gotoxy(WIN_WIDTH + 2, 15);cout<<"   Tombol D - Kanan";

	gotoxy(18, 5);cout<<"Tekan Tombol Apapun untuk Mulai!";
	getch();
	gotoxy(18, 5);cout<<"                      ";

	while(1){
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( carPos > 18 )
					carPos -= 4;
			}
			if( ch=='d' || ch=='D' ){
				if( carPos < 50 )
					carPos += 4;
			}
			if(ch==27){
				break;
			}
		}

		drawCar();
		drawEnemy(0);
		drawEnemy(1);
		if( collision() == 1  ){
			skorakhir = score;
			updatedata(userid,skorakhir);
			gameover(skorakhir);
			return;
		}
		Sleep(50);
		eraseCar();
		eraseEnemy(0);
		eraseEnemy(1);

		if( enemyY[0] == 10 )
			if( enemyFlag[1] == 0 )
				enemyFlag[1] = 1;

		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;

		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;

		if( enemyY[0] > SCREEN_HEIGHT-4 ){
			resetEnemy(0);
			score++;
			updateScore();
		}
		if( enemyY[1] > SCREEN_HEIGHT-4 ){
			resetEnemy(1);
			score++;
			updateScore();
		}
	}
}


