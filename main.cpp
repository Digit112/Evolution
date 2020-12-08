#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "darray.hpp"

#include "evo.hpp"

int play(int p, int r, int f, int f_n, int f_r, int p_r, int frames) {
	printf("Initializing game.\n");
	
	game g(p, r, f, f_n, f_r, p_r*p_r);
	darray<player> players = darray<player>(g.pop);
	darray<vecd2> food = darray<vecd2>(100);
	
	float a;
	float R;
	for (int i = 0; i < g.food_n; i++) {
		a = fmod(rand() / 100000.0, 2*3.14159);
		R = rand() % g.food_rad;
		food[i] = vecd2(R*cos(a), R*sin(a));
	}
	
	for (int i = 0; i < g.pop; i++) {
		printf("Initializing player %d\n", i);
		// Initialize stats
		players[i].speed = 8;
		players[i].sense = 50;
		
		// Initialize energy
		players[i].energy = 10000;
		
		// Initialize position
		a = (float) i / g.pop * 2 * 3.14159;
		players[i].p = vecd2(cos(a) * g.rad, sin(a) * g.rad);
		
		// Initialize velocity
		a += 3.14159;
		players[i].v = vecd2(cos(a) * players[i].speed, sin(a) * players[i].speed);
		
		// Initialize path
		players[i].t = 0;
		players[i].s = players[i].p;
		players[i].c = players[i].p + players[i].v*5;
		// Pick a random destination. Rotationally symetric.
		a = fmodf(rand() / 10000.0, 2*3.14159);
		R = rand() % g.rad;
		players[i].d = vecd2(cos(a) * R, sin(a) * R);
		// Calculate approximate length
		players[i].l = bez2len(players[i].s, players[i].c, players[i].d);
		printf("Creating player at (%.2f, %.2f)\n", players[i].p.x, players[i].p.y);
		printf("Directing to (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", players[i].s.x, players[i].s.y, players[i].c.x, players[i].c.y, players[i].d.x, players[i].d.y);
	}
	
	float timer = 0;
	float sperf = 1.0/g.food;
	
	vecd2 prev;
	for (int f = 0; f < frames; f++) {
		printf("Processing frame %d\n", f);
		
		// Move timer forward
		timer += 1.0/24;
		
		if (timer > sperf) {
			timer -= sperf;
			a = fmod(rand() / 100000.0, 2*3.14159);
			R = rand() % g.food_rad;
			food[g.food_n] = vecd2(R*cos(a), R*sin(a));
			g.food_n++;
		}
		
		// Check for extinction
		if (g.pop == 0) {
			printf("Extinct on frame %d.\n", f);
			return(0);
		}
		
		for (int p = 0; p < g.pop; p++) {
			// Calculate movement
			// Check if movement is complete, if so change destination to a random point
			if (players[p].t >= 1) {
				// Pick a random destination. Rotationally symetric.
				a = fmodf(rand() / 10000.0, 2*3.14159);
				r = rand() % g.rad;
				players[p].go(vecd2(cos(a) * r, sin(a) * r));
				players[p].found_food = false;
				printf("Redirecting to (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f).\n", players[p].s.x, players[p].s.y, players[p].c.x, players[p].c.y, players[p].d.x, players[p].d.y);
			}
			// Find the nearest food
			vecd2 nearest = vecd2(0, 0);
			int nearind;
			float mindis = g.rad*g.rad*4;
			float sqrdis;
			for (int F = 0; F < g.food_n; F++) {
				sqrdis = (food[F] - players[p].p).sqr_mag();
				if (sqrdis < mindis) {
					mindis = sqrdis;
					nearest = food[F];
					nearind = F;
				}
			}
			// If the nearest food is close enough to pick up, do so
			if (mindis < g.sqr_p_r) {
				food.remove(nearind);
				g.food_n--;
				players[p].energy += 10000;
			}
			// If the nearest food is closer than the sensing radius, go to it.
			else if (mindis < players[p].sense*players[p].sense and not players[p].found_food) {
				players[p].go(nearest);
				players[p].found_food = true;
			}
			
			// Update position and velocity
			players[p].t += players[p].speed / players[p].l;
			prev = players[p].p;
			players[p].p = bezier2(players[p].s, players[p].c, players[p].d, players[p].t);
			players[p].v = players[p].p - prev;
			printf("%.2f: (%.2f, %.2f)\n", players[p].t, players[p].p.x, players[p].p.y);
			
			// Energy cost
			players[p].energy-=(players[p].sense*players[p].sense*0.2*0.2 + players[p].speed*players[p].speed)*0.5 + (rand() % 1000 - 500);
			
			// Check if the player dies or can replicate.
			if (players[p].energy < 0) {
				players.remove(p);
				g.pop--;
			}
			else if (players[p].energy > 20000) {
				players.append( player( fmax(1, players[p].speed + (rand() % 2 - 1)), fmax(5, players[p].sense + (rand() % 10 - 5)) ) );
				int newp = g.pop;
				g.pop++;
				players[p].energy -= 10000;
				players[newp].p = players[p].p;
				players[newp].v = players[p].v;
				players[newp].energy = 10000;
				a = fmodf(rand() / 10000.0, 2*3.14159);
				r = rand() % g.rad;
				players[newp].go(vecd2(cos(a) * r, sin(a) * r));
			}
		}
		
		// Save frame
		char* fn = new char[32];
		sprintf(fn, "animation/%d.bin", f);
		FILE* fp = fopen(fn, "w+");
		delete[] fn;
		fwrite(&g.pop, 1, 4, fp);
		fwrite(&g.food_n, 1, 4, fp);
		for (int p = 0; p < g.pop; p++) {
			fwrite(&players[p].energy, 1, 4, fp);
			fwrite(&players[p].speed, 1, 4, fp);
			fwrite(&players[p].sense, 1, 4, fp);
			fwrite(&players[p].p.x, 1, 8, fp);
			fwrite(&players[p].p.y, 1, 8, fp);
			fwrite(&players[p].s.x, 1, 8, fp);
			fwrite(&players[p].s.y, 1, 8, fp);
			fwrite(&players[p].c.x, 1, 8, fp);
			fwrite(&players[p].c.y, 1, 8, fp);
			fwrite(&players[p].d.x, 1, 8, fp);
			fwrite(&players[p].d.y, 1, 8, fp);
		}
		for (int f = 0; f < g.food_n; f++) {
			fwrite(&food[f].x, 1, 8, fp);
			fwrite(&food[f].y, 1, 8, fp);
		}
		fclose(fp);
	}
}

int main() {
	// population, radius, food, food_n, food_rad, pickup_rad, frames
	play(10, 400, 2, 1, 300, 10, 4000);
	return 0;
}
