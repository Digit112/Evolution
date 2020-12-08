// 2D Linear interpolation
vecd2 lerp(vecd2 a, vecd2 b, float t) {
	return vecd2((b.x - a.x)*t + a.x, (b.y - a.y)*t + a.y);
}

// Quadratic Bezier interpolation
vecd2 bezier2(vecd2 a, vecd2 b, vecd2 c, float t) {
	return lerp(lerp(a, b, t), lerp(b, c, t), t);
}

// Approximate the length of a quadratic bezier curve.
float bez2len(vecd2 a, vecd2 b, vecd2 c) {
	vecd2 p = bezier2(a, b, c, 0);
	vecd2 q;
	float dis = 0;
	for (float i = 0.01; i <= 1; i += 0.01) {
		q = bezier2(a, b, c, i);
		dis += (p-q).mag();
		p = q;
	}
	return dis;
}

game::game() : pop(0), food(0), food_n(0), food_rad(0), rad(0), sqr_p_r(0) {}
game::game(int pop, int rad, int food, int food_n, int food_rad, int sqr_p_r) : pop(pop), food(food), food_n(food_n), food_rad(food_rad), rad(rad), sqr_p_r(sqr_p_r) {}

player::player() : p(0, 0), found_food(false), energy(0), v(0, 0), t(0), l(0), s(0, 0), d(0, 0), c(0, 0), speed(0), sense(0) {}
player::player(int speed, int sense) : p(0, 0), found_food(false), energy(0), v(0, 0), t(0), l(0), s(0, 0), d(0, 0), c(0, 0), speed(speed), sense(sense) {}

void player::go(vecd2 dest) {
	s = p;
	c = p + v*5;
	d = dest;
	t = 0;
	l = bez2len(s, c, d);
}
