#pragma once
#include "Aktor.h"
#include "Lokacja.h"
#include "Pole_siatki.h"
#include <list>
#include <vector>
using namespace std;

class Siatka
{
	int wiersze;
	int kolumny;
	std::vector<Aktor*> lista_aktorow;
	std::vector<std::vector<Pole_siatki*>> grid;
	Lokacja cel;
	list<Pole_siatki*> Podaj_liste_sasiadow(Lokacja aktywny);
	list<Pole_siatki*> Podaj_liste_sasiadow_diag(Lokacja aktywny);

public:
	Siatka(int wiersze, int kolumny);
	void dodajAktora(Aktor *nowy);
	void oblicz_pole_integracji();
	kierunki podajKierunki(int wiersz, int kolumna);
	void oblicz_kierunki();
	void rysuj_pole_przeplywu();
	void przelicz(int liczba_aktorow,float predkosc, float rozmiarPola);
	Lokacja pobierz_koordynaty_pola(float pos_wiersz, float pos_kolumna);
	void rysuj_siatke();
	void set_cel(int wiersz, int kolumna);
	Lokacja get_cel();
	~Siatka();
	int podajKosztDlaPola(int wiersz, int kolumna);
	float podajIntegracjePola(int wiersz, int kolumna);
	void setKoszt(int wiersz, int kolumna, int koszt);
	Aktor* zwrocAktora_temp();
	Aktor* zwrotAktora(int index);
	void usunAktorow();
	int podajWartoscPola(int index_x,int index_y);
	void losuj_przeszkody();
};

