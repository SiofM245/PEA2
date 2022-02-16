#pragma once
struct tList {
	int city1;
	int city2;
	int nCadence;

	tList(int x, int y, int cad);

};

class Tabu {
public:
	__int64 counter = 0;
	double frequency = 1;
	int optimum = 0;

	std::vector<tList> tabu;
	void start();
	double stop();
	void swaper(int *way, int a, int b);
	int* bestVertex(int *way, double **matrix, int cities, float asp);
	int* diversification(int cities, double **matrix);
	void setOptimum(int opt);
	double path(double **matrix, int cities, int *perm);
	void permutation(int *&perm, int cities);
	void assign(int *arr1, int *arr2, int cities);
	void addToTabu(int a, int b, int cities);
	bool isOnTabu(int a, int b);
	void decreaseCadence();
	void resetTabu();
	double ts(time_t runtime, double ** matrix1, int cities, int & solution, bool diver);

};