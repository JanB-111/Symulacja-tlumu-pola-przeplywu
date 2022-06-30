#include "Siatka.h"
#include <math.h>
#include <iostream>

Siatka::Siatka(int wiersze, int kolumny) {
	this->wiersze = wiersze;
	this->kolumny = kolumny;

	for (int i = 0; i < wiersze; i++) {

		std::vector<Pole_siatki*> vect_kolumny;

		for (int j = 0; j < kolumny; j++) {

			Lokacja temp = Lokacja(i, j);

			vect_kolumny.push_back(new Pole_siatki(temp));
		}
		grid.push_back(vect_kolumny);
	}

	cel.set_wiersz(-1);
	cel.set_kolumna(-1);

	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < kolumny; j++) {
			Pole_siatki* aktywny = grid[i][j];
			aktywny->set_kierunek(BLOKADA);
		}
	}

}

Siatka::~Siatka() {
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < kolumny; j++) {

			delete grid[i][j];
		}
	}
	this->usunAktorow();
}


void Siatka::oblicz_pole_integracji() {
	list<Pole_siatki*> otwarte;
	Pole_siatki* start = grid[cel.get_wiersz()][cel.get_kolumna()];

	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < kolumny; j++) {
			grid[i][j]->set_wartosc(65535);
		}
	}

	start->set_wartosc(0);
	otwarte.push_back(start);

	while (!otwarte.empty()) {

		Pole_siatki* element = otwarte.front();

		otwarte.pop_front();

		list<Pole_siatki*> temp = Podaj_liste_sasiadow(element->get_pozycja());
		list<Pole_siatki*>::iterator it;

		for (it = temp.begin(); it != temp.end(); it++) {

			Pole_siatki* act = *it;

			if (act->get_koszt() != 255) {

				float old_wart = act->get_wartosc();
				float new_wart = element->get_wartosc() + (1 + act->get_koszt());

				if (new_wart < old_wart) {

					act->set_wartosc(new_wart);
					otwarte.push_back(act);
				}
			}
		}

		temp = Podaj_liste_sasiadow_diag(element->get_pozycja());

		for (it = temp.begin(); it != temp.end(); it++) {

			Pole_siatki* act = *it;

			if (act->get_koszt() != 255) {

				float old_wart = act->get_wartosc();
				float pierwiastek = sqrt(2);
				float new_wart = element->get_wartosc() + (pierwiastek + (float)(act->get_koszt()));

				if (new_wart < old_wart) {

					act->set_wartosc(new_wart);
					otwarte.push_back(act);
				}
			}
		}
	}

}

list<Pole_siatki*> Siatka::Podaj_liste_sasiadow(Lokacja aktywny) {
	int akt_wiersz = aktywny.get_wiersz();
	int akt_kolumna = aktywny.get_kolumna();

	list<Pole_siatki*> result;

	if (akt_wiersz - 1 >= 0) {
		result.push_back(grid[akt_wiersz - 1][akt_kolumna]);
	};

	if (akt_wiersz + 1 < this->wiersze) {
		result.push_back(grid[akt_wiersz + 1][akt_kolumna]);
	};

	if (akt_kolumna - 1 >= 0) {
		result.push_back(grid[akt_wiersz][akt_kolumna - 1]);
	};

	if (akt_kolumna + 1 < this->kolumny) {
		result.push_back(grid[akt_wiersz][akt_kolumna + 1]);
	};

	return result;
};

list<Pole_siatki*> Siatka::Podaj_liste_sasiadow_diag(Lokacja aktywny) {
	int akt_wiersz = aktywny.get_wiersz();
	int akt_kolumna = aktywny.get_kolumna();

	list<Pole_siatki*> result;

	if ((akt_wiersz - 1 >= 0) && (akt_kolumna - 1 >= 0)) {
		result.push_back(grid[akt_wiersz - 1][akt_kolumna - 1]);
	};

	if ((akt_wiersz - 1 >= 0) && (akt_kolumna + 1 < this->kolumny)) {
		result.push_back(grid[akt_wiersz - 1][akt_kolumna + 1]);
	};

	if ((akt_wiersz + 1 < this->wiersze) && (akt_kolumna - 1 >= 0)) {
		result.push_back(grid[akt_wiersz + 1][akt_kolumna - 1]);
	};

	if ((akt_wiersz + 1 < this->wiersze) && (akt_kolumna + 1 < this->kolumny)) {
		result.push_back(grid[akt_wiersz + 1][akt_kolumna + 1]);
	};
	return result;
};

void Siatka::oblicz_kierunki() {

	for (int wiersz = 0; wiersz < wiersze; wiersz++) {
		for (int kolumna = 0; kolumna < kolumny; kolumna++) {

			float temp = 65535;

			Pole_siatki* aktywny = grid[wiersz][kolumna];

			if (aktywny->get_wartosc() != 65535) {

				Lokacja l_akt = aktywny->get_pozycja();

				int akt_wiersz = l_akt.get_wiersz();
				int akt_kolumna = l_akt.get_kolumna();

				if (grid[akt_wiersz][akt_kolumna]->get_wartosc() == 0) {

					aktywny->set_kierunek(CEL);
				}
				else {

					if ((akt_wiersz - 1 >= 0) && (akt_kolumna - 1 >= 0)) {
						if ((grid[akt_wiersz - 1][akt_kolumna - 1]->get_wartosc()) < (aktywny->get_wartosc()) && (grid[akt_wiersz - 1][akt_kolumna - 1]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz - 1][akt_kolumna - 1]->get_wartosc();
							aktywny->set_kierunek(NW);
						}
					};

					if ((akt_wiersz - 1 >= 0) && (akt_kolumna + 1 < this->kolumny)) {
						if ((grid[akt_wiersz - 1][akt_kolumna + 1]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz - 1][akt_kolumna + 1]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz - 1][akt_kolumna + 1]->get_wartosc();
							aktywny->set_kierunek(NE);
						}
					};



					if ((akt_wiersz + 1 < this->wiersze) && (akt_kolumna - 1 >= 0)) {
						if ((grid[akt_wiersz + 1][akt_kolumna - 1]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz + 1][akt_kolumna - 1]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz + 1][akt_kolumna - 1]->get_wartosc();
							aktywny->set_kierunek(SW);
						}
					};

					if ((akt_wiersz + 1 < this->wiersze) && (akt_kolumna + 1 < this->kolumny)) {
						if ((grid[akt_wiersz + 1][akt_kolumna + 1]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz + 1][akt_kolumna + 1]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz + 1][akt_kolumna + 1]->get_wartosc();
							aktywny->set_kierunek(SE);
						}
					};

					if (akt_wiersz - 1 >= 0) {
						if ((grid[akt_wiersz - 1][akt_kolumna]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz - 1][akt_kolumna]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz - 1][akt_kolumna]->get_wartosc();
							aktywny->set_kierunek(N);
						}
					};

					if (akt_wiersz + 1 < this->wiersze) {
						if ((grid[akt_wiersz + 1][akt_kolumna]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz + 1][akt_kolumna]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz + 1][akt_kolumna]->get_wartosc();
							aktywny->set_kierunek(S);
						}
					};

					if (akt_kolumna - 1 >= 0) {
						if ((grid[akt_wiersz][akt_kolumna - 1]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz][akt_kolumna - 1]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz][akt_kolumna - 1]->get_wartosc();
							aktywny->set_kierunek(W);
						}
					};

					if (akt_kolumna + 1 < this->kolumny) {
						if ((grid[akt_wiersz][akt_kolumna + 1]->get_wartosc() < aktywny->get_wartosc()) && (grid[akt_wiersz][akt_kolumna + 1]->get_wartosc() < temp)) {

							temp = grid[akt_wiersz][akt_kolumna + 1]->get_wartosc();
							aktywny->set_kierunek(E);
						}
					};
				}
			}
			else {
				aktywny->set_kierunek(BLOKADA);
			}
		}

	}

}


void Siatka::set_cel(int wiersz, int kolumna) {

	cel.set_wiersz(wiersz);
cel.set_kolumna(kolumna);
};

int Siatka::podajKosztDlaPola(int wiersz, int kolumna) {

	Pole_siatki* aktywny = grid[wiersz][kolumna];
	return aktywny->get_koszt();
};

float Siatka::podajIntegracjePola(int wiersz, int kolumna) {

	Pole_siatki* aktywny = grid[wiersz][kolumna];
	return aktywny->get_wartosc();
};

kierunki Siatka::podajKierunki(int wiersz, int kolumna) {

	Pole_siatki* aktywny = grid[wiersz][kolumna];
	return aktywny->get_kierunek();
};

void Siatka::setKoszt(int wiersz, int kolumna, int koszt) {

	Pole_siatki* aktywny = grid[wiersz][kolumna];
	aktywny->ustaw_koszt(koszt);
};

Lokacja Siatka::get_cel() {

	return this->cel;
};

void Siatka::dodajAktora(Aktor* nowy) {

	lista_aktorow.push_back(nowy);
};

Aktor* Siatka::zwrocAktora_temp() {

	if (!lista_aktorow.empty()) {
		return lista_aktorow.front();
	}
};

Aktor* Siatka::zwrotAktora(int index) {

	return lista_aktorow[index];
};

void Siatka::przelicz(int liczba_aktorow, float predkosc, float rozmiarPola) {

	float predkosc_lokalna = predkosc / 70;
	float predkosc_diag = predkosc_lokalna / sqrt(2);

	for (int i = 0; i < liczba_aktorow; i++) {
		Aktor* aktywny = lista_aktorow[i];

		int wiersz = aktywny->get_pos_wiersz();
		int kolumna = aktywny->get_pos_kolumna();

		float temp_wiersz;
		float temp_kolumna;

		int temp_wiersz_index;
		int temp_kolumna_index;

		kierunki kierunek = grid[wiersz][kolumna]->get_kierunek();

		if (grid[wiersz][kolumna]->get_kierunek() == N) {
			if ((aktywny->get_pos_wiersz() - predkosc_lokalna) >= 0) {
				aktywny->ustaw_pozycje(aktywny->get_pos_wiersz() - predkosc_lokalna, aktywny->get_pos_kolumna());
			}
			else {
				aktywny->ustaw_pozycje(0, aktywny->get_pos_kolumna());
			}
		}
		else if (grid[wiersz][kolumna]->get_kierunek() == NW) {
			if (aktywny->get_pos_wiersz() - predkosc_diag >= 0) {
				temp_wiersz = aktywny->get_pos_wiersz() - predkosc_diag;
			}
			else {
				temp_wiersz = 0;
			}
			if (aktywny->get_pos_kolumna() - predkosc_diag >= 0) {

				temp_kolumna = aktywny->get_pos_kolumna() - predkosc_diag;
			}
			else {
				temp_kolumna = 0;
			}

			temp_wiersz_index = (int)(temp_wiersz);
			temp_kolumna_index = (int)(temp_kolumna);

			if (podajKosztDlaPola(temp_wiersz_index, temp_kolumna_index) == 255) {
				if (temp_wiersz_index < wiersz) {
					temp_kolumna = aktywny->get_pos_kolumna() - predkosc_lokalna;
					temp_wiersz = temp_wiersz + predkosc_diag;
				}
				else if (temp_kolumna_index < kolumna) {
					temp_wiersz = aktywny->get_pos_wiersz() - predkosc_lokalna;
					temp_kolumna = temp_kolumna + predkosc_diag;

				}
			}

			aktywny->ustaw_pozycje(temp_wiersz, temp_kolumna);

		}
		else if (grid[wiersz][kolumna]->get_kierunek() == NE) {
			if (aktywny->get_pos_wiersz() - predkosc_diag >= 0) {
				temp_wiersz = aktywny->get_pos_wiersz() - predkosc_diag;
			}
			else {
				temp_wiersz = 0;
			}
			if (aktywny->get_pos_kolumna() + predkosc_diag < kolumny * rozmiarPola) {

				temp_kolumna = aktywny->get_pos_kolumna() + predkosc_diag;
			}
			else {
				temp_kolumna = (kolumny * rozmiarPola) - 1;
			}

			temp_wiersz_index = (int)(temp_wiersz);
			temp_kolumna_index = (int)(temp_kolumna);

			if (podajKosztDlaPola(temp_wiersz_index, temp_kolumna_index) == 255) {
				if (temp_wiersz_index < wiersz) {
					temp_kolumna = aktywny->get_pos_kolumna() + predkosc_lokalna;
					temp_wiersz = temp_wiersz + predkosc_diag;
				}
				else if (temp_kolumna_index > kolumna) {
					temp_wiersz = aktywny->get_pos_wiersz() - predkosc_lokalna;
					temp_kolumna = temp_kolumna - predkosc_diag;

				}

			}

			aktywny->ustaw_pozycje(temp_wiersz, temp_kolumna);
		}
		else if (grid[wiersz][kolumna]->get_kierunek() == S) {
			if ((aktywny->get_pos_wiersz() + predkosc_lokalna) < wiersze * rozmiarPola) {
				aktywny->ustaw_pozycje(aktywny->get_pos_wiersz() + predkosc_lokalna, aktywny->get_pos_kolumna());
			}
			else {
				aktywny->ustaw_pozycje((wiersze * rozmiarPola) - 1, aktywny->get_pos_kolumna());
			}
		}
		else  if (grid[wiersz][kolumna]->get_kierunek() == SE) {
			if (aktywny->get_pos_wiersz() + predkosc_diag < wiersze * rozmiarPola) {

				temp_wiersz = aktywny->get_pos_wiersz() + predkosc_diag;
			}
			else {
				temp_wiersz = (wiersze * rozmiarPola) - 1;
			}
			if (aktywny->get_pos_kolumna() + predkosc_diag < kolumny * rozmiarPola) {

				temp_kolumna = aktywny->get_pos_kolumna() + predkosc_diag;
			}
			else {
				temp_kolumna = (kolumny * rozmiarPola) - 1;
			}

			temp_wiersz_index = (int)(temp_wiersz);
			temp_kolumna_index = (int)(temp_kolumna);

			if (podajKosztDlaPola(temp_wiersz_index, temp_kolumna_index) == 255) {
				if (temp_wiersz_index > wiersz) {

					temp_kolumna = aktywny->get_pos_kolumna() + predkosc_lokalna;
					temp_wiersz = temp_wiersz - predkosc_diag;
				}
				else if (temp_kolumna_index > kolumna) {

					temp_wiersz = aktywny->get_pos_wiersz() + predkosc_lokalna;
					temp_kolumna = temp_kolumna - predkosc_diag;

				}

			}

			aktywny->ustaw_pozycje(temp_wiersz, temp_kolumna);

		}
		else  if (grid[wiersz][kolumna]->get_kierunek() == SW) {
			if ((aktywny->get_pos_wiersz() + predkosc_diag) < (wiersze * rozmiarPola) - 1) {
				temp_wiersz = aktywny->get_pos_wiersz() + predkosc_diag;
			}
			else {
				temp_wiersz = (wiersze * rozmiarPola) - 100;
			}
			if (aktywny->get_pos_kolumna() - predkosc_diag >= 0) {

				temp_kolumna = aktywny->get_pos_kolumna() - predkosc_diag;
			}
			else {
				temp_kolumna = 0;
			}

			temp_wiersz_index = (int)(temp_wiersz);
			temp_kolumna_index = (int)(temp_kolumna);

			if (podajKosztDlaPola(temp_wiersz_index, temp_kolumna_index) == 255) {
				if (temp_wiersz_index > wiersz) {
					temp_kolumna = aktywny->get_pos_kolumna() - predkosc_lokalna;
					temp_wiersz = temp_wiersz - predkosc_diag;
				}
				else if (temp_kolumna_index < kolumna) {
					temp_wiersz = aktywny->get_pos_wiersz() + predkosc_lokalna;
					temp_kolumna = temp_kolumna + predkosc_diag;

				}

			}
			aktywny->ustaw_pozycje(temp_wiersz, temp_kolumna);

		}
		else  if (grid[wiersz][kolumna]->get_kierunek() == W) {
			if ((aktywny->get_pos_kolumna() - predkosc_lokalna) >= 0) {
				aktywny->ustaw_pozycje(aktywny->get_pos_wiersz(), aktywny->get_pos_kolumna() - predkosc_lokalna);

			}
			else {
				aktywny->ustaw_pozycje(aktywny->get_pos_wiersz(), 0);
			}
		}

		else  if (grid[wiersz][kolumna]->get_kierunek() == E) {
			if ((aktywny->get_pos_kolumna() - predkosc_lokalna) < kolumny * rozmiarPola) {
				aktywny->ustaw_pozycje(aktywny->get_pos_wiersz(), aktywny->get_pos_kolumna() + predkosc_lokalna);
			}
			else {
				aktywny->ustaw_pozycje(aktywny->get_pos_wiersz(), (kolumny * rozmiarPola) - 1);
			}
		}
	}
}

void Siatka::usunAktorow() {
	for (int i = 0; i < lista_aktorow.size(); i++) {
		delete lista_aktorow[i];
		lista_aktorow[i] = NULL;
	}

	lista_aktorow.erase(lista_aktorow.begin(), lista_aktorow.end());
};

int Siatka::podajWartoscPola(int index_x, int index_y) {
	return this->grid[index_x][index_y]->get_wartosc();
};

void Siatka::losuj_przeszkody() {
	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < kolumny; j++) {
			Pole_siatki* aktywny = grid[i][j];
			float temp = (rand() % 20);
			if (temp <= 9) {
				aktywny->ustaw_koszt(0);
			}
			else if ((temp > 9) && (temp <= 16)) {
				aktywny->ustaw_koszt(4);
			}
			else {
				aktywny->ustaw_koszt(255);

			}
		}
	}

	for (int i = 0; i < wiersze; i++) {
		for (int j = 0; j < kolumny; j++) {
			Pole_siatki* aktywny = grid[i][j];
			Lokacja temp = aktywny->get_pozycja();
			int akt_wiersz = temp.get_wiersz();
			int akt_kolumna = temp.get_kolumna();

			if ((akt_wiersz - 1 >= 0) && (akt_kolumna - 1 >= 0)) {
				if (((grid[akt_wiersz - 1][akt_kolumna]->get_koszt()) == 255) && (grid[akt_wiersz][akt_kolumna - 1]->get_koszt() == 255)) {
					aktywny->ustaw_koszt(255);
				}
			}
			if ((akt_wiersz - 1 >= 0) && (akt_kolumna + 1 < kolumny)) {
				if (((grid[akt_wiersz - 1][akt_kolumna]->get_koszt()) == 255) && (grid[akt_wiersz][akt_kolumna + 1]->get_koszt() == 255)) {
					aktywny->ustaw_koszt(255);
				}
			}
			if ((akt_wiersz + 1 < wiersze) && (akt_kolumna + 1 < kolumny)) {
				if (((grid[akt_wiersz + 1][akt_kolumna]->get_koszt()) == 255) && (grid[akt_wiersz][akt_kolumna + 1]->get_koszt() == 255)) {
					aktywny->ustaw_koszt(255);
				}
			}
			if ((akt_wiersz + 1 < wiersze) && (akt_kolumna - 1 >= 0 )) {
				if (((grid[akt_wiersz + 1][akt_kolumna]->get_koszt()) == 255) && (grid[akt_wiersz][akt_kolumna - 1]->get_koszt() == 255)) {
					aktywny->ustaw_koszt(255);
				}
			}
		}
	}


	if (cel.get_kolumna() != (-1)) {
		Pole_siatki* aktywny = grid[cel.get_wiersz()][cel.get_kolumna()];
		aktywny->ustaw_koszt(0);
	}

}