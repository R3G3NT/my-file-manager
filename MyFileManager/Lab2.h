#pragma once
#include <string>
#include <fstream>
#include <vector>
using namespace std;

void GetCommand(string& command);

void Encode(uint32_t size, vector<char> buffer, string filePath);

void Decode(uint32_t size, vector<char> buffer, string filePath);

void WaitStart(string command);

void ReadBytes(string command, ifstream& file, string filePath);

int StartLab2(string filePath);