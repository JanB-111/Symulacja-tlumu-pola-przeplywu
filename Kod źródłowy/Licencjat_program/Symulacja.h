#pragma once
#include <vector>
#include "Aktor.h"
#include "Siatka.h";

using namespace std;

class Symulacja
{
	float rozmiar_wyswietlany;
	int wiersze;
	int kolumny;
	int liczba_aktorow;
	Siatka* siatka;

public:
	Symulacja() {};
	~Symulacja();

	void nastepny_krok(int liczba_aktorow, float predkosc);

	void set_wiersze(int wiersze) {
		this->wiersze = wiersze;
	};
	void set_kolumny(int kolumny) {
		this->kolumny = kolumny;

	};
	void stworzSiatke(int wiersze, int kolumny) ;
	Siatka* zwrocSiatke();
	void set_rozmiar_wyswietlany(float rozmiar_wyswietlany) {
		this->rozmiar_wyswietlany = rozmiar_wyswietlany;
	};
	void zniszcz_siatke();


	void set_liczba_aktorow(int liczba_aktorow) {
		this->liczba_aktorow = liczba_aktorow;
	};
	void generate_actors(int wiersz, int kolumna);
	void delete_actors();

	void losujPrzeszkody() {
		siatka->losuj_przeszkody();
	}

};

