#include <iostream>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <iomanip>
#include <string>
#include <ctime>
#include "tebakkata.h"
#include "cargame.h"
#include <stdio.h>
#include <limits>
#include <thread>
#include <chrono>
using namespace std;

HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD cursorposition;

void play(string userid,int skor); void transition(); void instruction();
void powerUP(int input, int type);
void updateUI(string section);
int loadData(string location, string arr[]);
void spawnEnemy(); void enemyAttack(); void giftFromEnemy(); void playerDied(string userid);
void wordAssigner(); int compareWord(char input); void isWordCorrect();
int isRightNumber(int input);
int controller(int *posX, int *posY, int xPar1, int yPar1, int xPar2, int yPar2);
int findSpacesDash(string kata);
void Gotoxy(int x, int y);
void SetCursor(bool visible, DWORD size);
string changeCase(string teks, char to);
int loadImage(string location, int posX, int posY, int mode = 1, char xBorder = 'z', char yBorder = ';');
void animate(string location, int posX, int posY, char xBorder = '"', char yBorder = ';');
int random(int minimum, int maksimum);
int findNumber(int arr[], int size, int number);
int findBorderMax(string location, char border);
int findXMax(string location, char border);
int findYMax(string location, char border);
int findIndex(string kata, char karakter);
void missing(string location, string problem);

const int wordCount = 8;
int isArrow; bool fromOtherMenu;
int sleepCount = 0;
int wordSpawnDelay = 500;
int powerUP1Delay = 10000;
int powerUP2Delay = 30000;
int notificationDelay;
float percentageEnemyDiedScore = 0.8;
float percentageWordCorrectScore = 0.8;
float percentageEnemyDiedHP = 0.5;
int first = true;

struct player{
    int sane = 100, score = 0, powerUP1 = 0, powerUP2 = 0;
};

struct gameData{
    int selectedWord = -1;
    int wordID[wordCount], wordDmg[wordCount], wordSize;
    string word[wordCount], wordType[wordCount], wordRan[wordCount];
    string listBuah[1000], listHewan[1000], listSport[1000];
    int jumlahBuah, jumlahHewan, jumlahSport;

    int enemyHP, enemyMHP, enemyDmg, enemyAtkSpeed, enemyTime;
};

struct pointer{
    int posX = 18, posY = 11;
};

pointer menuPointer;
player one;
gameData session;

bool mainkata(string userid,int skor){
    srand(time(0));
    SetCursor(0, 0);
    for (int i = 0; i < 8; i++) session.wordID[i] = NULL;

    session.jumlahBuah = loadData("./data/buah.txt", session.listBuah);
    session.jumlahHewan = loadData("./data/hewan.txt", session.listHewan);
    session.jumlahSport = loadData("./data/sport.txt", session.listSport);
    if (session.jumlahBuah == -1 || session.jumlahHewan == -1 || session.jumlahSport == -1) return 1;

    instruction();
    while(1){
        if (fromOtherMenu) system("cls"); loadImage("./gambar/title.txt", 5, 1, 1, '[', ']');
        fromOtherMenu = false;

        Gotoxy(menuPointer.posX, menuPointer.posY); cout << ">";
        do isArrow = controller(&menuPointer.posX, &menuPointer.posY, 17, 10, 18, 12);
            while (isArrow == 0);
        if (menuPointer.posY == 10 && isArrow == 2){transition(); play(userid,skor);}
        else if (menuPointer.posY == 11 && isArrow == 2) instruction();
        else if (menuPointer.posY == 12 && isArrow == 2){system("cls"); return 1;}
    }
}

void play(string userid,int skor){
    loadImage("./gambar/UI.txt", 0, 0);
    if (first){spawnEnemy(); first = false;}
    updateUI("selected"); updateUI("sane"); updateUI("score"); updateUI("powerup"); updateUI("enemy");

    while(1){
        isWordCorrect();

        if (session.enemyHP <= 0){
            giftFromEnemy(); spawnEnemy(); session.enemyTime = 0; updateUI("enemy");
            Gotoxy(16, 13); cout << "You defeated the enemy!"; notificationDelay = 1000;}

        if (kbhit()){
            int input = getch();
            if (input == 75){
                powerUP(input, 1); updateUI("powerup");}
            else if (input == 77){
                powerUP(input, 2); updateUI("powerup");}
            else if (input == 13){
                fromOtherMenu = true;
                menuPointer.posX = 17;
                menuPointer.posY = 10;
                break;}
            else if (isRightNumber(input) != 0){
                session.selectedWord = input - 49;
                updateUI("selected");}
            else if (session.selectedWord != -1 && isalpha(input)){
                if (compareWord(input) == 0){
                    one.sane -= 1; updateUI("sane");
                } updateUI("selected");
            }
        }

        Sleep(50); sleepCount += 50; session.enemyTime += 50; Gotoxy(0,0);

        if (session.enemyTime == session.enemyAtkSpeed) enemyAttack();
        else if (session.enemyTime > session.enemyAtkSpeed) session.enemyTime = 0;
        else if (session.enemyTime % 1000){
            Gotoxy(52,21); cout << 0; Gotoxy(53, 21); cout << (session.enemyAtkSpeed-session.enemyTime)/1000;}

        if(sleepCount % wordSpawnDelay == 0){
            wordAssigner(); updateUI("list");}

        if (sleepCount % powerUP1Delay == 0){
            one.powerUP1 += 1;
            Gotoxy(12, 13); cout << "You got one 'Exterminate One' power!";
            notificationDelay = 1000;
            if (one.powerUP1 > 5) one.powerUP1 = 5; updateUI("powerup");
        }
        if ((sleepCount % powerUP2Delay == 0) && one.powerUP1 >= 2){
            one.powerUP1 -= 2;
            one.powerUP2 += 1;
            Gotoxy(12, 13); cout << "You got one 'Exterminate all' power!";
            notificationDelay = 1000;
            if (one.powerUP2 > 2) one.powerUP2 = 2; updateUI("powerup");
        }

        if (notificationDelay > 0) notificationDelay -= 50;
        else if (notificationDelay <= 0){Gotoxy(6, 13); cout << "                                              ";}

        if (one.sane > 100) one.sane = 100;
        if (one.sane <= 0){
            playerDied(userid);
            break;
        }
    }
    mainkata(userid,skor);
}

void powerUP(int input, int type){
    if (type == 1){
        if (one.powerUP1 > 0){
            Gotoxy(12, 13); cout << "Pilih kata yang ingin dihapus!";
            input = getch();
            if (isRightNumber(input) != 0 && session.wordID[input-49] != NULL){
                session.wordRan[input-49] = session.word[input-49];
                one.powerUP1 -= 1;
                Gotoxy(6, 13); cout << "                                              ";}
            else {
                Gotoxy(12, 13); cout << "Kata yang ingin dihapus tidak ada!";
            }
        } else {
            Gotoxy(12, 13); cout << "You don't have any 'Exterminate One' power!";
        }
    } else if (type == 2){
        if (one.powerUP2 > 0){
            for (int i = 0; i < wordCount; i++){
                session.wordRan[i] = session.word[i];
            } one.powerUP2 -= 1;
        } else {
            Gotoxy(12, 13); cout << "You don't have any 'Exterminate All' power!";
        }
    } notificationDelay = 1000;
}

void updateUI(string section){
    section = changeCase(section, 'l');
    if (section == "score"){
        if (one.score < 10000){Gotoxy(42, 21); cout << setw(3) << one.score;}
        else {Gotoxy(40, 21); cout << setw(6) << one.score;}
    } else if (section == "enemy"){
        Gotoxy(30, 2); cout << "Enemy's HP";
        Gotoxy(30, 3); cout << "    ";
        Gotoxy(30, 3); cout << setw(5) << session.enemyHP << "/" << session.enemyMHP;
    } else if (section == "sane"){
        Gotoxy(29, 21); cout << setw(4) << one.sane << "    ";
        Gotoxy(29, 21); cout << setw(4) << one.sane << "%";
    } else if (section == "powerup"){
        Gotoxy(22, 15); cout << one.powerUP1;
        Gotoxy(49, 15); cout << one.powerUP2;
    } else if (section == "selected"){
        if (session.selectedWord == -1 || findNumber(session.wordID, wordCount, session.selectedWord+1) == -1){
            session.selectedWord = -1;
            Gotoxy(42, 17); cout << "          ";
            Gotoxy(42, 17); cout << "NONE";
            Gotoxy(32, 18); cout << "                    ";
        } else {
            Gotoxy(42, 17); cout << "          ";
            Gotoxy(32, 18); cout << "                    ";
            Gotoxy(42, 17); cout << session.wordType[session.selectedWord];
            Gotoxy(32, 18); cout << session.selectedWord + 1 << ". " << session.wordRan[session.selectedWord];
        }
    } else if (section == "list"){
        for (int i = 0; i < wordCount; i++){
            if (i < 4){
                Gotoxy(2, 17+i);
                if (findNumber(session.wordID, wordCount, i+1) != -1){cout << i+1 << ". " << session.wordType[i]; Gotoxy(11, 17+i); cout << session.wordDmg[i];}
                else cout << "            ";
            }
            else {
                Gotoxy(15, 13+i);
                if (findNumber(session.wordID, wordCount, i+1) != -1){cout << i+1 << ". " << session.wordType[i]; Gotoxy(24, 13+i); cout << session.wordDmg[i];}
                else cout << "            ";
            }
        }
    }
}

void spawnEnemy(){
    int type = random(1, 9);
    if (type <= 3) session.enemyMHP = random(15, 30);
    else if (type <= 6) session.enemyMHP = random(31, 60);
    else if (type <= 9) session.enemyMHP = random(61, 90);
    session.enemyHP = session.enemyMHP;

    session.enemyDmg = random(5, 15);
    session.enemyAtkSpeed = random(3, 7) * 1000;
}

void enemyAttack(){
    one.sane -= session.enemyDmg; updateUI("sane");
}

void giftFromEnemy(){
    one.score += percentageEnemyDiedScore * session.enemyMHP + (session.enemyAtkSpeed/1000) * 0.5;
    one.sane += percentageEnemyDiedHP * session.enemyMHP + (session.enemyAtkSpeed) * 0.5;
    updateUI("score");
}

void playerDied(string userid){
    system("cls");
    Gotoxy(2, 12); cout << "lol you died, it's whether you lose score from dying";
    Gotoxy(2, 13); cout << "or lose score from this game crashing";
    Gotoxy(2, 14);cout<<"Last score : "<<one.score;
    this_thread::sleep_for(chrono::milliseconds(5000));
    updatedata(userid,one.score);
     Gotoxy(2, 17);cout<<"\tPress any key to continue....";
     getch();
    one.score = 0; one.sane = 100; one.powerUP1 = one.powerUP2 = 0; for (int i = 0; i < wordCount; i++) session.wordID[i] = 0;
    first = true; fromOtherMenu = true;
}

void wordAssigner(){
    if (session.wordSize < 8){
        int index = findNumber(session.wordID, wordCount, 0);
        session.wordID[index] = index+1;
        int type = random(1, 9);
        string temp;
        if (type <= 3){
            session.wordType[index] = "Buah";
            session.word[index] = session.listBuah[random(0, session.jumlahBuah-2)];}
        else if (type <= 6){
            session.wordType[index] = "Hewan";
            session.word[index] = session.listHewan[random(0, session.jumlahHewan-2)];}
        else if (type <= 9){
            session.wordType[index] = "Sport";
            session.word[index] = session.listSport[random(0, session.jumlahSport-2)];}

        session.wordRan[index] = session.word[index];
        int letterRandomizedCount = random(2, 0.6 * (session.word[index].length()));
        char letterRandomized[1000]; for (int i = 0; i < 1000; i++) letterRandomized[i] = NULL;
        int letterIndex;
        for (int i = 0; i < letterRandomizedCount; i++){
            bool sama;
            do {
                sama = false;
                letterIndex = random(0, (session.word[index].length()-1-findSpacesDash(session.word[index])));
                for (int y = 0; y < letterRandomizedCount; y++){
                    if (letterIndex == letterRandomized[y] || session.word[index][letterIndex] == ' ' || session.word[index][letterIndex] == '-')
                        sama = true;
                }
            } while (sama);
            letterRandomized[i] = letterIndex;
            session.wordRan[index][letterIndex] = '_';
        }

        type = random(1, 9);
        if (type <= 3) session.wordDmg[index] = random(5, 30);
        else if (type <= 6) session.wordDmg[index] = random(31, 60);
        else if (type <= 9) session.wordDmg[index] = random(61, 85);
        session.wordSize += 1;
    }
}

int compareWord(char input){
    input = tolower(input);
    string teks = session.wordRan[session.selectedWord];
    int countDash = 0, dashIndex[1000];
    for (int i = 0; i < teks.length(); i++){
        if (teks[i] == '_'){
            dashIndex[countDash] = i;
            countDash++;}
    }

    bool benar = false;
    teks = changeCase(teks, 'l');
    for (int i = 0; i < countDash; i++){
        if (input == tolower(session.word[session.selectedWord][dashIndex[i]])){
            session.wordRan[session.selectedWord][dashIndex[i]] = session.word[session.selectedWord][dashIndex[i]];
            benar = true;
        }
    }
    if (benar) return 1;
    else return 0;
}

void isWordCorrect(){
    for (int i = 0; i < wordCount; i++){
        if (session.wordRan[i] == session.word[i] && session.word[i] != ""){
            session.wordID[i] = 0;
            session.enemyHP -= session.wordDmg[i];
            one.score += percentageWordCorrectScore * (session.word[i]).length(); cout << session.word[i].length();
            session.word[i] = session.wordRan[i] = "";
            session.selectedWord = -1;
            updateUI("score"); updateUI("selected"); updateUI("list"); updateUI("enemy");
            session.wordSize -= 1;
        }
    }
}

int findSpacesDash(string kata){
    int spacesordash = 0;
    for (int i = 0; i < kata.length(); i++){
        if (kata[i] == ' ' || kata[i] == '-') spacesordash++;
    } return spacesordash;
}

int loadData(string location, string arr[]){
    ifstream data(location);
    int i = 0;
    if (!data){
        missing(location, "F"); return -1;}
    else {
        while (data){
            string temp;
            getline(data, temp);
            arr[i] = temp; i++;
        }
    } data.close(); return i--;
}

int isRightNumber(int input){
    for (int i = 0; i < 8; i++)
        if (input == i+49) return 1;
    return 0;
}

void transition(){
    animate("./gambar/headstart.txt", 5, 1); system("cls");
    for (int i = 0; i < 5; i++) loadImage("./gambar/title2.txt", 5, 1, 0, '[', ']'); system("cls");
}

void instruction(){
    system("cls"); loadImage("./gambar/instruction.txt", 0, 0, 1);

    do isArrow = controller(&menuPointer.posX, &menuPointer.posY, 17, 11, 18, 11);
        while (isArrow != 1);
    fromOtherMenu = true;
}

int controller(int *posX, int *posY, int xPar1, int yPar1, int xPar2, int yPar2){
    int input;
    input = getch();
    switch(input){
        case 75: // left
            if (*posX > xPar1){(*posX)--; return 1;}
            break;
        case 77: // righ
            if (*posX < xPar2){(*posX)++; return 2;}
            break;
        case 72: // up
            if (*posY > yPar1){(*posY)--; return 3;}
            break;
        case 80: // down
            if (*posY < yPar2){(*posY)++; return 4;}
            break;
        default:
            return 0;
    }
}

int loadImage(string location, int posX, int posY, int mode, char xBorder, char yBorder){
    int xMax = findXMax(location, xBorder);
    int yMax = findYMax(location, yBorder);
    if (xMax == -1 || yMax == -1){missing(location, "F"); return -1;}
    else if (xMax == -2){missing(location, "HB"); return -1;}
    else if (yMax == -2){missing(location, "VB"); return -1;}

    ifstream imageData(location); if (!imageData){missing(location, "F"); imageData.close(); return -1;}
    string temp;
    for (int y = 0; y < yMax; y++){
        getline(imageData, temp);
        while(temp.length() < xMax){
            temp.resize(temp.length()+1);
            temp[temp.length()-1] = ' ';
        }
        if (mode == 0){
            for (int x = 0; x < xMax; x++){
                cout << temp[x];} cout << endl;}
        else
            for (int x = 0; x < xMax; x++){
                Gotoxy(x+posX, y+posY); cout << temp[x];}
    } imageData.close(); return 0;
}

void animate(string location, int posX, int posY, char xBorder, char yBorder){
    int xMax = findXMax(location, xBorder);
    int yMax = findYMax(location, yBorder);
    int borderMax = findBorderMax(location, yBorder);
    ifstream imageData(location);
    for (int i = 0; i < borderMax; i++){
        string temp;
        for (int y = 0; y < yMax; y++){
            getline(imageData, temp);
            while(temp.length() < xMax){
                temp.resize(temp.length()+1);
                temp[temp.length()-1] = ' ';
            } for (int x = 0; x < xMax; x++){Gotoxy(posX+x, posY+y); cout << temp[x];}
        } getline(imageData, temp); Sleep(stoi(temp.substr(1)));
    } cout << endl;
}

int random(int minimum, int maksimum){
    return rand() % (maksimum - minimum + 1) + minimum;
}

int findBorderMax(string location, char border){
    string temp; int borderMax = 0;
    ifstream imageData(location);
    if (!imageData){imageData.close(); return -1;}
    while(imageData){
        getline(imageData, temp);
        if (findIndex(temp, border) >= 0){
            borderMax++;
        }
    } imageData.close(); return borderMax;
}


int findXMax(string location, char border){
    string temp;
    ifstream imageData(location);
    if (!imageData){imageData.close(); return -1;}
    getline(imageData, temp);
    if (findIndex(temp, border) >= 0){
        imageData.close();
        return findIndex(temp, border);
    } imageData.close(); return -2;
}

int findYMax(string location, char border){
    string temp; int yMax = 0;
    ifstream imageData(location);
    if (!imageData){imageData.close(); return -1;}
    while(imageData){
        getline(imageData, temp);
        if (findIndex(temp, border) >= 0){
            imageData.close();
            return yMax;
        } yMax++;
    } imageData.close(); return -2;
}

int findIndex(string kata, char karakter){
    if (kata.find(karakter) != string::npos) return kata.find(karakter);
    else return -1;
}

int findNumber(int arr[], int size, int number){
    for (int i = 0; i < wordCount; i++){
        if (arr[i] == number){
            return i;
        }
    } return -1;
}

string changeCase(string teks, char to){
    if (to == 'u'){for (int i = 0; i < teks.length(); i++)teks[i] = toupper(teks[i]); return teks;}
    else if (to == 'l'){for (int i = 0; i < teks.length(); i++)teks[i] = tolower(teks[i]); return teks;}
    else {cout << " [Ada masalah pada pemakaian fungsi changeCase!]"; system("pause");}
}

void Gotoxy(int x, int y) {
    cursorposition.X = x;
    cursorposition.Y = y;
    SetConsoleCursorPosition(Console, cursorposition);
}

void SetCursor(bool visible, DWORD size) {
	if(size == 0) size = 20;
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(Console,&lpCursor);
}

void missing(string location, string problem){
    system("cls");
    if (problem == "F") cout << " [File \"" << location << "\" tidak ditemukan!]" << endl;
    else if (problem == "HB") cout << " [Ada masalah pada pembatas horizontal file \"" << location << "\"!]" << endl;
    else if (problem == "VB") cout << " [Ada masalah pada pembatas vertical file \"" << location << "\"!]" << endl;
    else cout << " [Ada masalah pada parameter fungsi 'missing'!]" << endl;
}
