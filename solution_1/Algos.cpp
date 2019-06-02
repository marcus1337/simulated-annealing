#include "Algos.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <array>

using namespace std;

Algos::~Algos() {
    delete[] cities;
}

void Algos::calculations() {
    srand(time(NULL));
    start = clock();
    greedyTour();
    twoOptAnnealing();
    printOut();
}

void Algos::twoOptAnnealing() {
    int i = 0;
    int j = 0;
    end = clock();
    double diff = (double)(end - start) / CLOCKS_PER_SEC;
    temperature = startTemp;
    while (diff < 1.99) {
        bool swapped = false;
        int bestCh = 0;
        int besti = -1;
        int bestj = -1;
        for (int l = 0; l < N - 1; l++) {
            for (int k = 0; k < M; k++) {
                i = l;
                j = cities[neighbours.get(tour[i], k)].pos;
                if (i > j) {
                    int store = i;
                    i = j;
                    j = store;
                }
                if (i == 0 && j == N - 1) continue;
                int i_prev = i == 0 ? N - 1 : i - 1;
                int j_next = j == N - 1 ? 0 : j + 1;

                int change = distance(tour[i_prev], tour[j]) + distance(tour[i], tour[j_next])
                    - distance(tour[i_prev], tour[i]) - distance(tour[j], tour[j_next]);
                if (change < bestCh) {
                    besti = i;
                    bestj = j;
                    bestCh = change;
                }
            }

            if (diff > 1.99) break;
        }

        if (bestCh < 0) {
            doSwap(besti, bestj);
            dist += bestCh;
            swapped = true;
        }
        else {
            randij(i, j);
            if (i == 0) {
                i += 1;
            }
            if (i > j) {
                int store = i;
                i = j;
                j = store;
            }
            if (i == 0) {
                i += 1;
            }
            int i_prev = i == 0 ? N - 1 : i - 1;
            int j_next = j == N - 1 ? 0 : j + 1;
            int change = distance(tour[i_prev], tour[j]) + distance(tour[i], tour[j_next])
                - distance(tour[i_prev], tour[i]) - distance(tour[j], tour[j_next]);
            temperature = startTemp * pow(1.0f - diff / 2, dropOffPoly);
            if (probability(dist + change) > criteria) {
                if (dist < bestDist) {
                    std::copy(tour, tour + N, bestTour);
                    bestDist = dist;
                }
                doSwap(i, j);
                dist += change;
                swapped = true;
            }
        }
        if (!swapped && diff < 1.99) {
            if (dist < bestDist) {
                std::copy(tour, tour + N, bestTour);
                bestDist = dist;
            }
            int iter = 0;
            while (2 > iter) {

                randij(i, j);
                if (i == 0) {
                    i += 1;
                }
                if (i > j) {
                    int store = i;
                    i = j;
                    j = store;
                }
                if (i == 0) {
                    i += 1;
                }

                int i_prev = i == 0 ? N - 1 : i - 1;
                int j_next = j == N - 1 ? 0 : j + 1;
                int change = distance(tour[i_prev], tour[j]) + distance(tour[i], tour[j_next])
                    - distance(tour[i_prev], tour[i]) - distance(tour[j], tour[j_next]);
                doSwap(i, j);
                dist += change;
                iter++;
            }
        }
        diff = (double)(clock() - start) / CLOCKS_PER_SEC;
    }
}

double Algos::probability(int newDist) {
    int i = dist - newDist;
    double d = exp(i / temperature);
    return d;
}

void Algos::doSwap(int i, int j) {
    if (j - i > 2) {
        std::copy(tour + i, tour + j + 1, newTour + i);
        for (int k = j; k >= i; k--) {
            int a = i + (j - k);
            tour[a] = newTour[k];
            cities[tour[a]].pos = a;
        }
    }
    else {
        int store = tour[i];
        tour[i] = tour[j];
        cities[tour[i]].pos = i;
        tour[j] = store;
        cities[tour[j]].pos = j;
    }
}

void Algos::greedyTour() {
    vector<bool> used(N, false);
    used[0] = true;
    tour[0] = 0;
    cities[0].pos = 0;
    vector<int> sortarr(N, 0);
    for (int i = 0; i < N; i++) {
        int best = -1;
        int bestDist = 0;
        int index = 0;
        City city_i = cities[tour[i]];
        for (int j = 0; j < N; j++) {
            if (city_i.ID != j) {
                sortarr[index] = j;
                index++;
                City city_j = cities[j];
                int i_to_j = euclidean(city_i, city_j);
                matrix.set(city_i.ID, city_j.ID, i_to_j);
                matrix.set(city_j.ID, city_i.ID, i_to_j);

                if (!used[j] && (best == -1 || bestDist > i_to_j)) {
                    best = j;
                    bestDist = i_to_j;
                }
            }
        }

        std::sort(std::begin(sortarr), std::end(sortarr), [&city_i, this](int a, int b) {
            return distance(city_i.ID, a) < distance(city_i.ID, b);
        });

        for (int k = 0; k < M; k++) {
            neighbours.set(city_i.ID, k, sortarr[k]);
        }
        if (i < N - 1) {
            tour[i + 1] = best;
            cities[best].pos = i + 1;
            used[best] = true;
            dist += bestDist;
        }
    }
    dist += distance(tour[N - 1], tour[0]);
}

void Algos::readData(istream &stream) {

    stream >> N;

    matrix.init(N, N);

    if (M > N) {
        M = N - 1;
    }

    neighbours.init(N, M);
    cities = new City[N];

    for (int i = 0; i < N; i++) {
        double x;
        double y;
        stream >> x;
        stream >> y;
        City city(i, x, y);
        cities[i] = city;
    }

}

void Algos::printOut() {
    if (bestDist < dist) {
        for (int i = 0; i < N; i++) {
            cout << bestTour[i] << endl;
        }
    }
    else {
        for (int i = 0; i < N; i++) {
            cout << tour[i] << endl;
        }
    }
}

int Algos::euclidean(City &i, City &j) {
    double r = sqrt(pow(j.x - i.x, 2) + pow(j.y - i.y, 2));
    return round_int(r);
}

int Algos::round_int(double r) {
    return (r > 0.0) ? (r + 0.5) : (r - 0.5);
}

int Algos::distance(int i, int j) {
    //   return matrix.get(city1.ID, city2.ID);
    return matrix.get(cities[i].ID, cities[j].ID);
}

void Algos::randij(int &i, int &j) {
    i = rand() % N;
    j = rand() % N;
}
