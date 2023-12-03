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
COORD Cursorposition;

void play(string userid,int skor); void transition(); void instruction(); void option(); void credit();
void powerUP(int input, int type);
void updateUI(string section);
int loadData(string location, string arr[]);
void spawnEnemy(); void enemyAttack(); void giftFromEnemy(); void playerDied(string userid);
void wordAssigner(); int compareWord(char input); void isWordCorrect(); void addNotification(int xModifier, string teks); void cleanNotification();
int isRightNumber(int input);
int controller(int *posX, int *posY, int xPar1, int yPar1, int xPar2, int yPar2);
int findSpacesDash(string kata);
void Gotoxy(int x, int y);
void SetCursor(bool visible, DWORD size);
string changeCase(string teks, char to);
int loadImage(string location, int posX, int posY, char xBorder = 'z', char yBorder = ';');
void animate(string location, int posX, int posY, char xBorder = '"', char yBorder = ';');
int random(int minimum, int maksimum);
int filterNumber(string teks, char pos);
int findNumber(int arr[], int size, int number);
int findBorderMax(string location, char border);
int findXMax(string location, char border);
int findYMax(string location, char border);
int findIndex(string kata, char karakter);
void missing(string location, string problem);

int xPosition = 0;
int yPosition = 0;

const int wordCount = 8;

int isArrow; bool fromOtherMenu;
int sleepCount = 0;
int wordSpawnDelay = 500;
int powerUP1Delay = 10000;
int powerUP2Delay = 30000;
int notificationDelay;
int damageFromWrongGuess = 1;
float percentageEnemyDiedScore = 0.8;
float percentageWordCorrectScore = 0.8;
float percentageEnemyDiedHP = 0.5;
float minPercenWordRanValue = 0.4, maxPercenWordRanValue = 0.6;
int minEnemyAtkSpdRanValue = 3, maxEnemyAtkSpdRanValue = 7;
int minEnemyDmgRanValue = 5, maxEnemyDmgRanValue = 15;

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
    bool enemyIsAttacking = false, enemyIsHit = false;
    string idle[10] = {"./gambar/idle/frame1.txt", "./gambar/idle/frame2.txt", "./gambar/idle/frame3.txt", "./gambar/idle/frame4.txt", "./gambar/idle/frame4.txt",
                       "./gambar/idle/frame3.txt", "./gambar/idle/frame2.txt", "./gambar/idle/frame1.txt", "./gambar/idle/frame5.txt"};
    string enemyAttacking[3] = {"./gambar/attack/dans1.txt", "./gambar/attack/dans2.txt", "./gambar/attack/dans3.txt"};
};

struct pointer{
    int posX = 48, posY = 12;
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
        if (fromOtherMenu) system("cls"); system("color 07");
        loadImage("./gambar/title.txt", 35 + xPosition, 1 + yPosition, '[', ']');
        loadImage("./gambar/menuOption.txt", 47 + xPosition, 10 + yPosition, 'z', ';');
        fromOtherMenu = false;

        Gotoxy(menuPointer.posX, menuPointer.posY); cout << ">";
        do isArrow = controller(&menuPointer.posX, &menuPointer.posY, 48 + xPosition, 10 + yPosition, 49 + xPosition, 14 + yPosition);
            while (isArrow == 0);
        if (menuPointer.posY == 10 + yPosition && isArrow == 2){if (first) transition(); play(userid,skor);}
        else if (menuPointer.posY == 11 + yPosition && isArrow == 2) option();
        else if (menuPointer.posY == 12 + yPosition && isArrow == 2) instruction();
        else if (menuPointer.posY == 13 + yPosition && isArrow == 2) credit();
        else if (menuPointer.posY == 14 + yPosition && isArrow == 2){system("cls"); return 1;}
    }
}

void play(string userid,int skor){
    int iters = 0;
    loadImage("./gambar/UI.txt", 30 + xPosition, 0 + yPosition);
    Gotoxy(42 + xPosition, 19 + yPosition); system("pause");
    if (first){spawnEnemy(); first = false;}
    updateUI("selected"); updateUI("sane"); updateUI("score"); updateUI("powerup"); updateUI("list");
    while(1){
        if (!session.enemyIsAttacking) system("color 07");
        isWordCorrect();

        if (session.enemyHP <= 0){
            giftFromEnemy(); spawnEnemy(); session.enemyTime = 0;
            cleanNotification();
            addNotification(10, "You defeated the enemy!"); notificationDelay = 1000;}

        if (kbhit()){
            int input = getch();
            if (input == 75){
                powerUP(input, 1); updateUI("powerup");}
            else if (input == 77){
                powerUP(input, 2); updateUI("powerup");}
            else if (input == 13){
                system("color 07");
                fromOtherMenu = true;
                menuPointer.posX = 47 + xPosition;
                menuPointer.posY = 10 + yPosition;
                break;}
            else if (isRightNumber(input) != 0){
                session.selectedWord = input - 49;
                updateUI("selected");}
            else if (session.selectedWord != -1 && isalpha(input)){
                if (compareWord(input) == 0){
                    one.sane -= damageFromWrongGuess; updateUI("sane");
                } updateUI("selected");
            }
        }

        Sleep(50); sleepCount += 50; session.enemyTime += 50;

        if (session.enemyTime == session.enemyAtkSpeed + 100){enemyAttack(); session.enemyIsAttacking = true; iters = 0;}
        else if (session.enemyTime >= session.enemyAtkSpeed + 100) session.enemyTime = 0;
        else if (session.enemyTime % 1000){
            Gotoxy(82 + xPosition, 27 + yPosition); cout << 0;
            Gotoxy(83 + xPosition, 27 + yPosition); cout << (1000 + session.enemyAtkSpeed-session.enemyTime)/1000;
        }

        if (session.enemyIsAttacking){
            if (iters > 2){iters = 0; session.enemyIsAttacking = false;}
            system("color 40");
            loadImage(session.enemyAttacking[iters], 31 + xPosition, 1 + yPosition, 'x', ';');
        } else if (session.enemyIsHit){
            if (iters > 3){iters = 0; session.enemyIsHit = false;}
            loadImage("./gambar/knock/knock.txt", 31 + xPosition, 1 + yPosition, 'x', ';');
        } else {
            if (iters > 8) iters = 0;
            loadImage(session.idle[iters], 31 + xPosition, 1 + yPosition, 'x', ';');
        } iters++; updateUI("enemy");

        if (sleepCount % wordSpawnDelay == 0){
            wordAssigner(); updateUI("list");}

        if (sleepCount % powerUP1Delay == 0){
            one.powerUP1 += 1;
            cleanNotification();
            addNotification(4, "You got one 'Exterminate One' power!");
            notificationDelay = 1000;
            if (one.powerUP1 > 5) one.powerUP1 = 5; updateUI("powerup");
        }
        if ((sleepCount % powerUP2Delay == 0) && one.powerUP1 >= 2){
            one.powerUP1 -= 2;
            one.powerUP2 += 1;
            cleanNotification();
            addNotification(4, "You got one 'Exterminate All' power!");
            notificationDelay = 1000;
            if (one.powerUP2 > 2) one.powerUP2 = 2; updateUI("powerup");
        }

        if (notificationDelay > 0) notificationDelay -= 50;
        else if (notificationDelay <= 0) cleanNotification();

        if (one.sane > 100){
            one.sane = 100;
            updateUI("sane");}
        else if (one.sane <= 0){
            one.sane = 0;
            updateUI("sane");
            playerDied(userid);
            break;
        }
    }
}

void powerUP(int input, int type){
    cleanNotification();
    if (type == 1){
        if (one.powerUP1 > 0){
            addNotification(6, "Pilih kata yang ingin dihapus!");
            input = getch();
            if (isRightNumber(input) != 0 && session.wordID[input-49] != NULL){
                session.wordRan[input-49] = session.word[input-49];
                one.powerUP1 -= 1;
                cleanNotification();}
            else {
                addNotification(6, "Kata yang ingin dihapus tidak ada!");
            }
        } else {
            addNotification(1, "You don't have any 'Exterminate One' power!");
        }
    } else if (type == 2){
        if (one.powerUP2 > 0){
            for (int i = 0; i < wordCount; i++){
                session.wordRan[i] = session.word[i];
            } one.powerUP2 -= 1;
        } else {
            addNotification(1, "You don't have any 'Exterminate All' power!");
        }
    } notificationDelay = 1000;
}

void updateUI(string section){
    section = changeCase(section, 'l');
    if (section == "score"){
        if (one.score < 10000){Gotoxy(72 + xPosition, 27 + yPosition); cout << setw(3) << one.score;}
        else {Gotoxy(70 + xPosition, 27 + yPosition); cout << setw(6) << one.score;}
    } else if (section == "enemy"){
        Gotoxy(72 + xPosition, 8 + yPosition); cout << "Enemy's HP";
        Gotoxy(72 + xPosition, 7 + yPosition); cout << "    ";
        Gotoxy(72 + xPosition, 7 + yPosition); cout << setw(5) << session.enemyHP << "/" << session.enemyMHP;
    } else if (section == "sane"){
        Gotoxy(59 + xPosition, 27 + yPosition); cout << setw(4) << one.sane << "    ";
        Gotoxy(59 + xPosition, 27 + yPosition); cout << setw(4) << one.sane << "%";
    } else if (section == "powerup"){
        Gotoxy(52 + xPosition, 21 + yPosition); cout << one.powerUP1;
        Gotoxy(79 + xPosition, 21 + yPosition); cout << one.powerUP2;
    } else if (section == "selected"){
        Gotoxy(72 + xPosition, 23 + yPosition); cout << "          ";
        Gotoxy(62 + xPosition, 24 + yPosition); cout << "                    ";
        if (session.selectedWord == -1 || findNumber(session.wordID, wordCount, session.selectedWord+1) == -1){
            session.selectedWord = -1;
            Gotoxy(72 + xPosition, 23 + yPosition); cout << "NONE";
        } else {
            Gotoxy(72 + xPosition, 23 + yPosition); cout << session.wordType[session.selectedWord];
            Gotoxy(62 + xPosition, 24 + yPosition); cout << session.selectedWord + 1 << ". " << session.wordRan[session.selectedWord];
        }
    } else if (section == "list"){
        for (int i = 0; i < wordCount; i++){
            if (i < 4){
                Gotoxy(32 + xPosition, 23 + i + yPosition);
                if (findNumber(session.wordID, wordCount, i+1) != -1){cout << i+1 << ". " << session.wordType[i]; Gotoxy(41 + xPosition, 23 + i + yPosition); cout << session.wordDmg[i];}
                else cout << "            ";
            }
            else {
                Gotoxy(45 + xPosition, 19 + i + yPosition);
                if (findNumber(session.wordID, wordCount, i+1) != -1){cout << i+1 << ". " << session.wordType[i]; Gotoxy(54 + xPosition, 19 + i + yPosition); cout << session.wordDmg[i];}
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

    session.enemyDmg = random(minEnemyDmgRanValue, maxEnemyDmgRanValue);
    session.enemyAtkSpeed = random(minEnemyAtkSpdRanValue, maxEnemyAtkSpdRanValue) * 1000;
}

void enemyAttack(){
    one.sane -= session.enemyDmg; updateUI("sane");
    cleanNotification();
    addNotification(3, "You took damage from the enemy attack!");
    notificationDelay = 500;
}

void giftFromEnemy(){
    one.score += percentageEnemyDiedScore * session.enemyMHP + (session.enemyAtkSpeed/1000) * 0.5;
    one.sane += percentageEnemyDiedHP * session.enemyMHP + (session.enemyAtkSpeed/1000) * 0.5;
    updateUI("score"); updateUI("sane");
}

void playerDied(string userid){
    menuPointer.posX = 47 + xPosition;
    menuPointer.posY = 10 + yPosition;
    session.enemyIsHit = false; session.enemyIsAttacking = false;
    updatedata(userid,one.score);
    one.score = 0; one.sane = 100; one.powerUP1 = one.powerUP2 = 0;
    for (int i = 0; i < wordCount; i++) session.wordID[i] = 0;
    session.wordSize = 0; sleepCount = 0;
    first = true; fromOtherMenu = true;
    cleanNotification();
    Gotoxy(32 + xPosition, 17 + yPosition); cout << "lol you died...";
    Gotoxy(42 + xPosition, 19 + yPosition); system("pause");
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
        int realLength = session.word[index].length()-findSpacesDash(session.word[index]);
        int letterRandomizedCount = random(minPercenWordRanValue * realLength, maxPercenWordRanValue * realLength);
        char letterRandomized[1000]; for (int i = 0; i < 1000; i++) letterRandomized[i] = NULL;
        int letterIndex;
        for (int i = 0; i < letterRandomizedCount; i++){
            bool sama;
            do {
                sama = false;
                letterIndex = random(0, realLength-1);
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
            one.score += percentageWordCorrectScore * (session.word[i]).length();
            session.word[i] = session.wordRan[i] = "";
            session.selectedWord = -1;
            session.enemyIsHit = true;
            updateUI("score"); updateUI("selected"); updateUI("list");
            session.wordSize -= 1;
            cleanNotification();
            addNotification(3, "The enemy took damage from you attack!"); notificationDelay = 500;
        }
    }
}

void addNotification(int xModifier, string teks){
    Gotoxy(36 + xModifier + xPosition, 19 + yPosition); cout << teks;
}

void cleanNotification(){
    Gotoxy(36 + xPosition, 19 + yPosition); cout << "                                              ";
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
    animate("./gambar/oneFileAnim/fadedTitle.txt", 35 + xPosition, 1 + yPosition);
    int c = 0;
    for (int i = 0; i < 10; i++){
        loadImage("./gambar/title2.txt", 30 + xPosition, c + yPosition, '[', ']'); c+=15;} system("cls");
}

void instruction(){
    system("cls"); loadImage("./gambar/instruction.txt", 0 + xPosition, 0 + yPosition);

    do isArrow = controller(&menuPointer.posX, &menuPointer.posY, 47 + xPosition, 12 + yPosition, 48 + xPosition, 12 + yPosition);
        while (isArrow != 1);
    fromOtherMenu = true;
}

void option(){
    bool breakOut = false;
    while(1){
        system("cls");
        loadImage("./gambar/title.txt", 35 + xPosition, 1 + yPosition, '[', ']');
        loadImage("./gambar/option.txt", 35 + xPosition, 10 + yPosition, 'z', ';');
        Gotoxy(38 + xPosition, 11 + yPosition); cout << xPosition;
        Gotoxy(38 + xPosition, 12 + yPosition); cout << yPosition;
        while(1){
            int input = getch();
            string temp;
            if (tolower(input) == 'y' || tolower(input) == 'x'){
                Gotoxy(35 + xPosition, 14 + yPosition); cout << "Masukkan Nilai Untuk Merubah Posisinya.";
                Gotoxy(35 + xPosition, 15 + yPosition); cout << "Tekan ENTER untuk konfirmasi perubahan!";
                if (tolower(input) == 'x'){
                    Gotoxy(38 + xPosition, 11 + yPosition); cout << "   ";
                    Gotoxy(38 + xPosition, 11 + yPosition); cin >> temp;
                    xPosition = filterNumber(temp, 'x');
                } else if (tolower(input) == 'y'){
                    Gotoxy(38 + xPosition, 12 + yPosition); cout << "   ";
                    Gotoxy(38 + xPosition, 12 + yPosition); cin >> temp;
                    yPosition = filterNumber(temp, 'y');
                } break;
            } else if (input == 75){
                breakOut = true;
                break;}
        } if (breakOut) break;
    }
    fromOtherMenu = true;
    menuPointer.posX = 47 + xPosition;
    menuPointer.posY = 11 + yPosition;
}

void credit(){
    system("cls");
    loadImage("./gambar/title.txt", 35 + xPosition, 1 + yPosition, '[', ']');
    loadImage("./gambar/credit.txt", 35 + xPosition, 10 + yPosition, '"', ';');
     do isArrow = controller(&menuPointer.posX, &menuPointer.posY, 47 + xPosition, 12 + yPosition, 48 + xPosition, 12 + yPosition);
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

int loadImage(string location, int posX, int posY, char xBorder, char yBorder){
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

int filterNumber(string teks, char pos){
    pos = tolower(pos);
    int length = teks.length();
    string number; int iteration = 0;
    for (int i = 0; i < length; i++){
        if (isdigit(teks[i]) || teks[i] == '-'){
            if (teks[i] == '-' && iteration > 0) continue;
            iteration++;
            number.resize(iteration);
            number[iteration-1] = teks[i];
        }
    } if (iteration > 0){
        if (number.length() > 3) number.resize(3);
        return stoi(number);}

    if (pos == 'y') return yPosition;
    else return xPosition;
}

string changeCase(string teks, char to){
    if (to == 'u'){for (int i = 0; i < teks.length(); i++)teks[i] = toupper(teks[i]); return teks;}
    else if (to == 'l'){for (int i = 0; i < teks.length(); i++)teks[i] = tolower(teks[i]); return teks;}
    else {cout << " [Ada masalah pada pemakaian fungsi changeCase!]"; system("pause");}
}

void Gotoxy(int x, int y) {
    Cursorposition.X = x;
    Cursorposition.Y = y;
    SetConsoleCursorPosition(Console, Cursorposition);
}

void SetCursor(bool visible, DWORD size){
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
