#pragma once
#include "SA.h"
#include "Tabu.h"
class Menu
{
	SA *sa;
	Tabu *tabu;

	int choice;
	std::string filename;
	int cities, optimum, number;
	double **matrix, amount;
	bool readed = false;
	bool diver = false;
	double  avg_time;
	double measurement;
	double measurement1;

	time_t runtime;
public:
	Menu();
	~Menu();

	void showMenu();
	void showSubmenu();
	bool wyborOpcji();
	bool readFile(std::string filename, double **&matrix, int &cities, int &optimum);
};

