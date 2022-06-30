#pragma once
#pragma once
#include <list>
#include "Lokacja.h"

using namespace std;

enum kierunki {
	CEL = 0,
	N = 1,
	NE = 2,
	E = 3,
	SE = 4,
	S = 5,
	SW = 6,
	W = 7,
	NW = 8,
	BLOKADA = 9
};

class Pole_siatki
{
	Lokacja pozycja;
	int koszt;
	kierunki kierunek;
	float wartosc;

public:
	Pole_siatki(Lokacja &pozycja);
	void ustaw_koszt(int koszt);
	int get_koszt();
	Lokacja get_pozycja();
	float get_wartosc();
	kierunki get_kierunek();
	void set_kierunek(kierunki x);
	void set_wartosc(float wartosc);

};

