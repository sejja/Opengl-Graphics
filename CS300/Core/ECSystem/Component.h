#pragma once
#include <memory>
#include "Object.h"

struct Object;

class Component {
public:
	Component(std::weak_ptr<Object> parent) : mParent(parent) {}
	virtual void Update() {}
	~Component() = default;
	std::weak_ptr<Object> mParent;
};