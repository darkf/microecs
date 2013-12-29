#include "ecs.hpp"
#include <iostream>

using std::cout;
using std::endl;

class PositionComponent : public Component {
	public:
	int x, y;
	PositionComponent(int x, int y) : x(x), y(y) {}
};

class VelocityComponent : public Component {
	public:
	int vx, vy;
	VelocityComponent(int vx, int vy) : vx(vx), vy(vy) {}
};

class VelocitySystem : public System<PositionComponent, VelocityComponent> {
	public:
	VelocitySystem() {}

	void logic(Entity& e) {
		auto pos = e.get<PositionComponent>();
		auto vel = e.get<VelocityComponent>();
		pos->x += vel->vx;
		pos->y += vel->vy;
	}
};

void printPosVel(Entity const& e) {
	auto pos = e.get<PositionComponent>();
	auto vel = e.get<VelocityComponent>();
	cout << "pos: " << pos->x << ", " << pos->y << " ";
	cout << "vel: " << vel->vx << ", " << vel->vy << endl;
}

int main() {
	VelocitySystem s;

	Entity e;
	e.insert(new PositionComponent(32, 64));
	e.emplace<VelocityComponent>(1, 2);

	printPosVel(e);
	s.process(e);
	printPosVel(e);
}
