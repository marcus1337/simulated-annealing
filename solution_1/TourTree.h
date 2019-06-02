#include <vector>
#include <math.h> 
#include <iostream>
#include <algorithm>
#include <reverse>

class TourTree {

    struct Node {
      
        bool leftToRight = true;
        int* kids;
        int start;
        int end;

        void insertValues(int _tour[], int parentIndex, int range, int maxN) {
            kids = _tour;
            start = parentIndex*range;
            end = start + range - 1;
            if (end >= maxN) {
                end = maxN - 1;
            }
        }

        void smallSwap(int i, int j, int* tempTour) {
           
            if (j - i > 2) {
                if (leftToRight) {
                    std::copy(kids + start + i, kids + start + j + 1, tempTour + i);
                    std::reverse_copy(tempTour + i, tempTour + j + 1, kids + start + i);
                }
                else {
                    i = end - i - start;
                    j = end - j - start;

                    std::copy(kids + start + j, kids + start + i + 1, tempTour + j);
                    std::reverse_copy(tempTour + j, tempTour + i + 1, kids + start + j);
                }
            }
            else {
                int store = kids[i + start];
                kids[i + start] = kids[j + start];
                kids[j + start] = store;
            }
        }

        Node() {
        }
    };

public:
    int N; //Root of N rounded up
    int maxN; //Actual tour length
    std::vector<Node> nodes;
    int* tempTour;
    int* tour;

    TourTree() {};
    void init(int _maxN, int _tour[], int _tempTour[]) {  
        tour = _tour;
        tempTour = _tempTour;
        maxN = _maxN;
        N = ceil(sqrt(_maxN));
        nodes.reserve(N);
       
        for (int i = 0; i < N; i++) {
            Node n; n.insertValues(_tour, i, N, maxN);
            nodes.push_back(n);
        }
       specialSwapRIGHT(nodes[0], nodes[1], 0, 1);

    }

    int getCity(int i) {
        int irot = i / N;
        int index = i - irot*N;
        if (!nodes[irot].leftToRight) {
            index = nodes[irot].end - index - nodes[irot].start;
        }
        return tour[irot*N + index];
    }

    void print2() {
        int counter = 0;
        for (int i = 0; i < N && counter < maxN; i++) {
            if (nodes[i].leftToRight) {
                for (int j = i*N; j < i*N + N && counter < maxN; j++) {
                    std::cout << tour[j] << std::endl;
                    counter++;
                }
            }
            else {
                for (int j = i*N + N - 1; j >= i*N && counter < maxN; j--) {
                    if (j < maxN) {
                        std::cout << tour[j] << std::endl;
                        counter++;
                    }
                }
            }
        }
    }

    void TwoOptSwap(int i, int j) {
        int irot = i/N;
        int jrot = j/N;

        if (jrot - irot == 0) {
            nodes[irot].smallSwap(i-irot*N, j-jrot*N, tempTour);
        }
        else {
            for (int i = irot + 1; i < jrot; i++) {
                nodes[i].leftToRight = !nodes[i].leftToRight;
            }

            if (nodes[irot].leftToRight && nodes[jrot].leftToRight) {
                specialSwap(nodes[irot], nodes[jrot], i - irot*N, j - jrot*N);
            }
            else if (!nodes[irot].leftToRight && nodes[jrot].leftToRight) { //LEFT reversed
                specialSwapLEFT(nodes[irot], nodes[jrot], i - irot*N, j - jrot*N);
            }
            else if (nodes[irot].leftToRight && !nodes[jrot].leftToRight) { //RIGHT reversed
                specialSwapRIGHT(nodes[irot], nodes[jrot], i - irot*N, j - jrot*N);
            }
            else { //BOTH reversed
                specialSwapBOTH(nodes[irot], nodes[jrot], i - irot*N, j - jrot*N);
            }
      
        }
    }

private:
    void specialSwap(Node& in, Node& jn, int i, int j) {
        int temp = N - i - 1;
        std::copy(tour + in.start + i, tour + in.start + N, tempTour);
        std::copy(tour + jn.start, tour + jn.start + j + 1, tempTour + temp + 1);
        int counter = 0;

        for (int e = N-i+j; e >= 0 && counter <= temp; e--) {
            tour[counter + i + in.start] = tempTour[e];
            counter++;
        }
        int oldCounter = counter;
        counter = 0;
        for (int e = N - i + j - oldCounter; e >= 0; e--) {
            tour[counter + jn.start] = tempTour[e];
            counter++;
        }
    }

    void specialSwapLEFT(Node& in, Node& jn, int i, int j) {
        int iL = in.end - i - in.start;
        std::reverse_copy(tour + in.start, tour + in.start + iL + 1, tempTour);
        std::copy(tour + jn.start, tour + jn.start + j + 1, tempTour + iL + 1);
      //  for (int e = 0; i < iL + j + 2; e++) {
     //       std::cout << "TEST:  " << tempTour[e] << std::endl;
      //  }
        std::copy(tempTour + iL + 1+j- iL, tempTour + iL + 1 + j+1, tour + in.start);
        std::reverse_copy(tempTour, tempTour + 1 + j, tour + jn.start);
    }

    void specialSwapRIGHT(Node& in, Node& jn, int i, int j) {
        int jL = jn.end - j - jn.start;
        int temp = N - i - 1;
        std::copy(tour + in.start + i, tour + in.start + N, tempTour);
        std::reverse_copy(tour + jn.start+jL, tour + jn.end + 1, tempTour + temp + 1);
  
          for (int e = 0; e < jL + N-i+2 ; e++) {
               //std::cout << "TEST:  " << tempTour[e] << std::endl;
               for (int b = 0; b < jL + N - i + 2; b++) {
                   if(b != e && tempTour[e] == tempTour[b])
                        std::cout << "WHATTT:  " << tempTour[b] << std::endl;
               }

          }
        //std::reverse_copy(tempTour + 1 + j, tempTour + temp + 2 + j, tour + in.start+i);
        //std::copy(tempTour, tempTour + 1 + jL+1, tour + jn.start + jL);
        
        std::reverse_copy(tempTour + 1 + j, tempTour + temp + 2 + j, tour + in.start + i);
        std::copy(tempTour, tempTour + 1 + jL + 1, tour + jn.start + jL);
    }

    void specialSwapBOTH(Node& in, Node& jn, int i, int j) {
        int jL = jn.end - j - jn.start;
        int iL = in.end - i - in.start;
        int temp = N - i - 1;

        std::reverse_copy(tour + in.start, tour + in.start + iL + 1, tempTour);
        std::reverse_copy(tour + jn.start + jL, tour + jn.end + 1, tempTour + iL + 1);

        /*for (int e = 0; e < jL + N-i+2 ; e++) {
        std::cout << "TEST:  " << tempTour[e] << std::endl;
        }*/
        std::copy(tempTour + iL + 1 + j - iL, tempTour + iL + 1 + j + 1, tour + in.start);
        std::copy(tempTour, tempTour + 1 + jL + 1, tour + jn.start + jL);
    }

};