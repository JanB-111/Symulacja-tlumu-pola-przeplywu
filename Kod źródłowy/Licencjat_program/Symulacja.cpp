#include "Symulacja.h"

Siatka* Symulacja::zwrocSiatke() {
	return this->siatka;
};

void Symulacja::stworzSiatke(int wiersze, int kolumny) {
	if (siatka != nullptr) {
		delete siatka;
		siatka = nullptr;
	}

	siatka = new Siatka(wiersze, kolumny);
};

void Symulacja::generate_actors(int szerokosc, int wysokosc) {
	int i = 0;
	while (i < liczba_aktorow) {

		float pos_x = (float(rand()) / float((RAND_MAX)) * szerokosc);
		float pos_y = (float(rand()) / float((RAND_MAX)) * wysokosc);

		int index_x = (int)pos_x;
		int index_y = (int)pos_y;

		Siatka* siatka = zwrocSiatke();

		if ((index_x != szerokosc) && (index_y != wysokosc)) {
			if ((siatka->podajWartoscPola(index_x, index_y) != 65535) && (siatka->podajKosztDlaPola(index_x, index_y) != 255)) {

				Aktor* x = new Aktor(pos_x, pos_y, i);

				siatka->dodajAktora(x);
				i++;
			}
		}
	}
}



void Symulacja::nastepny_krok(int liczba_aktorow,float predkosc) {
	siatka->przelicz(liczba_aktorow,predkosc, this->rozmiar_wyswietlany);
}

void Symulacja::delete_actors() {
	siatka->usunAktorow();
};

Symulacja::~Symulacja() {
	delete siatka;
}

void Symulacja::zniszcz_siatke() {

	delete siatka;
	siatka = NULL;
};
