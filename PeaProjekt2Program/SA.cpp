#include "pch.h"
#include"SA.h"

void SA::start()
{
	LARGE_INTEGER l_int;

	QueryPerformanceFrequency(&l_int);
	frequency = double(l_int.QuadPart);

	QueryPerformanceCounter(&l_int);
	counter = l_int.QuadPart;
}

void SA::setOptimum(int opt)
{
	optimum = opt;
}

double SA::stop()
{
	LARGE_INTEGER l_int;
	QueryPerformanceCounter(&l_int);
	return double(l_int.QuadPart - counter) / frequency;
}

double SA::route(double **matrix, int cities, int *permutation) {
	double cost = 0;

	//zliczanie drogi calkowitej dla danej permutacji
	for (int i = 0; i < cities - 1; i++) {
		cost += matrix[permutation[i]][permutation[i + 1]];
	}

	//droga od ostatniego do pierwszego miasta
	cost += matrix[permutation[cities - 1]][permutation[0]];

	return cost;
}

//tworzenie permutacji
void SA::permutation(int *& perm, int cities) {
	double* num = new double[cities];
	int random;

	for (int i = 0; i < cities; i++)
	{
		num[i] = i;
	}

	for (int i = cities; i > 0; i--)
	{
		random = rand() % i; //losuje jedno miasto
		perm[i - 1] = num[random];
		num[random] = num[i - 1];
	}

	delete[] num;
}

bool SA::probability(int length, int length1, double temperature) {

	double p = pow(e, ((-1 * ((long long)length1 - (long long)length)) / temperature));

	double r = (double)rand() / RAND_MAX; // wybieramy cyfrê z przedzia³u <0,1>

	return (r < p); //jezeli prawdopodobienstwo ze wzoru jest wieksze od losowo wybranej liczby z przedzialu<0,1>

}

double SA::sa(double ** matrix1, int cities, int & solution, time_t runtime) {

	start();

	int * permutation1 = new int[cities];
	int * permutation2 = new int[cities];

	double oldOptimum = 0;
	int city1, city2;
	double temperature, delta_max = 0, delta, cost1, cost2;;


	for (int i = 0; i < cities; i++)
	{
		permutation(permutation1, cities);
		permutation(permutation2, cities);
		delta = abs(route(matrix1, cities, permutation1) - route(matrix1, cities, permutation2));
		if (delta > delta_max)
		{
			delta_max = delta;
		}

		delete[] permutation1;
		delete[] permutation2;
		permutation1 = new int[cities];
		permutation2 = new int[cities];
	}

	temperature = delta_max;

	//tworze permutacje i obliczam koszt drogi dla niej
	permutation(permutation1, cities);
	cost1 = route(matrix1, cities, permutation1);

	//tworze zapasowa tablice permutacja2 
	for (int i = 0; i < cities; i++)
	{
		permutation2[i] = permutation1[i];
	}

	std::time_t finish;
	std::time_t start1 = std::time(NULL);

	finish = start1 + runtime;
	std::ofstream outFile("wynik.txt", std::ofstream::out);
	int counter = 0;;

	while (temperature > min_temperature && start1<finish )
	{
		counter++;
		do
		{
			//losowanie 2 nowych miast do zamiany w permutacji
			city1 = rand() % cities;
			city2 = rand() % cities;

		} while (city1 == city2);//petla wykonuje sie dopoki miasta nie beda rozne

		//zamiana miast
		permutation2[city2] = permutation1[city1];
		permutation2[city1] = permutation1[city2];


		cost2 = route(matrix1, cities, permutation2);

		if (cost2 <= cost1 || probability(cost1, cost2, temperature))
		{
			cost1 = cost2;//
			if (cost1 <= solution)
			{
				solution = cost1;
				if (oldOptimum != (double)(solution - optimum) / optimum) {
					outFile << stop() << ": " << (double)(solution - optimum) / optimum << std::endl;
					oldOptimum = (double)(solution - optimum) / optimum;
				}
			}

			permutation1[city1] = permutation2[city1]; //koszt dla permutacji 2 by³ wiêkszy wiêc staje sie "lepsza" permutacja
			permutation1[city2] = permutation2[city2];
		}
		else
		{
			permutation2[city1] = permutation1[city1];
			permutation2[city2] = permutation1[city2];
		}

		double delta_min = pow(delta_max, 0.0001);

		double beta = (delta_max - delta_min) / (100000 * delta_max*delta_min);

		temperature = (temperature/(1+beta*temperature));

		start1 = std::time(NULL);
		stop();
		if ((double)(solution - optimum) / optimum == (double)0) {
			break;
		}
	}
	outFile.close();

	delete[] permutation1;
	delete[] permutation2;

	return stop();
}

