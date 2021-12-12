#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Collider.h"

using namespace BlocksEngine;


Collider::Collider(std::shared_ptr<CollisionBody> body)
    : Component{},
body_{std::move(body)},
material_{body->}
{
}
