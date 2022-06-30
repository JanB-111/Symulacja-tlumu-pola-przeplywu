#pragma once
class Aktor
{
	int index;
	float pos_wiersz;
	float pos_kolumna;

public:

	Aktor(float pos_wiersz, float pos_kolumna,int index);
	Aktor(const Aktor& nowy);

	void ustaw_pozycje(float pos_wiersz, float pos_kolumna);

	float get_pos_wiersz();
	float get_pos_kolumna();
};

