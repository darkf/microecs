/* Micro Entity-Component-System (ECS) framework
   Copyright (c) 2013 darkf
   Licensed under the terms of the zlib license
*/

#ifndef ECS_H
#define ECS_H

#include <algorithm>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <vector>

/* Components are data stores. They don't contain logic, they're just models. */
class Component {};

/* Entities are objects that are a bag of components */
class Entity {
	private:
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
		componentMap.emplace(&typeid(T), static_cast<Component*>(c));
	}

	template<typename T>
	void RemoveComponent() {
		static_assert(std::is_base_of<Component, T>::value, "RemoveComponent needs a subclass of Component");
		componentMap.erase(&typeid(T));
	}

	template<typename T>
	bool HasComponent() const {
		static_assert(std::is_base_of<Component, T>::value, "HasComponent needs a subclass of Component");
		return HasComponent(&typeid(T));
	}

	bool HasComponent(const std::type_info* ti) const {
		return componentMap.find(ti) != componentMap.end();
	}

	std::vector<std::pair<const std::type_info*, Component*>> ComponentPairs() const {
		return std::vector<std::pair<const std::type_info*, Component*>>(componentMap.begin(), componentMap.end());
	}
};

/* Systems are things that work on entities containing
   specific component types, and performing logic on them.
*/

template<typename Base, typename... Deriveds>
struct is_base_of_all;

template<typename Base, typename Derived, typename... Deriveds>
struct is_base_of_all<Base, Derived, Deriveds...> :
	std::integral_constant<bool, std::is_base_of<Base, Derived>::value && is_base_of_all<Base, Deriveds...>::value>
{};

template<typename Base>
struct is_base_of_all<Base> : std::true_type {};

// note: there is no type-check on the component types
// so if they are not subclasses of Component, nothing will
// error.
template<typename... Components>
class System {
	static_assert(is_base_of_all<Component, Components...>::value, "All components need to be a subclass of Component.");
	public:
	std::vector<const std::type_info*> componentTypes;
	System() : componentTypes {&typeid(Components)...} {}

	// where the magic happens
	virtual void logic(Entity& e) = 0;

	void process(Entity& e) {
		if (std::all_of(std::begin(componentTypes), std::end(componentTypes), [&](auto type) { return e.HasComponent(type); }))
			logic(e);
	}

	void process(std::vector<Entity>& entities) {
		for(Entity& e : entities) process(e);
	}
};

#endif
