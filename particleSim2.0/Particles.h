#ifndef PARTICLES_H
#define PARTICLES_H

#include<vector>

//Particle struct
struct Particles {
	float vx = 0;
	float vy;
	float x;
	float y;
	float a = -9.8;
	float ep;
	float ek;
	float em;
	float mass = 0.5;
};


class Spring {
private:
	float k;
	float h = 6;

public:
	float getK() const {
		return k;
	}

	void setK(float num) {
		k = num;
	}

	float getHeight() const {
		return h;
	}

};

//functions
void updatePos(std::vector<Particles>&, Spring&);
void wallCollis(std::vector<Particles>&);
void clearAndFix(std::vector<Particles>&, std::vector<std::vector<std::vector<Particles*>>>&);
void particleCollis(std::vector<Particles*>&

#endif


