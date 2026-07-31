// Executes particle simulation where a user defined amount of particles (10-100) move around indefinitely. 
// Uses struct vectors and struct pointer vectors to store objects and use grid based programming. Euclidean approximation is used to simulate movement over a fixed time step.

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "Particles.h"

using namespace std;

//timestep and width of each grid cell
const double dt = 0.1;
const int width = 800 / 3;

int main()
{
    int nP, row, col;
    double accumulator;


    random_device myEngine;

    //prompts user to enter valid amount of particles
    cout << "How many particles do you want(10-100): ";
    cin >> nP;

    while (nP < 10 || nP>100) {
        cout << "\nInvalid Input. Enter a valid amount: ";
        cin >> nP;
    }

    //creates a vector of particles with the valid size the user specified to keep track of every particle
    vector<Particles>particles(nP);

    //the grid (3d vector of Particle pointers) to optimize the program
    vector<vector<vector<Particles*>>>grid(3, vector<vector<Particles*>>(3));

    //sets up random number generator for particle position and velocity
    uniform_real_distribution<double>randPos(1, 799);
    uniform_real_distribution<double>randVelo(1, 30);

    for (int index = 0; index < particles.size(); index++) {
        //Randomly assigns x and y to be from 1 to 799 because having the user decide would be tedious
        particles[index].x = randPos(myEngine);
        particles[index].y = randPos(myEngine);

        //Randomly assigns vx and vy to be from 1 to 30
        particles[index].vx = randVelo(myEngine);
        particles[index].vy = randVelo(myEngine);

        //finds grid cell based on particle position
        row = particles[index].y / width;
        col = particles[index].x / width;

        //safeguards to ensure that each particle goes to a valid cell
        if (row < 0) {
            row = 0;
        }
        if (row > 2) {
            row = 2;
        }
        if (col < 0) {
            col = 0;
        }
        if (col > 2) {
            col = 2;
        }

        //puts the particle in the cell
        grid[row][col].push_back(&particles[index]);
    }

    //sets accumulator to 0
    accumulator = 0;

    //start frame timer
    auto previousTime = chrono::high_resolution_clock::now();

    //infinite loop
    while (true) {
        //end of frame timer
        auto currentTime = chrono::high_resolution_clock::now();

        //duration of frame in seconds
        chrono::duration<double> frameDur = currentTime - previousTime;

        //assign the start of the next frame to be the time the previous one ended
        previousTime = currentTime;

        //increment accumulator by frame duration
        accumulator += frameDur.count();

        while (accumulator >= dt) {
            //update position, wall collision checks, and clear and update the grid
            updatePos(particles);
            wallCollis(particles);
            clearAndFix(particles, grid);

            for (int row = 0; row < grid.size(); row++) {
                for (int col = 0; col < grid[0].size(); col++) {
                    //if a grid cell has more then 1 particle (is active) then check if they are colliding
                    if (grid[row][col].size() > 1) {
                        particleCollis(grid[row][col]);
                    }
                }
            }
            //decrement accumulator by the timestep
            accumulator -= dt;
        }
    }


    return 0;
}
