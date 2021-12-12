#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Physics.h"

#include <sstream>

using namespace BlocksEngine;

Physics::PhysicsSettings::PhysicsSettings()
    : gravity{Vector3{0.0f, -9.807f, 0.0f}},
      persistentContactDistanceThreshold{0.03f},
      defaultFrictionCoefficient{0.3f},
      defaultBounciness{0.5f},
      restitutionVelocityThreshold{0.5f},
      defaultRollingResistance{0.0f},
      isSleepingEnabled{true},
      defaultVelocitySolverIterationsCount{10},
      defaultPositionSolverIterationsCount{5},
      defaultTimeBeforeSleep{1.0f},
      defaultSleepLinearVelocity{0.02f},
      defaultSleepAngularVelocity{3.0f * (Math::PI / 180.0f)},
      maxContactManifoldsCount{3},
      cosAngleSimilarContactManifold{0.95f}
{
}

std::string Physics::PhysicsSettings::ToString() const
{
    std::stringstream ss;

    ss << "gravity=" << gravity.ToString() << '\n';
    ss << "persistentContactDistanceThreshold=" << persistentContactDistanceThreshold << '\n';
    ss << "defaultFrictionCoefficient=" << defaultFrictionCoefficient << '\n';
    ss << "defaultBounciness=" << defaultBounciness << '\n';
    ss << "restitutionVelocityThreshold=" << restitutionVelocityThreshold << '\n';
    ss << "defaultRollingResistance=" << defaultRollingResistance << '\n';
    ss << "isSleepingEnabled=" << isSleepingEnabled << '\n';
    ss << "defaultVelocitySolverIterationsCount=" << defaultVelocitySolverIterationsCount << '\n';
    ss << "defaultPositionSolverIterationsCount=" << defaultPositionSolverIterationsCount << '\n';
    ss << "defaultTimeBeforeSleep=" << defaultTimeBeforeSleep << '\n';
    ss << "defaultSleepLinearVelocity=" << defaultSleepLinearVelocity << '\n';
    ss << "defaultSleepAngularVelocity=" << defaultSleepAngularVelocity << '\n';
    ss << "maxContactManifoldsCount=" << maxContactManifoldsCount << '\n';
    ss << "cosAngleSimilarContactManifold=" << cosAngleSimilarContactManifold << std::endl;

    return ss.str();
}
