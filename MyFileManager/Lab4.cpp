#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <fstream>
#include <filesystem>

#include "Console.h"
#include "Lab4.h"
using namespace std;
namespace fs = filesystem;

void GetCommandLab4(string& command)
{
    cout << "Zip -> 1\n";
    cout << "Unzip -> 2\n";
    cout << "Enter command: ";
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '1')
        {
            command = "-z";
            cout << ch << '\n';
            break;
        }
        else if (ch == '2')
        {
            command = "-u";
            cout << ch << '\n';
            break;
        }
    }
}

void WaitStartLab4(string command)
{
    string message = "Press 1 to start ";
    if (command == "-z" || command == "-zip")
        message += "zip";
    else
        message += "unzip";
    cout << message << endl;

    while (true)
    {
        char ch = _getch();
        if (ch == '1')
            break;
    }
}

struct Range
{
    long double low;
    long double high;
};

long double ArithmeticEncode(uint32_t size, vector<char>& buffer, map<char, int>& freqs)
{
    map<char, Range> symRanges;
    long double currentLow = 0.0;
    for (auto const& [ch, count] : freqs)
    {
        long double probability = (long double)count / size;
        symRanges[ch] = { currentLow, currentLow + probability };
        currentLow += probability;
    }

    long double L = 0.0;
    long double H = 1.0;
    for (char c : buffer)
    {
        long double width = H - L;
        Range symRange = symRanges[c];
        H = L + width * symRange.high;
        L = L + width * symRange.low;
    }

    return L;
}

void EncodeLab4(ifstream& file, string filePath)
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

    fs::path fP = filePath;
    string newfP = fP.parent_path().string() + "\\" + fP.stem().string() + ".end";
    ofstream outFile(newfP, std::ios::binary);

    //Запись длины и строки расширения
    string ext = fs::path(filePath).extension().string();
    int extSize = ext.size();
    outFile << extSize << ' ';
    outFile << ext << ' ';

    //Запись количества символов в файле
    outFile << size << ' ';

    map<char, int> freqs;
    for (char c : buffer)
        freqs[c]++;
    //Запись количества элементов в таблице частот
    outFile << freqs.size() << ' ';

    for (auto const& [ch, count] : freqs)
    {
        // Пишем 1 байт символа
        outFile << ch << ' ';
        // Пишем 4 байта частоты (int)
        outFile << count << ' ';
    }

    long double L = ArithmeticEncode(size, buffer, freqs);
    outFile << L << ' ';
    outFile.close();

    cout << "Successful encoded!" << endl;

    fs::remove(filePath);
}

void ArithmeticDecode(vector<char>& result, map<char, int>& freqs, int originalSize, long double L)
{
    map<char, Range> symRanges;
    long double currentLow = 0.0;
    for (auto const& [ch, count] : freqs)
    {
        long double probability = (long double)count / originalSize;
        symRanges[ch] = { currentLow, currentLow + probability };
        currentLow += probability;
    }

    long double code = L;
    for (uint32_t i = 0; i < originalSize; i++)
    {
        for (auto const& [ch, range] : symRanges)
        {
            if (code >= range.low && code < range.high)
            {
                result.push_back(ch);
                //Масштабируем число для поиска следующего символа
                long double width = range.high - range.low;
                code = (code - range.low) / width;
                break;
            }
        }
    }
}

void DecodeLab4(ifstream& file, string filePath)
{
    file.seekg(0, ios::beg);

    //Читаем расширение
    int extSize;
    file >> extSize;
    string ext;
    file >> ext;

    //Читаем количество символов в оригинале
    uint32_t originalSize;
    file >> originalSize;

    //Читаем таблицу частот
    int freqsSize;
    file >> freqsSize;
    map<char, int> freqs;
    for (int i = 0; i < freqsSize; i++)
    {
        char ch;
        int count;
        file >> ch;
        file >> count;
        freqs[ch] = count;
    }

    //Читаем заархивированное число
    double L;
    file >> L;
    file.close();

    vector<char> result;
    ArithmeticDecode(result, freqs, originalSize, L);

    fs::path fP = filePath;
    string outPath = fP.parent_path().string() + "\\" + fP.stem().string() + ext;
    ofstream outFile(outPath, ios::binary);
    outFile.write(result.data(), result.size());
    outFile.close();

    cout << "Successful decoded!" << endl;

    fs::remove(filePath);
}

int StartLab4(string filePath)
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
    GetCommandLab4(command);

    SetCursorBlinking(false);

    WaitStartLab4(command);
    if (command == "-z" || command == "-zip")
        EncodeLab4(file, filePath);
    else
        DecodeLab4(file, filePath);

    cout << "Press 1 to return in MyFileManager :)" << endl;
    while (true)
    {
        char ch = _getch();
        if (ch == '1')
            break;
    }
    return 0;
}