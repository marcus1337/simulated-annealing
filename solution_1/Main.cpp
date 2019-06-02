#include <fstream>
#include <string>
#include <iostream>

#include "Algos.h"

using namespace std;

int main() {

    //ifstream infile("tsp.in");
    istream& infile = cin;
    Algos algos;
    algos.readData(infile);

    algos.calculations();

    return 0;
}