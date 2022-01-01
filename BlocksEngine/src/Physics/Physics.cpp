#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Physics.h"

#include "BlocksEngine/Exceptions/EngineException.h"

using namespace BlocksEngine;

Physics::Physics(const bool recordMemoryAllocations, const bool connectVisualDebugger)
{
    foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator_, defaultErrorCallback_);

    if (!foundation_)
    {
        throw ENGINE_EXCEPTION("Could not create Physx Foundation");
    }

    if (connectVisualDebugger)
    {
        pvd_ = PxCreatePvd(*foundation_);
        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        pvd_->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
    }

    physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, physx::PxTolerancesScale(), recordMemoryAllocations,
                               pvd_);

    if (!physics_)
    {
        throw ENGINE_EXCEPTION("Could not create Physics");
    }

    physx::PxSceneDesc sceneDesc{physics_->getTolerancesScale()};
    sceneDesc.gravity = {0.0f, -9.81f, 0.0f};
    cpuDispatcher_ = physx::PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = cpuDispatcher_;
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    scene_ = physics_->createScene(sceneDesc);
    scene_->getScenePvdClient()->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);

    defaultMaterial_ = physics_->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxRigidStatic* groundPlane = PxCreatePlane(*physics_, physx::PxPlane{0, 1, 0, 0}, *defaultMaterial_);
    scene_->addActor(*groundPlane);

    cooking_ = PxCreateCooking(PX_PHYSICS_VERSION, *foundation_,
                               physx::PxCookingParams(physics_->getTolerancesScale()));
}

Physics::~Physics()
{
    PX_RELEASE(scene_);
    PX_RELEASE(cpuDispatcher_);

    PX_RELEASE(physics_);
    if (pvd_)
    {
        physx::PxPvdTransport* transport = pvd_->getTransport();
        pvd_->release();
        pvd_ = nullptr;
        PX_RELEASE(transport);
    }
    PX_RELEASE(foundation_);
}

void Physics::Update() const
{
    constexpr float delta = 1.0f / 60.0f;
    scene_->simulate(delta);
    scene_->fetchResults(true);
}

physx::PxPhysics& Physics::GetPhysics()
{
    return *physics_;
}

physx::PxScene& Physics::GetScene()
{
    return *scene_;
}

physx::PxCooking& Physics::GetCooking()
{
    return *cooking_;
}

physx::PxMaterial& Physics::DefaultMaterial()
{
    return *defaultMaterial_;
}
