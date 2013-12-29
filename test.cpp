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

int main() {
	Entity e;
	e.AddComponent(new PositionComponent(32, 64));
	e.AddComponent(new VelocityComponent(1, 2));
	auto pos = e.GetComponent<PositionComponent>();
	auto vel = e.GetComponent<VelocityComponent>();
	cout << pos->x << ", " << pos->y << endl;
	cout << vel->vx << ", " << vel->vy << endl;
	for(auto c : e.ComponentPairs()) cout << c.first->name() << endl;
	delete pos;
	delete vel;
	return 0;
}
