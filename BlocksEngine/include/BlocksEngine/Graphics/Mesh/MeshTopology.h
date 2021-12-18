// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: MeshTopology.h

#pragma once

namespace BlocksEngine
{
    enum class MeshTopology;
    class MeshTopologyBuffer;
};

enum class BlocksEngine::MeshTopology
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
};
