// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: PhysicsMaterial.h

#pragma once

#include <sstream>

#include "BlocksEngine/Graphics/Material/Material.h"

namespace BlocksEngine
{
    class PhysicsMaterial;
}

class BlocksEngine::PhysicsMaterial
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    PhysicsMaterial& operator=(const PhysicsMaterial& material);
    PhysicsMaterial& operator=(PhysicsMaterial&& material) noexcept;

    ~PhysicsMaterial() = default;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    [[nodiscard]] float GetBounciness() const;

    void SetBounciness(float bounciness);

    [[nodiscard]] float GetFrictionCoefficient() const;

    void SetFrictionCoefficient(float frictionCoefficient);

    [[nodiscard]] float GetRollingResistance() const;

    void SetRollingResistance(float rollingResistance);

    [[nodiscard]] float GetMassDensity() const;

    void SetMassDensity(float massDensity);

    [[nodiscard]] std::string ToString() const;

private:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    PhysicsMaterial(float frictionCoefficient, float rollingResistance, float bounciness, float massDensity = 1.0f);
    PhysicsMaterial(const PhysicsMaterial& material);
    PhysicsMaterial(PhysicsMaterial&& material) noexcept;

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    float frictionCoefficient_;

    float rollingResistance_;

    float bounciness_;

    float massDensity_;
};


inline float BlocksEngine::PhysicsMaterial::GetBounciness() const
{
    return bounciness_;
}


inline void BlocksEngine::PhysicsMaterial::SetBounciness(const float bounciness)
{
    assert(bounciness >= 0.0f && bounciness <= 1.0f);
    bounciness_ = bounciness;
}

inline float BlocksEngine::PhysicsMaterial::GetFrictionCoefficient() const
{
    return frictionCoefficient_;
}

inline void BlocksEngine::PhysicsMaterial::SetFrictionCoefficient(const float frictionCoefficient)
{
    assert(frictionCoefficient >= 0.0f);
    frictionCoefficient_ = frictionCoefficient;
}

inline float BlocksEngine::PhysicsMaterial::GetRollingResistance() const
{
    return rollingResistance_;
}

inline void BlocksEngine::PhysicsMaterial::SetRollingResistance(const float rollingResistance)
{
    assert(rollingResistance >= 0.0f);
    rollingResistance_ = rollingResistance;
}

inline float BlocksEngine::PhysicsMaterial::GetMassDensity() const
{
    return massDensity_;
}

inline void BlocksEngine::PhysicsMaterial::SetMassDensity(const float massDensity)
{
    massDensity_ = massDensity;
}

inline std::string BlocksEngine::PhysicsMaterial::ToString() const
{
    std::stringstream ss;

    ss << "Friction Coefficient = " << frictionCoefficient_ << '\n';
    ss << "Rolling Resistance = " << rollingResistance_ << '\n';
    ss << "Bounciness = " << bounciness_ << std::endl;

    return ss.str();
}

inline BlocksEngine::PhysicsMaterial& BlocksEngine::PhysicsMaterial::operator=(const PhysicsMaterial& material)
{
    if (this != &material)
    {
        frictionCoefficient_ = material.frictionCoefficient_;
        bounciness_ = material.bounciness_;
        rollingResistance_ = material.rollingResistance_;
    }

    return *this;
}

inline BlocksEngine::PhysicsMaterial& BlocksEngine::PhysicsMaterial::operator=(PhysicsMaterial&& material) noexcept
{
    if (this != &material)
    {
        frictionCoefficient_ = std::exchange(material.frictionCoefficient_, 0);
        bounciness_ = std::exchange(bounciness_, 0);
        rollingResistance_ = std::exchange(rollingResistance_, 0);
        // TODO: Shouldn't we set the mass density too?
        material.massDensity_ = 0;
    }

    return *this;
}
