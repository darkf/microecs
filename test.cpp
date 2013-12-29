#include "ecs.hpp"

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
		auto pos = e.GetComponent<PositionComponent>();
		auto vel = e.GetComponent<VelocityComponent>();
		pos->x += vel->vx;
		pos->y += vel->vy;
	}
};

void printPosVel(Entity& e) {
	auto pos = e.GetComponent<PositionComponent>();
	auto vel = e.GetComponent<VelocityComponent>();
	cout << "pos: " << pos->x << ", " << pos->y << " ";
	cout << "vel: " << vel->vx << ", " << vel->vy << endl;
}

int main() {
	VelocitySystem s;

	Entity e;
	e.AddComponent(new PositionComponent(32, 64));
	e.AddComponent(new VelocityComponent(1, 2));

	auto pos = e.GetComponent<PositionComponent>();
	auto vel = e.GetComponent<VelocityComponent>();

	printPosVel(e);
	s.process(e);
	printPosVel(e);

	delete pos;
	delete vel;
	return 0;
}
