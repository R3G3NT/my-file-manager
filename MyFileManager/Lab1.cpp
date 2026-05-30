#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <fstream>
#include <conio.h>

#include "Console.h"
#include "Lab1.h"
using namespace std;

void ShowMenuLab1()
{
    cout << "ESC Escape | ";
    cout << "F1 Show Chance | ";
    cout << "F2 Show Counts | ";
    cout << "F3 Show Graph" << endl << endl;
}

void ShowChance(unsigned long long bytes[], int amount, double border)
{
    for (int i = 0; i < 256; i++)
    {
        cout << hex << uppercase << setw(2) << setfill('0') << i << "    ";
        if ((i + 1) % 16 == 0)
        {
            cout << setfill(' ') << endl;
            for (int j = (i + 1) - 16; j < i + 1; j++)
            {
                double chance = round(((double)bytes[j] / amount) * 1000) / 1000;
                if (chance > border)
                    SetConsoleTextAttribute(hConsole, (LIGHT_GREEN << 4) | BLACK);
                cout << left << setw(5) << chance;
                if (chance > border)
                    SetConsoleTextAttribute(hConsole, (BLACK << 4) | LIGHT_GREY);
                cout << " ";
            }
            cout << endl << endl;
        }
    }
}

void ShowCounts(unsigned long long bytes[], int amount, double border)
{
    for (int i = 0; i < 256; i++)
    {
        cout << hex << uppercase << setw(2) << setfill('0') << i << "    ";
        if ((i + 1) % 16 == 0)
        {
            cout << setfill(' ') << endl;
            for (int j = (i + 1) - 16; j < i + 1; j++)
            {
                double chance = round(((double)bytes[j] / amount) * 1000) / 1000;
                if (chance > border)
                    SetConsoleTextAttribute(hConsole, (LIGHT_GREEN << 4) | BLACK);
                cout << left << setw(5) << dec << bytes[j];
                if (chance > border)
                    SetConsoleTextAttribute(hConsole, (BLACK << 4) | LIGHT_GREY);
                cout << " ";
            }
            cout << endl << endl;
        }
    }
}

void ShowGraph(unsigned long long bytes[], Graphs graph)
{
    int r = 10;
    for (int y = -r; y <= r; y++)
    {
        for (int x = -r * 2; x <= r * 2; x++)
        {
            bool isBorderCircle = abs((x / 2) * (x / 2) + y * y - r * r) < r;
            bool isInCircle = (x / 2) * (x / 2) + y * y < r* r;

            if (graph == FULL && isInCircle)
            {
                cout << "#";
            }
            else if (graph == EMPTY && isBorderCircle)
            {
                cout << "#";
            }
            else if (graph == HALF && (isBorderCircle || (isInCircle && x <= 0)))
            {
                cout << "#";
            }
            else if (graph == FIRST_HALF_HALF
                && (isBorderCircle || (isInCircle && (x <= 0 || x >= 0 && y >= 0))))
            {
                cout << "#";
            }
            else if (graph == SECOND_HALF_HALF
                && (isBorderCircle || (isInCircle && x >= 0 && y <= 0)))
            {
                cout << "#";
            }
            else
                cout << " ";
        }
        cout << endl;
    }
}

void ChooseGraph(unsigned long long bytes[], int amount)
{
    int amount_symbols = 0;
    for (int i = 0; i < 256; i++)
    {
        if ((i >= 'A' && i <= 'Z') || (i >= 'a' && i <= 'z') || (i >= '0' && i <= '9'))
            amount_symbols += bytes[i];
    }

    double occurrence = round((double)amount_symbols / amount * 100);
    cout << "Amount: " << dec << amount << endl;
    cout << "Amount of symbols: " << amount_symbols << endl;
    cout << "Occurrence: " << occurrence << "%" << endl << endl;

    if (100 - occurrence <= 10)
    {
        ShowGraph(bytes, FULL);
    }
    else if (75 - occurrence <= 10)
    {
        ShowGraph(bytes, FIRST_HALF_HALF);
    }
    else if (50 - occurrence <= 10)
    {
        ShowGraph(bytes, HALF);
    }
    else if (25 - occurrence <= 10)
    {
        ShowGraph(bytes, SECOND_HALF_HALF);
    }
    else
    {
        ShowGraph(bytes, EMPTY);
    }
}

void Events(unsigned long long bytes[], int amount, double border)
{
    while (true)
    {
        if (GetAsyncKeyState(VK_F1) & 0x8000)
        {
            system("cls");
            ShowMenuLab1();
            ShowChance(bytes, amount, border);
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_F2) & 0x8000)
        {
            system("cls");
            ShowMenuLab1();
            ShowCounts(bytes, amount, border);
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_F3) & 0x8000)
        {
            system("cls");
            ShowMenuLab1();
            ChooseGraph(bytes, amount);
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            system("cls");
            break;
        }
        Sleep(10);
    }
}

int StartLab1(string filePath)
{
    ifstream file;
    file.open(filePath, std::ios::binary);
    if (!file.is_open())
    {
        cerr << "Couldn't open the file!" << endl;
        cout << "Press 1 to return in MyFileManager :)" << endl;
        while (true)
        {
            char ch = _getch();
            if (ch == '1')
                break;
        }
        SetCursorBlinking(false);
        return -1;
    }

    double border = 0;
    cout << "Enter border: ";
    string borderArg;
    cin >> borderArg;
    replace(borderArg.begin(), borderArg.end(), '.', ',');
    border = stod(borderArg);

    SetCursorBlinking(false);
    system("cls");

    unsigned long long bytes[256] = {};
    char ch;
    int amount = 0;
    while (file.get(ch))
    {
        auto ch_code = (unsigned char)ch;
        bytes[ch_code]++;
        amount++;
    }
    file.close();

    ShowMenuLab1();
    ShowChance(bytes, amount, border);
    Events(bytes, amount, border);

    cout << dec;

    return 0;
}
