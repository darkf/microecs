#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <type_traits>
using std::cout;
using std::endl;

class Component {};

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

class Entity {
	protected:
	std::unordered_map<const std::type_info*, Component*> componentMap;

	public:
	template<typename T>
	T* GetComponent() const {
		static_assert(std::is_base_of<Component, T>::value, "GetComponent needs a subclass of Component");
		auto i = componentMap.find(&typeid(T));
		if(i == componentMap.end())
			throw "component not found";
		return static_cast<T*>(i->second);
	}

	template<typename T>
	void AddComponent(T* c) {
		static_assert(std::is_base_of<Component, T>::value, "AddComponent needs a subclass of Component");
		componentMap.insert({&typeid(T), static_cast<Component*>(c)});
	}

	template<typename T>
	void RemoveComponent() {
		static_assert(std::is_base_of<Component, T>::value, "RemoveComponent needs a subclass of Component");
		componentMap.erase(&typeid(T));
	}
};

class X{};

int main() {
	Entity e;
	e.AddComponent(new PositionComponent(32, 64));
	e.AddComponent(new VelocityComponent(1, 2));
	auto pos = e.GetComponent<PositionComponent>();
	auto vel = e.GetComponent<VelocityComponent>();
	cout << pos->x << ", " << pos->y << endl;
	cout << vel->vx << ", " << vel->vy << endl;
	delete pos;
	delete vel;
	return 0;
}
