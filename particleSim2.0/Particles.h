#ifndef PARTICLES_H
#define PARTICLES_H

#include<vector>

//Particle struct
struct Particles {
	double vx = 0;
	double vy;
	double x;
	double y;
	double a = -9.8;
	double ep;
	double ek;
	double em;
	double mass = 0.5;

};

class Spring {
private:
	double k = 10;
	double h = 6;

public:
	double getK() const {
		return k;
	}

	void setK(double num) {
		k = num;
	}

	double getHeight() const {
		return h;
	}

};

//functions
void updatePos(std::vector<Particles>&, Spring&);
void wallCollis(std::vector<Particles>&);
void clearAndFix(std::vector<Particles>&, std::vector<std::vector<std::vector<Particles*>>>&);
void particleCollis(std::vector<Particles*>&);
void newVelo(Particles&, double, double);
void veloManagment(std::vector<Particles>&, Spring&);
void calcEM(std::vector<Particles>&);

#endif


