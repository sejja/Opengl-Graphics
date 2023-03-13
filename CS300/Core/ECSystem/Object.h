#pragma once
#include <memory>
#include <vector>
#include "Core/Math/Transform.h"

struct Component;

struct Object {
	Mathematics::Transformation transform;
	std::vector<std::shared_ptr<Component>> components;
};