#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <filesystem>

#include "Console.h"
#include "Lab1.h"
#include "Lab2.h"
#include "Lab3.h"
#include "Lab4.h"
using namespace std;
namespace fs = filesystem;

short correctionNum = 4;

string GetFileSize(unsigned long long fileSize)
{
    if (fileSize / (1024 * 1024 * 1024) != 0)
        return format("{} ГБ", fileSize / (1024 * 1024 * 1024));
    if (fileSize / (1024 * 1024) != 0)
        return format("{} MБ", fileSize / (1024 * 1024));
    if (fileSize / 1024 != 0)
        return format("{} КБ", fileSize / 1024);
    return format("{}  Б", fileSize);
}

void ShowFastMenu()
{
    cout << "+------------------+" << endl;
    cout << "|  Choose folder:  |" << endl;
    cout << "|  1. Desktop      |" << endl;
    cout << "|  2. Pictures     |" << endl;
    cout << "|  3. Documents    |" << endl;
    cout << "|  4. Downloads    |" << endl;
    cout << "|  5. Music        |" << endl;
    cout << "|  6. Videos       |" << endl;
    cout << "|  7. C:\\          |" << endl;
    cout << "+------------------+" << endl;
}

void OpenFolderMenu(fs::path& dir_path)
{
    char choice;
    while (true)
    {
        choice = _getch();
        if (choice == '1')
        {
            dir_path = "C:\\Users\\User\\Desktop";
            break;
        }
        else if (choice == '2')
        {
            dir_path = "C:\\Users\\User\\Pictures";
            break;
        }
        else if (choice == '3')
        {
            dir_path = "C:\\Users\\User\\Documents";
            break;
        }
        else if (choice == '4')
        {
            dir_path = "C:\\Users\\User\\Downloads";
            break;
        }
        else if (choice == '5')
        {
            dir_path = "C:\\Users\\User\\Music";
            break;
        }
        else if (choice == '6')
        {
            dir_path = "C:\\Users\\User\\Videos";
            break;
        }
        else if (choice == '7')
        {
            dir_path = "C:\\";
            break;
        }
    }
}

int GetAmountFiles(fs::path dir_path)
{
    int amount = 0;
    try
    {
        for (const auto& entry : fs::directory_iterator(dir_path))
            amount++;
    }
    catch (...)
    {
        return 0;
    }
    return amount;
}

void ShowFiles(fs::path dir_path, int choiceFile)
{
    int i = choiceFile;
    try
    {
        for (const auto& entry : fs::directory_iterator(dir_path))
        {
            string fileName = entry.path().filename().string();
            if (fileName.length() > 51)
            {
                if (entry.is_directory())
                    fileName = fileName.substr(0, 44) + "...";
                else
                    fileName = fileName.substr(0, 48) + "...";
            }

            if (i == choiceFile)
                SetConsoleTextAttribute(hConsole, (LIGHT_GREEN << 4) | BLACK);

            if (!entry.is_directory())
            {
                cout << left << setw(52) << fileName;
                cout << right << setw(8) << GetFileSize(entry.file_size()) << endl;
            }
            else
            {
                fileName = "[F] " + fileName;
                cout << left << setw(52) << fileName;
                cout << setw(8) << " " << endl;
            }

            if (i == choiceFile)
                SetConsoleTextAttribute(hConsole, (BLACK << 4) | LIGHT_GREY);
            i++;
        }
    }
    catch (const fs::filesystem_error& e)
    {
        cerr << "Error reading directory: " << endl;
        cerr << e.what() << endl;
    }
}

void ChangeCurrentFile(fs::path dir_path, int userChoice, int previousUserChoice)
{
    string previousFile;
    bool isDirectoryPreviousFile = false;
    string previousFileSize = "";

    string currentFile;
    bool isDirectoryCurrentFile = false;
    string currentFileSize = "";

    int i = 0;
    for (const auto& entry : fs::directory_iterator(dir_path))
    {
        string fileName = entry.path().filename().string();
        if (i == previousUserChoice)
        {
            if (fileName.length() > 51)
            {
                if (entry.is_directory())
                    fileName = fileName.substr(0, 44) + "...";
                else
                    fileName = fileName.substr(0, 48) + "...";
            }
            if (entry.is_directory())
            {
                isDirectoryPreviousFile = true;
                fileName = "[F] " + fileName;
            }
            previousFile = fileName;
            previousFileSize = GetFileSize(entry.file_size());
        }
        else if (i == userChoice)
        {
            if (fileName.length() > 51)
            {
                if (entry.is_directory())
                    fileName = fileName.substr(0, 44) + "...";
                else
                    fileName = fileName.substr(0, 48) + "...";
            }
            if (entry.is_directory())
            {
                isDirectoryCurrentFile = true;
                fileName = "[F] " + fileName;
            }
            currentFile = fileName;
            currentFileSize = GetFileSize(entry.file_size());
        }
        i++;
    }

    COORD cPos{ 0, previousUserChoice + correctionNum };
    SetConsoleCursorPosition(hConsole, cPos);
    cout << left << setw(52) << previousFile;
    if (!isDirectoryPreviousFile)
        cout << right << setw(8) << previousFileSize;
    else
        cout << right << setw(8) << " ";

    cPos.Y = userChoice + correctionNum;
    SetConsoleCursorPosition(hConsole, cPos);
    SetConsoleTextAttribute(hConsole, (LIGHT_GREEN << 4) | BLACK);
    cout << left << setw(52) << currentFile;
    if (!isDirectoryCurrentFile)
        cout << right << setw(8) << currentFileSize;
    else
        cout << right << setw(8) << " ";
    SetConsoleTextAttribute(hConsole, (BLACK << 4) | LIGHT_GREY);
}

string GetCurrentFilePath(fs::path dir_path, int userChoice)
{
    int i = 0;
    for (const auto& entry : fs::directory_iterator(dir_path))
    {
        if (i == userChoice)
            return entry.path().string();
        i++;
    }
}

bool OpenFolder(fs::path& dir_path, int userChoice)
{
    int i = 0;
    try
    {
        for (const auto& entry : fs::directory_iterator(dir_path))
        {
            if (i == userChoice && entry.is_directory())
            {
                dir_path = entry.path();
                return true;
            }
            i++;
        }
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void Events(fs::path& dir_path, int& userChoice, int amountFiles)
{
    while (true)
    {
        if (GetAsyncKeyState(VK_F1) & 0x8000)
        {
            SetCursorBlinking(true);
            system("cls");
            StartLab1(GetCurrentFilePath(dir_path, userChoice));
            Sleep(100);
            break;
        }
        if (GetAsyncKeyState(VK_F2) & 0x8000)
        {
            SetCursorBlinking(true);
            system("cls");
            StartLab2(GetCurrentFilePath(dir_path, userChoice));
            Sleep(100);
            break;
        }
        if (GetAsyncKeyState(VK_F3) & 0x8000)
        {
            system("cls");
            StartLab3(GetCurrentFilePath(dir_path, userChoice));
            Sleep(100);
            break;
        }
        if (GetAsyncKeyState(VK_F4) & 0x8000)
        {
            SetCursorBlinking(true);
            system("cls");
            StartLab4(GetCurrentFilePath(dir_path, userChoice));
            Sleep(100);
            break;
        }
        if ((GetAsyncKeyState(VK_UP) & 0x8000) && amountFiles > 1)
        {
            if (userChoice == 0)
            {
                userChoice = amountFiles - 1;
                ChangeCurrentFile(dir_path, userChoice, 0);
            }
            else
            {
                userChoice--;
                ChangeCurrentFile(dir_path, userChoice, userChoice + 1);
            }
            Sleep(150);
        }
        if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && amountFiles > 1)
        {
            if (userChoice == amountFiles - 1)
            {
                userChoice = 0;
                ChangeCurrentFile(dir_path, userChoice, amountFiles - 1);
            }
            else
            {
                userChoice++;
                ChangeCurrentFile(dir_path, userChoice, userChoice - 1);
            }
            Sleep(150);
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        {
            bool isOpenFolder = OpenFolder(dir_path, userChoice);
            if (isOpenFolder)
            {
                isOpenFolder = false;
                Sleep(100);
                break;
            }
            Sleep(100);
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        {
            if (dir_path.has_parent_path() && dir_path != "C:\\")
            {
                dir_path = dir_path.parent_path();
                Sleep(100);
                break;
            }
            Sleep(100);
        }
        Sleep(10);
    }
}

int main()
{
    setlocale(LC_ALL, "");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251); //Для возможности ввода русских символов
    SetCursorBlinking(false);

    int userChoice;
    fs::path dir_path;
    ShowFastMenu();
    OpenFolderMenu(dir_path);
    while (true)
    {
        userChoice = 0;
        system("cls");
        cout << "Open file in Lab [F1] [F2] [F3] [F4]" << "\n\n";
        cout << dir_path << "\n\n";
        ShowFiles(dir_path, userChoice);
        SetConsoleCursorPosition(hConsole, { 0, 0 });
        SetConsoleCursorPosition(hConsole, { 0, correctionNum });
        int amountFiles = GetAmountFiles(dir_path);
        Events(dir_path, userChoice, amountFiles);
    }

    return 0;
}