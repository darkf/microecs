/* Micro Entity-Component-System (ECS) framework
   Copyright (c) 2013 darkf
   Licensed under the terms of the zlib license
*/

#ifndef _ECS_H
#define _ECS_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <type_traits>
using std::cout;
using std::endl;

/* Components are data stores. They don't contain logic, they're just models. */
class Component {};

/* Entities are objects that are a bag of components */
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

/* Systems are things that work on entities containing
   specific component types, and performing logic on them.
*/

// note: there is no type-check on the component types
// so if they are not subclasses of Component, nothing will
// error.
template<typename... Components>
class System {
	public:
	std::vector<const std::type_info*> componentTypes;
	System() : componentTypes {&typeid(Components)...} {}

	// where the magic happens
	virtual void logic(Entity& e) = 0;

	void process(Entity& e) {
		for(auto type : componentTypes) {
			if(!e.HasComponent(type))
				return;
		}
		logic(e);
	}

	void process(std::vector<Entity>& entities) {
		for(Entity& e : entities) process(e);
	}
};

#endif