#include "Pole_siatki.h"
using namespace std;

Pole_siatki::Pole_siatki(Lokacja& pozycja) {
	this->pozycja = pozycja;
	this->koszt = 0;
	this->wartosc = 404;
};

void Pole_siatki::ustaw_koszt(int koszt) {
	this->koszt = koszt;
};

int Pole_siatki::get_koszt() {
	return koszt;
};

float Pole_siatki::get_wartosc() {
	return wartosc;
};

kierunki Pole_siatki::get_kierunek() {
	return kierunek;
};

void Pole_siatki::set_kierunek(kierunki x) {
	this->kierunek = x;
};

void Pole_siatki::set_wartosc(float wartosc) {
	this->wartosc = wartosc;
};


Lokacja Pole_siatki::get_pozycja() {
	return this->pozycja;

}