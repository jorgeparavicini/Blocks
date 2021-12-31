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
    using RotateSignal = boost::signals2::signal<void(const Quaternion&)>;

    explicit Transform(const Vector3<float>& position = Vector3<float>::Zero,
                       const Quaternion& orientation = Quaternion::Identity,
                       const Vector3<float>& scale = Vector3<float>::One);

    [[nodiscard]] Matrix GetMatrix() const noexcept;
    [[nodiscard]] const Vector3<float>& GetPosition() const noexcept;
    [[nodiscard]] const Vector3<float>& GetLocalPosition() const noexcept;
    [[nodiscard]] const Quaternion& GetOrientation() const noexcept;
    [[nodiscard]] const Quaternion& GetLocalOrientation() const noexcept;
    [[nodiscard]] const Vector3<float>& GetScale() const noexcept;
    [[nodiscard]] const Vector3<float>& GetLocalScale() const noexcept;

    void SetPosition(const Vector3<float>& position) noexcept;
    void SetLocalPosition(const Vector3<float>& position) noexcept;
    void SetOrientation(const Quaternion& orientation) noexcept;
    void SetLocalOrientation(const Quaternion& orientation) noexcept;
    void SetScale(const Vector3<float>& scale) noexcept;
    void SetLocalScale(const Vector3<float>& scale) noexcept;

    void SetParent(std::weak_ptr<Transform> parent);
    void AddChild(std::weak_ptr<Transform> child);
    void RemoveChild(Transform& transform);

    [[nodiscard]] bool operator==(const Transform& transform) const noexcept;

    boost::signals2::connection AddSignalOnMove(const MoveSignal::slot_type& slot) noexcept;
    boost::signals2::connection AddSignalOnRotate(const RotateSignal::slot_type& slot) noexcept;

    friend Actor;

private:
    Vector3<float> position_;
    Quaternion orientation_;
    Vector3<float> scale_;

    std::weak_ptr<Transform> parent_;
    std::vector<std::weak_ptr<Transform>> children_{};

    // Signals
    MoveSignal moved_{};
    RotateSignal rotated_{};
};
