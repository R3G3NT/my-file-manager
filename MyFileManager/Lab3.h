#pragma once
#include <string>
#include <vector>
#include <map>

#include "WAVHeader.h"
using namespace std;

void WaitStart(); 

void SetHz(map<int, int>& Hz);

void GetHz(map<int, int>& Hz, vector<char>& buffer, WAVHeader& header, int second);

void DrawHistogram(map<int, int>& Hz, int amountHz);

int DrawSeconds(vector<char>& buffer, WAVHeader& header);

int StartLab3(string filePath);