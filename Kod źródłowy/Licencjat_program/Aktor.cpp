#include "Aktor.h"

Aktor::Aktor(float pos_wiersz, float pos_kolumna, int index) {
	this->pos_wiersz = pos_wiersz;
	this->index = index;
	this->pos_kolumna = pos_kolumna;
	//this->deleted = false;
};

Aktor::Aktor(const Aktor& nowy) {
	this->pos_wiersz = nowy.pos_wiersz;
	this->index = nowy.index;
	this->pos_kolumna = nowy.pos_kolumna;
	//this->deleted = nowy.deleted;
};


void Aktor::ustaw_pozycje(float pos_wiersz, float pos_kolumna) {
	this->pos_wiersz = pos_wiersz;
	this->pos_kolumna = pos_kolumna;
};




float Aktor:: get_pos_wiersz() {
	return pos_wiersz;
}
float Aktor::get_pos_kolumna() {
	return pos_kolumna;
};