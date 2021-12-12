#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Engine/Material.h"

BlocksEngine::Material::Material(const float frictionCoefficient, const float rollingResistance,
                                 const float bounciness, const float massDensity)
    : frictionCoefficient_{frictionCoefficient},
      rollingResistance_{rollingResistance},
      bounciness_{bounciness},
      massDensity_{massDensity}
{
}

BlocksEngine::Material::Material(const Material& material) = default;

BlocksEngine::Material::Material(Material&& material) noexcept
    : frictionCoefficient_{std::exchange(material.frictionCoefficient_, 0)},
      rollingResistance_{std::exchange(material.rollingResistance_, 0)},
      bounciness_{std::exchange(material.bounciness_, 0)},
      massDensity_{std::exchange(material.massDensity_, 0)}
{
}
