#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <utility>

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

    int max = M * N - 1;

    //defining the bounds in a way where every number is equally likely to be generated
    uniform_int_distribution<int> distribution(0, max);

    int random_index_1 = distribution(generator);
    int random_index_2 = distribution(generator); //We are just going to swap two numbers from random indexes in the grid to mutate and make sure no duplicate nums present

    swap(grid.at(random_index_1), grid.at(random_index_2)); //using swap function from utility library
}

vector<int> reproduce(const vector<int>& parent1, const vector<int>& parent2, int M, int N) {
    int size = M * N;
    vector<int> child(size);
    
    // How will you track which numbers (1 to totalSize) are already in child?

    vector<bool> numbersUsedAlready(size + 1, false); //doing size + 1 so that we dont need to do weird indice tracking as index 13 corresponds to the number 13 and whether it is present 

    //Picking random cut point
    uniform_int_distribution<int> distribution(1, size - 2); //picking size - 2 so that one whole parent does not get copied onto child
    int random_cut_off = distribution(generator);

    //Copying from parent1 into child and marking numbers as used
    for (int i = 0; i <= random_cut_off; i++)
    {
        child[i] = parent1.at(i);

        //keeping track of numbers alreay copied to child to avoid duplicates in reprod process
        numbersUsedAlready[parent1.at(i)] = true; 
    }
    
    //Looping through parent2 and placing unused numbers into child

    int child_index = random_cut_off + 1;
    for (int i = 0; i < size; i++)
    {
        if(child_index >=size) //child array is full
            break;

        if(numbersUsedAlready[parent2.at(i)] != false) 
            continue; //prevents child_index from being iterated when we want to skip a number that is already present

        child[child_index] = parent2.at(i);
        child_index = child_index + 1;
    }
        return child;
}


int main() {

    return 0;
}


