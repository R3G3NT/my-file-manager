#pragma once
#include <string>

enum Graphs
{
    EMPTY,
    FULL,
    HALF,
    FIRST_HALF_HALF,
    SECOND_HALF_HALF
};

void ShowMenuLab1();

void ShowChance(unsigned long long bytes[], int amount, double border);

void ShowCounts(unsigned long long bytes[], int amount, double border);

void ShowGraph(unsigned long long bytes[], Graphs graph);

void ChooseGraph(unsigned long long bytes[], int amount);

void Events(unsigned long long bytes[], int amount, double border);

int StartLab1(std::string filePath);