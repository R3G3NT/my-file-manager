#include <iostream>
#include <iomanip>
#include <conio.h>
#include <filesystem>

#include "Console.h"
#include "WAVHeader.h"
#include "Lab2.h"
using namespace std;
namespace fs = filesystem;

void GetCommand(string& command)
{
    cout << "Encode -> 1\n";
    cout << "Decode -> 2\n";
    cout << "Enter command: ";
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '1')
        {
            command = "-e";
            cout << ch << '\n';
            break;
        }
        else if (ch == '2')
        {
            command = "-d";
            cout << ch << '\n';
            break;
        }
    }
}

void Encode(uint32_t size, vector<char> buffer, string filePath)
{
    for (int i = 0; i <= 9; ++i)
    {
        cout << "\rProgress: " << i * 10 << "%";
        Sleep(200);
    }

    WAVHeader header;
    SetWAVHeader(header, size);

    fs::path fP = filePath;
    vector<char> fPextension; //Расширение файла
    for (char ch : fP.extension().string())
        fPextension.push_back(ch);

    string newfP = fP.parent_path().string() + "\\" + fP.stem().string() + ".wav";
    ofstream outFile(newfP, std::ios::binary);
    if (outFile.is_open())
    {
        outFile.write(reinterpret_cast<const char*>(&header), sizeof(WAVHeader));
        if (fPextension.size() < 10)
        {
            while (fPextension.size() < 10)
                fPextension.push_back(' ');
        }
        outFile.write(fPextension.data(), fPextension.size());
        outFile.write(buffer.data(), buffer.size());
        outFile.close();
        fs::remove(filePath);
        cout << "\rProgress: " << 100 << "%" << endl;
        cout << "Successful encoded!" << endl;
    }
    else
    {
        cout << endl;
        cerr << "Error encoding!" << endl;
    }
}

void Decode(uint32_t size, vector<char> buffer, string filePath)
{
    for (int i = 0; i <= 9; ++i)
    {
        cout << "\rProgress: " << i * 10 << "%";
        Sleep(200);
    }

    fs::path fP = filePath;
    string newfP = fP.parent_path().string() + "\\" + fP.stem().string();
    for (int i = 0; i < 10; i++)
    {
        if (buffer[i] != ' ')
            newfP += buffer[i];
    }
    ofstream outFile(newfP, std::ios::binary);
    if (outFile.is_open())
    {
        outFile.write(&buffer[10], buffer.size() - 10);
        outFile.close();
        fs::remove(filePath);
        cout << "\rProgress: " << 100 << "%" << endl;
        cout << "Successful decoded!" << endl;
    }
    else
    {
        cout << endl;
        cerr << "Error decoding!" << endl;
    }
}

void WaitStart(string command)
{
    string message = "Press 1 to start ";
    if (command == "-e" || command == "-encode")
        message += "encoding";
    else
        message += "decoding";
    cout << message << endl;

    while (true)
    {
        char ch = _getch();
        if (ch == '1')
        {
            break;
        }
    }
}

void ReadBytes(string command, ifstream& file, string filePath)
{
    if (command == "-e" || command == "-encode")
    {
        //Определяем размер файла по текущей позиции указателя
        uint32_t size = file.tellg();
        vector<char> buffer(size);
        //Перемещаем указатель обратно в начало
        file.seekg(0, std::ios::beg);

        if (file.read(buffer.data(), size))
            cout << "Successful readed " << size << " bytes!" << endl;
        else
            cerr << "Error reading bytes!";
        file.close();

        WaitStart(command);
        Encode(size, buffer, filePath);
    }
    else
    {
        uint32_t size = (uint32_t)file.tellg() - sizeof(WAVHeader);
        vector<char> buffer(size);
        file.seekg(sizeof(WAVHeader), ios::beg);

        if (file.read(buffer.data(), size))
            cout << "Successful readed " << size << " bytes!" << endl;
        else
            cerr << "Error reading bytes!";
        file.close();

        WaitStart(command);
        Decode(size, buffer, filePath);
    }
}

int StartLab2(string filePath)
{
    ifstream file;
    file.open(filePath, std::ios::binary | std::ios::ate);
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

    string command;
    GetCommand(command);

    SetCursorBlinking(false);

    ReadBytes(command, file, filePath);

    cout << "Press 1 to return in MyFileManager :)" << endl;
    while (true)
    {
        char ch = _getch();
        if (ch == '1')
            break;
    }
    SetCursorBlinking(false);

    return 0;
}
