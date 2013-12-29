#ifndef _ECS_H
#define _ECS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <type_traits>
using std::cout;
using std::endl;

class Component {};

class Entity {
	protected:
	std::unordered_map<const std::type_info*, Component*> componentMap;

	public:
	template<typename T>
	T* GetComponent() const {
		static_assert(std::is_base_of<Component, T>::value, "GetComponent needs a subclass of Component");
		auto i = componentMap.find(&typeid(T));
		if(i == componentMap.end())
			return nullptr;
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

	template<typename T>
	bool HasComponent() {
		static_assert(std::is_base_of<Component, T>::value, "HasComponent needs a subclass of Component");
		return componentMap.find(&typeid(T)) != componentMap.end();
	}

	bool HasComponent(const std::type_info* ti) {
		return componentMap.find(ti) != componentMap.end();
	}

	std::vector<std::pair<const std::type_info*, Component*>> ComponentPairs() {
		std::vector<std::pair<const std::type_info*, Component*>> components(componentMap.size());
		std::copy(componentMap.begin(), componentMap.end(), components.begin());
		return components;
	}
};

#endif