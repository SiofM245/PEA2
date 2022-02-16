#include "pch.h"
#include "Tabu.h"

void Tabu::start()
{
	LARGE_INTEGER l_int;

	QueryPerformanceFrequency(&l_int);
	frequency = double(l_int.QuadPart);

	QueryPerformanceCounter(&l_int);
	counter = l_int.QuadPart;
}

double Tabu::stop()
{
	LARGE_INTEGER l_int;
	QueryPerformanceCounter(&l_int);
	return double(l_int.QuadPart - counter) / frequency;
}

tList::tList(int x, int y, int cad) {
	city1 = x;
	city2 = y;
	nCadence = cad;
}



double Tabu::path(double **matrix, int cities, int *perm) {
	double cost = 0;

	for (int i = 0; i < cities - 1; i++) {
		cost += matrix[perm[i]][perm[i + 1]];
	}

	cost += matrix[perm[cities - 1]][perm[0]];//droga od ostatniego do pierwszego miasta

	return cost;
}



void Tabu::permutation(int *& perm, int cities) {

	for (int c = 0; c < cities; c++)
	{
		bool check;
		int random;
		do {
			random = rand() % cities; //losuje jedno miasto
			check = true;
			for (int j = 0; j < c; j++) {
				if (random == perm[j]) {
					check = false;
					break;
				}
			}

		} while (!check);
		perm[c] = random;
	}

}

//dywersyfikacja- tworze N (gdzie N-liczba miast) nowych tras i wybieram najlepsza z nich
int* Tabu::diversification(int cities, double **matrix) {
	double current, best = DBL_MAX;
	int *best_route = new int[cities];
	int *current_route = new int[cities];
	for (int i = 0; i < cities; i++) {
		int *permutation1 = new int[cities];

		permutation(permutation1, cities);
		current = path(matrix, cities, permutation1);
		assign(current_route, permutation1, cities);

		if (current <= best) {
			best = current;
			assign(best_route, current_route, cities);
		}
		delete[]permutation1;
	}

	delete[]current_route;
	return best_route;
}

void Tabu::assign(int *arr1, int *arr2, int cities) {
	for (int i = 0; i < cities; i++) {
		arr1[i] = arr2[i];
	}
}

void Tabu::swaper(int *route, int a, int b) {
	int temp = route[a];
	route[a] = route[b];
	route[b] = temp;
}

void Tabu::addToTabu(int a, int b, int cities)
{
	tabu.push_back(tList(a, b, 10));
}

bool Tabu::isOnTabu(int a, int b) {
	for (int i = 0; i < tabu.size(); i++) {
		if ((tabu.at(i).city1 == a && tabu.at(i).city2 == b || tabu.at(i).city2 == a && tabu.at(i).city1 == b) && tabu.at(i).nCadence > 0) {
			return true;
		}
	}
	return false;
}

//zmniejszenie kadencji
void Tabu::decreaseCadence() {
	for (int i = 0; i < tabu.size(); i++) {
		tabu.at(i).nCadence--;
		if (tabu.at(i).nCadence == 0) {
			std::swap(tabu.at(i), tabu.at(tabu.size() - 1));
			tabu.pop_back();
		}
	}
}

//resetowanie Tabu Listy
void Tabu::resetTabu() {
	for (int i = 0; i < tabu.size(); i++) {
		tabu.push_back(tList(NULL, NULL, NULL));
		tabu.at(i).nCadence = 0;
	}
}

//wybranie najlepszego sasiada
int * Tabu::bestVertex(int *way, double **matrix, int cities, float asp) {
	double currentPath, bestPath;
	float improvement;
	int *best = new int[cities];

	assign(best, way, cities);

	for (int i = 0; i < cities; i++)
	{
		for (int j = 0; j < cities; j++) {

			swaper(way, i, j);//wyznaczenie sasiedztwa-zamiana 2 miast w trasie
			decreaseCadence();

			currentPath = path(matrix, cities, way);
			bestPath = path(matrix, cities, best);

			//jezeli przejscie miedzy miastami i,j znajduje sie na liscie tabu
			if (isOnTabu(i, j) != false) {

				if (currentPath < bestPath) {
					//kryterium aspiracji- nowa najlepsza trasa przynajmniej 40% lepsza od obecnie branej jako najlepsza
					improvement = (bestPath - currentPath) / bestPath;

					if (improvement >= asp) {
						resetTabu();

						delete[] best;
						best = new int[cities];
						assign(best, way, cities);

						addToTabu(i, j, cities);
						//dodajPrzejscieDoTabu(j, i, miasta);

						//wielkosc listy-3*N
						if (tabu.size() > 3 * cities) {//jezeli lista wieksza od 3*N-usuwam z niej najdawniej dodane przejscie
							tabu.pop_back();
						}

					}
				}
				swaper(way, i, j);
				break;
			}
			//jezeli przejscie miedzy miastami i,j nie znajduje sie na liscie tabu
			else if (currentPath < bestPath) {

				delete[] best;
				best = new int[cities];
				assign(best, way, cities);

				addToTabu(i, j, cities);
				//dodajPrzejscieDoTabu(j, i, miasta);
				//wielkosc listy-3*N
				if (tabu.size() > 3 * cities) {//jezeli lista wieksza od 3*N-usuwam z niej najdawniej dodane przejscie
					tabu.pop_back();
				}
			}
			swaper(way, i, j);
		}
	}
	return best;
}

double Tabu::ts(time_t runtime, double ** matrix1, int cities, int & solution, bool diver)
{
	double oldOptimum = 0;
	start();
	int *bestPath = new int[cities];
	int *firstPermutation = new int[cities];
	int amount = 0, localCounter = 0;

	permutation(firstPermutation, cities);

	assign(bestPath, firstPermutation, cities);
	int *currentPath = new int[cities];
	assign(currentPath, bestPath, cities);

	std::time_t finish;
	std::time_t start1 = std::time(NULL);

	finish = start1 + runtime;
	int counter = 0;

	while (start1 < finish)
	{
		counter++;
		currentPath = bestVertex(currentPath, matrix1, cities, 0.1);//wyszukiwanie nowej trasy w sasiedztwie obecnej
		if (path(matrix1, cities, currentPath) < path(matrix1, cities, bestPath)) {
			delete[] bestPath;
			bestPath = new int[cities];
			assign(bestPath, currentPath, cities);

		}
		else
			amount++; //jezeli nie znaleziono lepszej trasy 

		//jezeli przez 10 przejsc nie znaleziono lepszej trasy nastepuje dywersyfikacja
		if (amount == cities && diver==true) {
			delete[] currentPath;
			currentPath = new int[cities];
			currentPath = diversification(cities, matrix1);
		}


		//if (amount == 2*miasta){//jezeli przez N przejsc nie znaleziono lepszej trasy, cala jest losowana od nowa
		

		start1 = time(NULL);
		stop();
		if ((double)(solution - optimum) / optimum == (double)0) {
			break;
		}
		solution = path(matrix1, cities, bestPath);
	}


	solution = path(matrix1, cities, bestPath);


	delete[]bestPath;
	delete[]firstPermutation;
	delete[]currentPath;
	return stop();
}

