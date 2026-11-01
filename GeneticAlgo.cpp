#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <utility>
#include <algorithm>
#include <string>
#include <filesystem>

using namespace std;

//Setup for randomizer engine in one place as setting it up is computationally expensive
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

vector<int> reproduce(const vector<int>& parent1, const vector<int>& parent2, int size) {

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

vector<int> calculateFitnessValues(vector<vector<int>> population, int M, int N)
{
    vector<int> costs;
     int max_cost = 0;
    for (auto pop: population)
    {
        int cost = calculateCost(pop, M, N);
        if(cost>max_cost)
            max_cost = cost;

        costs.push_back(cost);
    }

    //Fitness evaluation
    vector<int> fitness;
    for (auto cost: costs)
    {
        fitness.push_back((max_cost-cost)+1); //adding 1 so that worst individuals have a nonzero chance of selection
    }

    return fitness;
}


vector<int> geneticAlgorithm(vector<vector<int>>population, int popSize, int generations, int M, int N){

    int gridSize = M * N;
    uniform_int_distribution<int> mutate_distribution(1, 10); // keeping this outside of the loops so we dont need to keep rebuilding distribution

    for (int x = 0; x < generations; x++)
    {
        vector<int> fitness = calculateFitnessValues(population, M, N);
        vector<vector<int>> population2; // emptylist where new population gets put into

        discrete_distribution<int> parent_distribution(fitness.begin(), fitness.end());
        for (int i = 0; i < popSize; i++)
        {
            //randomly picking parent1 and parent2 based on fitness weights that are evaluated into probabilities using discrete distribution from random library
            int parent1_index = parent_distribution(generator);
            int parent2_index = parent_distribution(generator);

            vector<int> child = reproduce(population.at(parent1_index), population.at(parent2_index), gridSize);

            //Calculating random number such that if its a certain number we mutate child
            int random_chance = mutate_distribution(generator);
            if(random_chance == 1)
                mutate(child, M, N);

            population2.push_back(child);
        }
        population = population2;
    }

    //returning lowest cost individual from population
    int min_cost = 999999999;
    int best_individual = 0;
    int index = 0;
    for (auto pop: population)
    {
        int cost = calculateCost(pop, M, N);
        if(cost<min_cost)
        {
            min_cost = cost;
            best_individual = index;
        }
        index++;
    }
    return population.at(best_individual);
}

int main(int argc, char* argv[]) {

    if(argc < 3)
    {
        cout << "Not enough arguments, enter M N for rows and columns of grid, followed by filename.txt for solution text file" << endl;
        return 1;
    }

    int M = stoi(argv[1]);
    int N = stoi(argv[2]);


    string fileName = "";
    int runIndex = 1;
    while(true) {
        string candidateFileName = "solution_" + to_string(runIndex) + ".txt";
        if(!filesystem::exists(candidateFileName)){
            fileName = candidateFileName;
            break;
        }
        runIndex++;
    }


    ofstream solutionFile(fileName);
    ofstream solutionCostFile("solution_cost.txt", ios::app); //opening file in append mode to not overwrite previous costs

    int popSize = 100;
    int generations = 1000;

    vector<vector<int>> population = initializePopulation(popSize, M, N);
    vector<int> geneticPopulation = geneticAlgorithm(population, popSize, generations, M, N);
    int cost = calculateCost(geneticPopulation, M, N);

    std::cout << "\n========================================\n";
    std::cout << "Grid Dimensions: " << M << " x " << N << "\n";
    std::cout << "Total Cost: " << cost << "\n";
    std::cout << "Solution Grid:\n";

    if(solutionFile.is_open()){
        int currentCell = 0;
        for (int r = 0; r < M; r++)
        {
            for (int c = 0; c < N; c++)
            {
                currentCell = r * N + c;
                solutionFile <<  geneticPopulation.at(currentCell);
                cout << geneticPopulation.at(currentCell);
                if(c < N - 1){
                    solutionFile << ",";
                    cout << "\t";
                }
            }
            solutionFile << "\n";
            cout << "\n";
        }
        solutionFile.close();
        cout << fileName << " created." << endl;
    }

    if(solutionCostFile.is_open()){
        solutionCostFile << fileName << "cost value: " << cost << "\n";
        cout << "cost value: " << cost << "\n";
        solutionCostFile.close();
        cout << "solution_cost.txt updated." << endl;
    }


    return 0;
}


