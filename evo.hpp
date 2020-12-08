#include "vec.hpp"

class game {
public:
	// number of players
	int pop;
	
	// amount of food generated per second
	int food;
	
	// amount of food on the play field
	int food_n;
	
	// size of the play field
	int rad;
	
	// size of the cicle in which food is generated
	int food_rad;
	
	// size of the circle around a player in which food can be picked up, squared
	int sqr_p_r;
	
	game();
	game(int, int, int, int, int, int);
};

class player {
public:
	// position
	vecd2 p;
	
	// remaining energy
	int energy;
	
	// Whether the current path is towards food
	bool found_food;
	
	// Change in position since last turn
	vecd2 v;
	
	// progress along current path
	float t;
	
	// approximate total length of current path
	float l;
	
	// point this creature is on a ptah from
	vecd2 s;
	
	// current point this creature is on a path to
	vecd2 d;
	
	// control point used in quadratic bezier curves controlling paths
	vecd2 c;
	
	// stats
	int speed;
	int sense;
	
	player();
	player(int, int);
	
	// Change this player's destination
	void go(vecd2 d);
};

#include "evo.cpp"










