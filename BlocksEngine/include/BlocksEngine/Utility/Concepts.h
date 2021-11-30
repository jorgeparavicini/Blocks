// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: Concepts.h

#pragma once

#include <type_traits>

template <class T, class U>
concept Derived = std::is_base_of_v<U, T>;
