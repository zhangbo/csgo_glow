#pragma once
#include <iostream> // For STL i/o
#include <ctime> // For std::chrono
#include <thread> // std::thread
#include <vector> //vector ( dynamic array )
#include <iomanip>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <cstdio>

class GlowESP
{
public:
	/*Offsets*/

	std::string gettime ( );
	void begin ( );
	void glowstart ( );
	void infoprint ( );
	void printEnemyBlood();
};