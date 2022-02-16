#pragma once
class SA {
private:
	__int64 counter = 0;
	double frequency = 1;
	const double e = 2.718281828459;
	double min_temperature = 0.001;
	double solution = 0;
	int optimum = 0;

public:
	void setOptimum(int opt);

	//wyliczenie czasu koñcowego
	double stop();
	//ustawienie danych potrzebnych do mierzenia czasu
	void start();

	//obliczanie drogi pomiêdzy miastami
	double route(double **matrix, int cities, int *perm);

	//losowanie
	void permutation(int *&perm, int cities);

	//prawdopodobienstwo wybrania sasiedniego miasta
	bool probability(int length1, int length2, double temperature);

	//algorytm symulowanego wyrzazania
	double sa(double **matrix1, int cities, int & solution, time_t runtime);
};