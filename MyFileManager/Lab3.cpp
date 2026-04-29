#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <fstream>
#include <conio.h>
#include <filesystem>

#include "Console.h"
#include "Lab3.h"
using namespace std;
namespace fs = filesystem;

void WaitStart()
{
    cout << "Press 1 to start " << endl;
    while (true)
    {
        char ch = _getch();
        if (ch == '1')
        {
            break;
        }
    }
}

void SetHz(map<int, int>& Hz)
{
    Hz[50] = 0;
    Hz[100] = 0;
    Hz[200] = 0;
    Hz[500] = 0;
    Hz[1000] = 0;
    Hz[2000] = 0;
    Hz[5000] = 0;
    Hz[10000] = 0;
    Hz[20000] = 0;
}

void GetHz(map<int, int>& Hz, vector<char>& buffer, WAVHeader& header, int second)
{
    for (int i = header.byteRate * second; i < header.byteRate * (second + 1); i++)
    {
        int valHz = (unsigned char)buffer[i] * (20000 / 256);
        if (valHz - 50 < 10)
            Hz[50] += 1;
        else if (valHz - 100 < 10)
            Hz[100] += 1;
        else if (valHz - 200 < 10)
            Hz[200] += 1;
        else if (valHz - 500 < 10)
            Hz[500] += 1;
        else if (valHz - 1000 < 10)
            Hz[1000] += 1;
        else if (valHz - 2000 < 10)
            Hz[2000] += 1;
        else if (valHz - 5000 < 10)
            Hz[5000] += 1;
        else if (valHz - 10000 < 10)
            Hz[10000] += 1;
        else if (valHz - 20000 < 10)
            Hz[20000] += 1;
    }
}

void DrawHistogram(map<int, int>& Hz, int amountHz)
{
    cout << endl;
    SetConsoleTextAttribute(hConsole, (LIGHT_GREEN << 4) | BLACK);
    int j = 0;
    for (const auto& pair : Hz)
    {
        int numRows = 7 - round((double)pair.second / ((double)amountHz / 7));
        COORD cPos;
        cPos.Y = 0;
        cPos.X = 6 * j;
        for (int i = 0; i < 7; i++)
        {
            cPos.Y++;
            SetConsoleCursorPosition(hConsole, cPos);
            if (i >= numRows)
                cout << "     " << endl;
        }
        j++;
    }
    SetConsoleTextAttribute(hConsole, (BLACK << 4) | LIGHT_GREY);
    cout << " 50    100   200   500  1000  2000  5000  10000 20000" << endl;
}

int DrawSeconds(vector<char>& buffer, WAVHeader& header)
{
    int duration = (double)header.subchunk2Size / header.byteRate;
    int second = 0;
    while (true)
    {
        cout << second + 1 << " second (-1 <-) (-> +1) (ESC)" << endl;
        map<int, int> Hz;
        SetHz(Hz);
        GetHz(Hz, buffer, header, second);
        int amountHz = 0;
        for (const auto& pair : Hz)
            amountHz += pair.second;
        DrawHistogram(Hz, amountHz);
        cout << endl;
        for (const auto& pair : Hz)
            cout << setw(5) << pair.first << " Hz = " << pair.second << endl;
        cout << endl << "Amount Hz = " << amountHz << endl;

        while (true)
        {
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                if (second == 0)
                    second = duration - 1;
                else
                    second--;
                Sleep(150);
                break;
            }
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                if (second == duration - 1)
                    second = 0;
                else
                    second++;
                Sleep(150);
                break;
            }
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
                return 0;
            Sleep(10);
        }
        system("cls");
    }
}

int StartLab3(string filePath)
{
    ifstream file;
    file.open(filePath, std::ios::binary);
    fs::path fP = filePath;
    if (!file.is_open())
    {
        cerr << "Couldn't open the file!" << endl;
        cout << "Press 1 to return in MyFileManager :)" << endl;
        while (true)
        {
            char ch = _getch();
            if (ch == '1')
                return -1;
        }
    }
    else if (fP.extension().string() != ".wav")
    {
        cerr << "File is not wav!" << endl;
        file.close();
        cout << "Press 1 to return in MyFileManager :)" << endl;
        while (true)
        {
            char ch = _getch();
            if (ch == '1')
                return -1;
        }
    }

    WAVHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));
    file.seekg(0, std::ios::end);
    //Определяем размер файла по текущей позиции указателя
    uint32_t size = (uint32_t)file.tellg() - sizeof(WAVHeader);
    vector<char> buffer(size);
    file.seekg(sizeof(WAVHeader), std::ios::beg);

    if (file.read(buffer.data(), size))
        cout << "Successful readed " << size << " bytes!" << endl;
    else
        cerr << "Error reading bytes!";
    file.close();

    cout << endl;
    PrintHeaderInfo(header);
    WaitStart();
    system("cls");
    SetCursorBlinking(false);
    DrawSeconds(buffer, header);

    cout << "Press 1 to return in MyFileManager :)" << endl;
    while (true)
    {
        char ch = _getch();
        if (ch == '1')
            break;
    }

    return 0;
}