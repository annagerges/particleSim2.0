//cpp file for the program's functions

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <string>
#include <fstream>
#include "Particles.h"

using namespace std;

const float dt = 0.1f;

//updates position using euclidean approximation
void updatePos(vector<Particles>& part, Spring& s) {
	for (int index = 0; index < part.size(); index++) {
		//changes acceleration because the ball is on the spring
		if (part[index].getY() <= s.getHeight()) {
			part[index].setA((((s.getK() / part[index].getMass()) * -1) * (part[index].getY() - s.getHeight())) - 9.8);
		}
		else {
			part[index].setA(-9.8);
		}
		part[index].setVy(part[index].getVy() + part[index].getA() * 0.01);
		part[index].setY(part[index].getY() + part[index].getVy() * 0.01);
		part[index].setX(part[index].getX() + part[index].getVx() * 0.01);
	}
}

//checks if any of the particles collided with the wall
void  wallCollis(vector<Particles>& part) {
	for (int index = 0; index < part.size(); index++) {
		//if the particle is beyond or colliding with the left edge: reverse its x velocity and change its x position to 1
		if (part[index].getX() <= 0) {
			part[index].setVx(abs(part[index].getVx()));
			part[index].setX(1);

		}

		//if the particle is beyond or colliding with the right edge: reverse its x velocity and change its x position to 799
		else if (part[index].getX() >= 800) {
			part[index].setVx(abs(part[index].getVx()) * -1);
			part[index].setX(799);
		}

		//if the particle is beyond or colliding with the upper edge: reverse its y velocity and change its y position to 799
		if (part[index].getY() >= 800) {
			part[index].setVy(abs(part[index].getVy()) * -1);
			part[index].setY(799);
		}
		// if the particle is beyond or colliding with the floor: reverse its y velocity and clamp it
		else if (part[index].getY() <= 1) {
			part[index].setVy(abs(part[index].getVy()));
			part[index].setY(1);
		}
	}
}

//clears and places the updated particles into their correct grid positions
void clearAndFix(vector<Particles>& part, vector<vector<vector<Particles*>>>& grid, int width) {
	int row, col;

	//assign every particle a grid cell based on its position
	for (int index = 0; index < part.size(); index++) {
		row = part[index].getY() / width;
		col = part[index].getX() / width;

		int nR = grid.size(), nC = grid[0].size();

		//safeguards to make sure that every particle gets assigned a valid cell 

		if (row < 0) {
			row = 0;
		}
		else if (row >= nR) {
			row = nR-1;
		}
		if (col < 0) {
			col = 0;
		}
		else if (col >= nC) {
			col = nC-1;
		}

		int r = part[index].getRow(), c = part[index].getCol();

		//if the particle's row or column is different from where it was before in the grid, then it needs to be moved to it's correct cell coordinates
		if (r != row || c != col) {

			//for readability and to loop though the grid cell faster use a reference for grid[r][c] instead of recalculating it multiple times 
			vector<Particles*>&oldCell = grid[r][c];

			for (int i = 0; i < oldCell.size(); i++) {
				if (oldCell[i] == &part[index]) {
					oldCell[i] = oldCell.back();
					oldCell.pop_back();
					break;
				}
			}

			part[index].setRow(row);
			part[index].setCol(col);

			grid[row][col].emplace_back(&part[index]);

		}

	}
}

//checks if particles collided with each other by passing an active grid cell and looping through its particles
void particleCollis(vector<Particles*>& grid) {
	float d, dx, dy;

	for (int start = 0; start < grid.size() - 1; start++) {
		for (int index = start + 1; index < grid.size(); index++) {

			//compute dy and dx to figure out if they collided on x or y axis
			dy = grid[index]->getY() - grid[start]->getY();
			dx = grid[index]->getX() - grid[start]->getX();

			//absolute distance
			float absDx = abs(dx);
			float absDy = abs(dy);

			//if distance^2 is less than the particle radius^2: then they collided. Using squared to budget cpu resources and be accurate at the same time
			if (absDx * absDx + absDy * absDy < 100) {

				// Calculate relative velocity
				float dvx = grid[index]->getVx() - grid[start]->getVx();
				float dvy = grid[index]->getVy() - grid[start]->getVy();

				//only swap velo if they are moving towards eachother. Determines if they are pointing to eachother and acts accordingly
				if (dx * dvx + dy * dvy < 0) {

					if (absDx <= absDy) {
						float tempVy = grid[start]->getVy();
						grid[start]->setVy(grid[index]->getVy());
						grid[index]->setVy(tempVy);
					}
					else {
						float tempVx = grid[start]->getVx();
						grid[start]->setVx(grid[index]->getVx());
						grid[index]->setVx(tempVx);
					}
				}
			}
		}
	}
}

//saves particle state into csv file
void csvDump(vector<Particles>& part, std::fstream& file, float time) {
	for (int index = 0; index < part.size(); index++) {
		file << to_string(index + 1) << "," << to_string(part[index].getX()) << "," << to_string(part[index].getY()) << "," << to_string(part[index].getVx())
			<< "," << to_string(part[index].getVy()) << "," << to_string(part[index].getA()) << "," << "," << to_string(part[index].getRow()) << "," << to_string(part[index].getCol()) << ", " << to_string(time) << "\n";
	}
}