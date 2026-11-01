#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>

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

vector<vector<int>> initializePopulation(int k, int M, int N)
{
    vector<vector<int>> population;

    for (int i = 0; i < k; i++)
    {
        vector<int> pop_iteration(M * N);
        int current = 0;
        for (int x = 0; x < M * N; x++)
        {
            pop_iteration[x] = ++current;
        }

        shuffle(pop_iteration.begin(), pop_iteration.end(), generator); //using shuffle function from algorithm to randomize all initial pops
        population.push_back(pop_iteration);
    }

    return population;
}

vector<int> geneticAlgorithm(vector<vector<int>>population, vector<int>fitness, int popSize, int generations){

    for (int x = 0; x < generations; x++)
    {

        vector<vector<int>> population2; // emptylist where new population gets put into

        for (int i = 0; i < popSize; i++)
        {
            //randomly picking parent1 and parent2 based on fitness weights that are evaluated into probabilities using discrete distribution from random library
            discrete_distribution<int> distribution(fitness.begin(), fitness.end());
            int parent1_index = distribution(generator);
            int parent2_index = distribution(generator);
        }
    }
}



int main() {

    int popSize = 100;
    int M = 4;
    int N = 5;


    vector<int> costs;
    vector<vector<int>> population = initializePopulation(popSize, M, N);

    //Getting all the different costs and max_cost for our fitness evaluation 
    int max_cost = 0;
    for (auto pop: population)
    {
        int cost = calculateCost(pop, M, N);
        if(cost>max_cost)
            max_cost = cost;

        costs.push_back(calculateCost(pop, M, N));
    }

    //Fitness evaluation
    vector<int> fitness;
    for (auto cost: costs)
    {
        fitness.push_back((max_cost-cost)+1); //adding 1 so that worst individuals have a nonzero chance of selection
    }






    return 0;
}


