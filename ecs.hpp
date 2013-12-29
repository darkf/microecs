/* Micro Entity-Component-System (ECS) framework
   Copyright (c) 2013 darkf
   Licensed under the terms of the zlib license
*/

#ifndef ECS_H
#define ECS_H

#include <algorithm>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <vector>

/* Components are data stores. They don't contain logic, they're just models. */
class Component {};

/* Entities are objects that are a bag of components */
class Entity {
	using ComponentPtr = std::unique_ptr<Component, void(*)(Component*)>;
	private:
	std::unordered_map<const std::type_info*, ComponentPtr> componentMap;

	template<typename T>
	static ComponentPtr MakeComponentPtr(T* ptr) {
		return ComponentPtr(ptr, [](Component* p) { delete static_cast<T*>(p); });
	}

	public:
	template<typename T>
	T* get() const {
		static_assert(std::is_base_of<Component, T>::value, "Entity::get needs a subclass of Component");
		return static_cast<T*>(get(&typeid(T)));
	}

	Component* get(const std::type_info* ti) const {
		auto i = componentMap.find(ti);
		if(i == componentMap.end())
			return nullptr;
		return i->second.get();
	}

	template<typename T, typename... Args>
	void emplace(Args&&... args) {
		static_assert(std::is_base_of<Component, T>::value, "Entity::emplace needs a subclass of Component");
		auto ptr = new T(std::forward<Args>(args)...);
		auto component = MakeComponentPtr(ptr);
		componentMap.emplace(&typeid(T), std::move(component));
	}

	template<typename T>
	void insert(T* c) {
		static_assert(std::is_base_of<Component, T>::value, "Entity::insert needs a subclass of Component");
		static_assert(!std::is_same<Component, T>::value, "Entity::insert needs a specific Component");
		componentMap.emplace(&typeid(T), MakeComponentPtr(c));
	}

	template<typename T>
	void erase() {
		static_assert(std::is_base_of<Component, T>::value, "Entity::erase needs a subclass of Component");
		erase(&typeid(T));
	}

	void erase(const std::type_info* ti) {
		componentMap.erase(ti);
	}

	template<typename T>
	bool contains() const {
		static_assert(std::is_base_of<Component, T>::value, "Entity::contains needs a subclass of Component");
		return contains(&typeid(T));
	}

	bool contains(const std::type_info* ti) const {
		return componentMap.find(ti) != componentMap.end();
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

template<typename... Components>
class System {
	static_assert(is_base_of_all<Component, Components...>::value, "All components need to be a subclass of Component");
	public:
	std::vector<const std::type_info*> componentTypes;
	System() : componentTypes {&typeid(Components)...} {}

	// where the magic happens
	virtual void logic(Entity& e) = 0;

	void process(Entity& e) {
		if (std::all_of(std::begin(componentTypes), std::end(componentTypes), [&](auto type) { return e.contains(type); }))
			logic(e);
	}

	void process(std::vector<Entity>& entities) {
		for(Entity& e : entities) process(e);
	}
};

#endif
