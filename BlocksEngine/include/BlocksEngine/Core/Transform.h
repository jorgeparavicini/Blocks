// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: GetTransform.h

#pragma once

#include <boost/signals2/signal.hpp>

#include "BlocksEngine/Core/Math/Matrix.h"
#include "BlocksEngine/Core/Math/Quaternion.h"
#include "BlocksEngine/Core/Math/Vector3.h"

namespace BlocksEngine
{
    class Camera;
    class Actor;
    class Transform;
}

class BlocksEngine::Transform
{
public:
    using MoveSignal = boost::signals2::signal<void(const Vector3<float>&)>;

    explicit Transform(physx::PxRigidActor* actor);

    [[nodiscard]] Matrix GetMatrix() const noexcept;
    [[nodiscard]] Vector3<float> GetPosition() const noexcept;
    [[nodiscard]] Vector3<float> GetLocalPosition() const noexcept;
    [[nodiscard]] Quaternion GetOrientation() const noexcept;
    [[nodiscard]] Quaternion GetLocalOrientation() const noexcept;
    [[nodiscard]] Vector3<float> GetScale() const noexcept;
    [[nodiscard]] Vector3<float> GetLocalScale() const noexcept;

    void SetPosition(const Vector3<float>& position) noexcept;
    void SetLocalPosition(const Vector3<float>& position) noexcept;
    void SetOrientation(const Quaternion& rotation) noexcept;
    void SetLocalOrientation(const Quaternion& rotation) noexcept;
    void SetScale(const Vector3<float>& scale) noexcept;
    void SetLocalScale(const Vector3<float>& scale) noexcept;

    void SetParent(std::weak_ptr<Transform> parent);
    void AddChild(std::weak_ptr<Transform> child);
    void RemoveChild(Transform& transform);

    [[nodiscard]] bool operator==(const Transform& transform) const noexcept;

    boost::signals2::connection AddSignalOnMove(const MoveSignal::slot_type& slot) noexcept;

    friend Actor;

private:
    physx::PxRigidActor* actor_;
    Vector3<float> scale_;

    std::weak_ptr<Transform> parent_;
    std::vector<std::weak_ptr<Transform>> children_{};

    void SetActor(physx::PxRigidActor* actor);

    // Signals
    MoveSignal moved_{};
};
