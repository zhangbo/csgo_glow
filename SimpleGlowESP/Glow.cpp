#include <iostream> // For STL i/o
#include <ctime> // For std::chrono
#include <thread> // std::thread
#include <vector> //vector ( dynamic array )
#include <iomanip>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <cstdio>
#include "Glow.h"
#include "MemMan.h"
#include <string>


#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */

GlowESP glow;
MemMan mem;
struct offsets
{
	DWORD dwEntityList = 0x4CE54EC;
	DWORD dwLocalPlayer = 0xCD4774;
	DWORD dwGlowObjectManager = 0x5225718;
	DWORD m_iGlowIndex = 0xA3F8;
	DWORD m_iTeamNum = 0xF4;
	DWORD m_bDormant = 0xED;
	DWORD m_iHealth = 0x100;
}offsets;

struct vars
{
	bool loopcontrol = true;
	DWORD pLocal , GameMod , GlowObj , GlowIndex , Entity;
	int ProcID , myteam , entteam;
	float r1 , g1 , b1 , a1 , r2 , g2 , b2 , a2;
	const char * pcsgo = "csgo.exe";
	const char * pclient = "client_panorama.dll";
}vars;

std::string GlowESP::gettime ( )
{
	std::string appendgod;
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now ( );
	std::time_t now_c = std::chrono::system_clock::to_time_t ( now );
	//std::cout << std::put_time ( std::localtime ( &now_c ) , " [" "%T" "] " ) << std::endl;
	return "";
}

void GlowESP::begin ( )
{
	char temp;

	glow.gettime();
	//rgba
	vars.r1 = 0.f;
	vars.g1 = 1.f;
	vars.b1 = 0.f;
	vars.a1 = 1.f;
	vars.r2 = 1.f;
	vars.g2 = 0.f;
	vars.b2 = 0.f;
	vars.a2 = 1.f;
	vars.loopcontrol = true;
	GlowESP::glowstart ( );

}
void GlowESP::glowstart ( )
{
	vars.ProcID = mem.getProcess ( vars.pcsgo );//csgo.exe
	if (vars.ProcID == 0)
	{
		std::cout << "Game is not running!";
		return;
	}
	vars.GameMod = mem.getModule ( vars.ProcID , vars.pclient );//client_panorama.dll
	vars.pLocal = mem.readMem<DWORD> ( vars.GameMod + offsets.dwLocalPlayer );
	vars.GlowObj = mem.readMem<DWORD> ( vars.GameMod + offsets.dwGlowObjectManager );
	vars.myteam = mem.readMem<int> ( vars.pLocal + offsets.m_iTeamNum );
	while ( vars.loopcontrol )
	{
		for ( int i = 0; i < 64; i++ )//entity loop
		{
			vars.Entity = mem.readMem<DWORD> ( vars.GameMod + offsets.dwEntityList + i * 0x10 );
			if ( vars.Entity != NULL )
			{

				vars.GlowIndex = mem.readMem<DWORD> ( vars.Entity + offsets.m_iGlowIndex );
				vars.entteam = mem.readMem<int> ( vars.Entity + offsets.m_iTeamNum );
				if ( vars.entteam == vars.myteam )
				{

					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x4) , vars.r1 );
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x8) , vars.g1 );
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0xC) , vars.b1 );
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x10) , vars.a1 );

				}
				else
				{
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x4) , vars.r2 );
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x8) , vars.g2 );
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0xC) , vars.b2 );
					mem.writeMem<float> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x10) , vars.a2 );
					int blood = mem.readMem<int>( vars.Entity + offsets.m_iHealth);
					if (blood >= 85)
					{
						std::cout << GREEN << blood << " ";
					}
					else if (blood >= 40 && blood < 85)
					{
						std::cout << YELLOW << blood << " ";
					}
					else
					{
						std::cout << RED << blood << " ";
					}
					
				}

				mem.writeMem<bool> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x24) , true );//occulted
				mem.writeMem<bool> ( vars.GlowObj + ((vars.GlowIndex * 0x38) + 0x25) , false );//when UNocculsedddd
			}
		}
		std::cout << "\033[F";
	}

}

void GlowESP::printEnemyBlood( )
{

}

void GlowESP::infoprint ( )
{
	//if ( !vars.GameMod == NULL )
	//{
	//	std::cout << "-> Found Process: [ " << vars.pcsgo << " ]" << std::endl;
	//	std::cout << "-> Found Module: [ " << vars.pclient << " ]" << std::endl;
	//	std::cout << "-> Local Player: [ " << vars.pLocal << " ]" << std::endl;
	//}

}
int main ( )
{
	glow.begin ( );
	system ( "pause" );
	return 0;
}