#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Text.h"
#include <list>
#include <conio.h>
#include <windows.h>
#include <regex>

using namespace std;
//Проблема с чтениемм файла recurs ...


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO cbsi;



void clearRow(short row) {
    SetConsoleCursorPosition(hConsole, { 0,row });
    cout << "                                                              ";
}

string getLine(Text::iterator& it) {
    string temp;
    for (int i = 0; i < it.tab(); i++) temp += "   ";
    temp += *it;
    return temp;
}

void draft(COORD position, Text& temp) {
    system("cls");
    cout << temp;
    SetConsoleCursorPosition(hConsole, position);
}

enum keyboard {
    Delete = 8,
    Enter = 13,
    Esc = 27,
    Up = 72,
    Down = 80,
    Left = 75,
    Right = 77,
    Start = 126,
    ReStart = 96,
    Save = 19,
    Go = -32
};


bool isChar(char c) {
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        (c >= '0' && c <= '9') ||
        c == ' ' || c == '.' || c == ',' || c == ':' || c == ';' ||
        c == '=' || c == '-' || c == '+' || c == '*' || c == '/' || 
        c == '<' || c == '>' || c == '(' || c == ')' || c == '!' ||
        c == '"';
}


int main() {
	//setlocale(LC_ALL, "Rus");

	Text temp;
	temp.readFile("C:\\Users\\fill2\\OneDrive\\Рабочий стол\\temp.txt");
	cout << temp;
    Text::iterator it = temp.begin();

    COORD position = { 0, 0};
    SetConsoleCursorPosition(hConsole, position);
    
    string teckString = getLine(it);
    bool key_go = false;

    char c;
    do {
        c = _getch();
        if (c == keyboard::Go) {
            key_go = true;
            c = _getch();
        }
        if (c == keyboard::Left && key_go) {
            position.X--;
        }
        else if (c == keyboard::Right && key_go) {
            position.X++;
        }
        else if (c == keyboard::Up && key_go && position.Y > 0){
            removSpace(teckString);
            (*it) = teckString;
            position.Y--;
            it--;
            teckString.clear();
            teckString = getLine(it);
        }
        else if (c == keyboard::Down && key_go && it != temp.end()){
            removSpace(teckString);
            (*it) = teckString;
            position.Y++;
            it++;
            teckString.clear();
            teckString = getLine(it);
        }
        else if (c == keyboard::Enter) {
            removSpace(teckString);
            (*it) = teckString;
            
            if (temp.ifEndWord(teckString) && temp.endUp(it)) {
                it.endUp();
            }
            

            temp.add(it);
            position.Y++;
            teckString.clear();
            teckString = getLine(it);
            position.X = teckString.size();
            draft(position, temp);
        }
        else if (c == keyboard::Delete && position.X > 0 && position.X <= teckString.size()) {
            teckString.erase(position.X - 1, 1);
            clearRow(position.Y);
            SetConsoleCursorPosition(hConsole, { 0,position.Y });
            cout << teckString;
            position.X--;
        }
        else  if (c == keyboard::Start) {
            removSpace(teckString);
            (*it) = teckString;
            for (int i = 0; i < 60; i++) cout << endl;
            position = { 0, 0 };
            SetConsoleCursorPosition(hConsole, position);
            temp.Start();
        }
        else if (c == keyboard::ReStart) {
            for (int i = 0; i < 60; i++) cout << endl;
            position = { 0, 0 };
            SetConsoleCursorPosition(hConsole, position);
            it = temp.begin();
            teckString.clear();
            teckString = getLine(it);
            draft(position, temp);
            clearRow(position.Y);
            SetConsoleCursorPosition(hConsole, { 0,position.Y });
            cout << teckString;
        }
        else if (c == keyboard::Save) {
            temp.writeFile("C:\\Users\\fill2\\OneDrive\\Рабочий стол\\temp.txt");
        }
        else {
            if (!key_go && isChar(c) && position.X <= teckString.size()) {
                auto p = teckString.begin();
                for (int i = 0; i < position.X; i++)p++;
                teckString.insert(p, c);
                SetConsoleCursorPosition(hConsole, { 0,position.Y });
                cout << teckString;
                position.X++;
            }
        }
            

        if (position.X < 0) position.X = 0;
        SetConsoleCursorPosition(hConsole, position);
        key_go = false;
    } while (c != Esc);

    system("pause");
    return 0;

	
    

	
}