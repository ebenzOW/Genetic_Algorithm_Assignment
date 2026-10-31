#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

//Setup for randomizer engine
static random_device rand_device;
static mt19937 generator(rand_device());

int calculateCost(const vector<int>& grid, int M, int N)
{
    int cost = 0;
    int currentCell = 0;
    int rightNeighbor = 0;
    int downNeighbor = 0;

    for (int r = 0; r < M; r++)
    {
        for (int c = 0; c < N; c++)
        {
            currentCell = r * N + c;
            rightNeighbor = currentCell + 1;
            downNeighbor = (r + 1) * N + c;

            if(c < N - 1)
                cost = cost + abs(grid.at(currentCell) - grid.at(rightNeighbor));
            if(r != M - 1)    
                cost = cost + abs(grid.at(currentCell) - grid.at(downNeighbor));
        }
    }

    return cost;
}

void mutate(vector<int>& grid, int M, int N){

    int max = M * N;

    //defining the bounds in a way where every number is equally likely to be generated
    uniform_int_distribution<int> distribution(1, max);

    int random_index_1 = distribution(generator);
    int random_index_2 = distribution(generator);

    
}


int main() {

    return 0;
}


