#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Engine/PhysicsMaterial.h"

BlocksEngine::PhysicsMaterial::PhysicsMaterial(const float frictionCoefficient, const float rollingResistance,
                                               const float bounciness, const float massDensity)
    : frictionCoefficient_{frictionCoefficient},
      rollingResistance_{rollingResistance},
      bounciness_{bounciness},
      massDensity_{massDensity}
{
}

BlocksEngine::PhysicsMaterial::PhysicsMaterial(const PhysicsMaterial& material) = default;

BlocksEngine::PhysicsMaterial::PhysicsMaterial(PhysicsMaterial&& material) noexcept
    : frictionCoefficient_{std::exchange(material.frictionCoefficient_, 0)},
      rollingResistance_{std::exchange(material.rollingResistance_, 0)},
      bounciness_{std::exchange(material.bounciness_, 0)},
      massDensity_{std::exchange(material.massDensity_, 0)}
{
}
