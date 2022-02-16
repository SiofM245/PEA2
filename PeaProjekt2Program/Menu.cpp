#include "pch.h"
#include "Menu.h"


Menu::Menu()
{
	sa = new SA();
	tabu = new Tabu();
	choice = -1;
	cities = 0;
	avg_time = 0.0;
}


Menu::~Menu()
{
}

void Menu::showMenu() {
	system("cls");
	std::cout << "1. Wczytaj dane\n";
	std::cout << "2. Symulowane wyżarzanie\n";
	std::cout << "3. Tabu Search\n";
	std::cout << "4. Wyjscie\n";
}

void Menu::showSubmenu() {
	system("cls");
	std::cout << "Wybierz plik (bez rozszerzenia - atsp): ";
}

bool Menu::wyborOpcji() {
	std::cout << "Wybierz: ";
	std::cin >> choice;

	switch (choice)
	{
	case 1:
		showSubmenu();
		std::cin >> filename;
		if(readFile(filename, matrix, cities, optimum))
			readed = true;
		system("pause");
		break;
	case 2:
		system("cls");
		if (readed) {
			std::cout << "Rozpoczeto";
			int result = INT_MAX;
			measurement = sa->sa(matrix, cities, result, runtime);
			avg_time += measurement;
			std::cout << "Sredni czas wykonania algorytmu: ";
			std::cout << avg_time << "\n";
			std::cout << "\n" << "\n" << "Zakonczono obliczenia." << "\n" << "\n";
			std::cout << "\n";
		}
		else {
			std::cout << "Brak danych";
		}
		system("pause");
		break;
	case 3:
		system("cls");
		if (readed) {
			int result1 = INT_MAX;
			diver = false;
			measurement1 = tabu->ts(runtime, matrix, cities, result1, diver);
			avg_time += measurement1;
			avg_time /= 1;
			std::cout << "Sredni czas wykonania algorytmu: ";
			std::cout << avg_time << std::endl;
			std::cout << std::endl << std::endl;
			avg_time = 0;
		}
		else {
			std::cout << "Brak danych";
		}
		system("pause");
		break;
	case 4:
		return true;
		break;
	default:
		break;
	}
	return false;
}

bool Menu::readFile(std::string filename, double **&matrix, int &cities, int &optimum) {
	std::fstream file;
	file.open((filename + ".atsp").c_str(), std::ios::in);

	if (file.good()) {

		file >> cities >> optimum;

		matrix = new double *[cities];

		for (int i = 0; i < cities; ++i) {
			*(matrix + i) = new double[cities]; 
			for (int j = 0; j < cities; ++j) {

				file >> matrix[i][j];

			}

		}


		std::cout << "Plik odczytany poprawnie." << std::endl;
		std::cout << std::endl;

		file.close();
		return true;

	}
	else {
		std::cout << "Blad. " << std::endl;
		system("pause");
		file.close();
		return false;
	}
}
