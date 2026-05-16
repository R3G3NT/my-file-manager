#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <conio.h>
#include <fstream>
#include <filesystem>
using namespace std;

void GetCommandLab4(string& command);

void WaitStartLab4(string command);

long double ArithmeticEncode(uint32_t size, vector<char>& buffer, map<char, int>& freqs);

void EncodeLab4(ifstream& file, string filePath);

void ArithmeticDecode(vector<char>& result, map<char, int>& freqs, int originalSize, long double L);

void DecodeLab4(ifstream& file, string filePath);

int StartLab4(string filePath);