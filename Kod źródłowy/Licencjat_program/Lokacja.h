#pragma once
class Lokacja
{
	int wiersz;
	int kolumna;

public:
	Lokacja() {};
	Lokacja(int wiersz, int kolumna) {
		this->wiersz = wiersz;
		this->kolumna =kolumna;
	}

	int get_wiersz() {
		return this->wiersz;

	}

	int get_kolumna() {
		return this->kolumna;
		
	}

	void set_wiersz(int wiersz) {
		this->wiersz = wiersz;
	}

	void set_kolumna(int kolumna) {
		this->kolumna = kolumna;
	}

};

