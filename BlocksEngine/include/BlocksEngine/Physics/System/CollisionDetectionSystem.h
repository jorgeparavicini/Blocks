// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: CollisionDetectionSystem.h

#pragma once
#include <robin_hood.h>

#include "BlocksEngine/Physics/Collision/Collider.h"
#include "BlocksEngine/Physics/Collision/RaycastInfo.h"

namespace BlocksEngine
{
    class CollisionDetectionSystem;
}

class BlocksEngine::CollisionDetectionSystem
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    CollisionDetectionSystem();

    CollisionDetectionSystem(const CollisionDetectionSystem&) = delete;
    CollisionDetectionSystem& operator=(const CollisionDetectionSystem&) = delete;

    CollisionDetectionSystem(CollisionDetectionSystem&&) = delete;
    CollisionDetectionSystem& operator=(CollisionDetectionSystem&&) = delete;

    ~CollisionDetectionSystem() = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    CollisionDispatch& GetCollisionDispatch();

    void AddCollider(std::shared_ptr<Collider> collider, const AABB& aabb);

    void RemoveCollider(std::shared_ptr<Collider> collider);

    void UpdateCollider(std::shared_ptr<Actor> colliderActor, float timeStep);

    void UpdateCollider(float timeStep);

    void AddNoCollisionPair(std::shared_ptr<Actor> body1Actor, std::shared_ptr<Actor> body2Actor);

    void RemoveNoCollisionPair(std::shared_ptr<Actor> body1Actor, std::shared_ptr<Actor> body2Actor);

    void AskForBroadPhaseCollisionCheck(std::shared_ptr<Collider> collider);

    void NotifyOverlappingPairsToTestOverlap(std::shared_ptr<Collider> collider);

    void ReportContactsAndTriggers();

    void ComputeCollisionDetection();

    void Raycast(std::shared_ptr<RaycastCallback> raycastCallback, const Ray& ray,
                 unsigned short raycastWithCategoryMaskBits) const;

    void TestOverlap(std::shared_ptr<CollisionBody> body1, std::shared_ptr<CollisionBody> body2);

    void TestOverlap(std::shared_ptr<CollisionBody> body, OverlapCallback& callback);

    void TestOverlap(OverlapCallback& overlapCallback);

    void TestCollision(std::shared_ptr<CollisionBody> body1, std::shared_ptr<CollisionBody> body2,
                       CollisionCallback& callback);

    void TestCollision(std::shared_ptr<CollisionBody> body, CollisionCallback& callback);

    void TestCollision(CollisionCallback& callback);

    std::shared_ptr<EventListener> GetWorldEventListener();

    const AABB GetWorldAABB(const Collider& collider) const;

private:
    using OverlappingPairMap = robin_hood::unordered_map<std::pair<int, int>, std::shared_ptr<OverlappingPair>>;


    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::vector<Collider> colliders_;
    std::unique_ptr<CollisionDispatch> collisionDispatch_;
    robin_hood::unordered_set<bodypair> noCollisionPairs_;
    OverlappingPairs overlappingPairs_;
    BroadPhaseSystem broadPhaseSystem_;
    robin_hood::unordered_map<int, std::weak_ptr<Actor>> mapBroadPhaseIdToColliderActor_;
    NarrowPhaseInput narrowPhaseInput_;
    std::vector<ContactPointInfo> potentialContactPoints_;
    std::vector<ContactManifoldInfo> potentialContactManifolds_;
    std::vector<ContactPair> contactsPairs1_;
    std::vector<ContactPair> contactPairs2_;
    std::shared_ptr<std::vector<ContactPair>> previousContactPairs_;
    std::shared_ptr<std::vector<ContactPair>> currentContactPairs_;
    std::vector<ContactPair> lostContactPairs_;
    robin_hood::unordered_map<int64_t, int> mapPairIdToContactPairIndex1_;
    robin_hood::unordered_map<int64_t, int> mapPairIdToContactPairIndex2_;
    std::shared_ptr<robin_hood::unordered_map<int64_t, int>> previousMapPairIdToContactPairIndex_;
    std::shared_ptr<robin_hood::unordered_map<int64_t, int>> currentMapPairIdToContactPairIndex_;

    std::vector<ContactManifold> contactManifolds1_;
    std::vector<ContactManifold> contactManifolds2_;
    std::shared_ptr<std::vector<ContactManifold>> previousContactManifolds_;
    std::shared_ptr<std::vector<ContactManifold>> currentContactManifolds_;

    std::vector<ContactPoint> contactPoints1_;
    std::vector<ContactPoint> contactPoints2_;
    std::shared_ptr<std::vector<ContactPoint>> previousContactPoints_;
    std::shared_ptr<std::vector<ContactPoint>> currentContactPoints_;

    robin_hood::unordered_map<std::shared_ptr<Actor>, std::vector<int>> mapBodyToContactPairs_;

    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    void ComputeBroadPhase();

    void ComputeMiddlePhase(NarrowPhaseInput& narrowPhaseInput, bool needToReportContacts);

    void ComputeMiddlePhaseCollisionSnapshot(std::vector<int64_t>& convexPairs,
                                             std::vector<int64_t>& concavePairs,
                                             NarrowPhaseInput& narrowPhaseInput,
                                             bool reportContacts);
    void ComputeNarrowPhase();

    bool ComputeNarrowPhaseOverlapSnapshot(NarrowPhaseInput& narrowPhaseInput,
                                           std::shared_ptr<OverlapCallback> callback);

    bool ComputeNarrowPhaseCollisionSnapshot(NarrowPhaseInput& narrowPhaseInput, CollisionCallback& callback);

    void ComputeOverlapSnapshotContactPairs(NarrowPhaseInput& narrowPhaseInput,
                                            std::vector<ContactPair>& contactPairs) const;

    void ComputeOverlapSnapshotContactPairs(NarrowPhaseInputBatch& narrowPhaseInputBatch,
                                            std::vector<ContactPair>& contactPairs,
                                            robin_hood::unordered_set<int64_t>& setOverlapContactPairId) const;

    void UpdateOverlappingPairs(const std::vector<std::pair<int, int>>& overlappingNodes);

    void RemoveNonOverlappingPairs();

    void AddLostContactPair(int overlappingPairIndex);

    bool TestNarrowPhaseCollision(NarrowPhaseInput& narrowPhaseInput, bool clipWithPreviousAxisIfStillColliding);

    void ComputeConvexVsConcaveMiddlePhase(int pairIndex, NarrowPhaseInput& narrowPhaseInput, bool reportContacts);

    void SwapPreviousAndCurrentContacts();

    void ProcessPotentialContacts(NarrowPhaseInputBatch& narrowPhaseInputBatch,
                                  bool updateLastFrameInfo,
                                  std::vector<ContactPointInfo>& potentialContactPoints,
                                  std::shared_ptr<robin_hood::unordered_map<int, int>> mapPairIdToContactPairIndex,
                                  std::vector<ContactManifoldInfo>& potentialContactManifolds,
                                  std::shared_ptr<std::vector<ContactPair>> contactPairs,
                                  robin_hood::unordered_map<std::shared_ptr<Actor>, std::vector<int>>&
                                  mapBodyToContactPairs);

    void ProcessPotentialContacts(NarrowPhaseInput& narrowPhaseInput,
                                  bool updateLastFrameInfo,
                                  std::vector<ContactPointInfo>& potentialContactPoints,
                                  std::shared_ptr<robin_hood::unordered_map<int, int>> mapPairIdToContactPairIndex,
                                  std::vector<ContactManifoldInfo>& potentialContactManifolds,
                                  std::shared_ptr<std::vector<ContactPair>> contactPairs,
                                  robin_hood::unordered_map<std::shared_ptr<Actor>, std::vector<int>>&
                                  mapBodyToContactPairs);

    void ReducePotentialContactManifolds(std::shared_ptr<std::vector<ContactPair>> contactPairs,
                                         std::vector<ContactManifoldInfo>& potentialContactManifolds,
                                         const std::vector<ContactPointInfo>& potentialContactPoints) const;

    void ComputeLostContactPairs();

    void CreateSnapshotContacts(std::vector<ContactPair>& contactPairs, std::vector<ContactManifold>& contactManifolds,
                                std::vector<ContactPoint>& contactPoints,
                                std::vector<ContactManifoldInfo>& potentialContactManifolds,
                                std::vector<ContactPointInfo>& potentialContactPoints);

    void InitContactsWithPreviousOnes();

    void ReduceContactPoints(ContactManifoldInfo& manifold, const Transform& shape1ToWorldTransform,
                             const std::vector<ContactPointInfo>& potentialContactPoints) const;

    void ReportContacts(CollisionCallback& callback, std::shared_ptr<std::vector<ContactPair>> contactPairs,
                        std::shared_ptr<std::vector<ContactManifold>> manifolds,
                        std::shared_ptr<std::vector<ContactPoint>> contactPoints,
                        std::vector<ContactPair>& lostContactPairs);

    void ReportTriggers(EventListener& eventListener, std::shared_ptr<std::vector<ContactPair>> contactPairs,
                        std::vector<ContactPair>& lostContactPairs);

    float ComputePotentialManifoldLargestContactDepth(const ContactManifoldInfo& manifold,
                                                      const std::vector<ContactPointInfo>& potentialContactPoints)
    const;

    void ProcessSmoothMeshContacts(std::shared_ptr<OverlappingPair> pair);

    void FilterOverlappingPairs(Actor& actor, std::vector<int>& convexPairs, std::vector<int>& concavePairs) const;

    void FilterOverlappingPairs(Actor& body1Actor, Actor& body2Actor, std::vector<int>& convexPairs,
                                std::vector<int>& concavePairs) const;

    void CreateContacts();
};

inline CollisionDispatch& BlocksEngine::CollisionDetectionSystem::GetCollisionDispatch()
{
    return collisionDispatch_;
}

inline void BlocksEngine::CollisionDetectionSystem::AddCollider(std::shared_ptr<Collider> collider, const AABB& aabb)
{
    broadPhaseSystem_.AddCollider(std::move(collider), aabb);

    int broadPhaseId = colliders_
}

