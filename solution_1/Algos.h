#include <vector>
#include <istream>
#include <math.h>
#include "Matrix.h"
#include <time.h>
#include <chrono>
#include <ctime>
#include <array>

using namespace std;

class Algos {
public:
    clock_t start, end;

    struct City {
        int ID;
        double x;
        double y;
        int pos = -1;

        City(int _ID, double _x, double _y) : x(_x), y(_y), ID(_ID) {}

        City() {};
    };

    const double criteria = .01;
    const int dropOffPoly = 3;
    const float startTemp = 15000;
    double temperature = 1000000000;
    City *cities = nullptr;
    Matrix matrix;
    Matrix neighbours;
    int tour[1100] = {};
    int newTour[1100] = {};
    int bestTour[1100] = {};
    int N;
    int M = 100;
    int dist = 0;
    int bestDist = 999999999;

    ~Algos();

    void readData(istream &stream);

    void greedyTour();

    int computeTourDistance(int array[]);

    void printOut();

    int euclidean(City &i, City &j);

    static int round_int(double r);

    int distance(int i, int j);

    void calculations();

    void twoOptAnnealing();

    void randij(int &i, int &j);

    void doSwap(int i, int j);

    double probability(int newDist);
};