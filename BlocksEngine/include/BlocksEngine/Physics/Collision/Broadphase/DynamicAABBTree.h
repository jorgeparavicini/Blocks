// 
// Copyright (c) 2021, Severin Goddon & Jorge Paravicini
// All rights reserved.
// 
// This source code is licensed under the MIT-style license found in LICENSE file in the root directory of this source tree.
// 
// Author: Jorge Paravicini
// File: DynamicAABBTree.h

#pragma once
#include "BlocksEngine/Physics/Collision/Shapes/AABB.h"

namespace BlocksEngine
{
    struct TreeNode;
    class DynamicAABBTreeOverlapCallback;
    class DynamicAABBTreeRaycastCallback;
    class DynamicAABBTree;
}

/**
 * \brief This structure represents a node of the dynamic AABB Tree.
 */
struct BlocksEngine::TreeNode
{
    //------------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------------
    const static int32_t NULL_TREE_NODE;

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    // A node is either in the tree (has a parent) or in the free nodes list
    // (has a next node)

    /**
     * \brief A node is either in the tree (has a parent) or in the free nodes list
     * (has a next node)
     */
    union
    {
        /**
         * \brief Parent node ID
         */
        int32_t parentId;

        /**
         * \brief Next allocated node ID
         */
        int32_t nextNodeId;
    };

    /**
     * \brief A node is either a leaf (has data) or is an internal node (has children)
     */
    union
    {
        /**
         * \brief Left and right child of the node (children[0] = left child)
         */
        int32_t children[2];

        /**
         * \brief Two pieces of data stored at the node (in case the node is a leaf)
         */
        union
        {
            int32_t dataInt[2];
            void* dataPointer;
        };
    };

    /**
     * \brief Height of the node in the tree
     */
    int16_t height;

    /**
     * \brief Fat axis aligned bounding box (AABB) corresponding to the node
     */
    AABB aabb;

    /**
     * \brief Checks whether the node is a leaf ot the tree
     * \return True if the node is a leaf of the tree
     */
    [[nodiscard]] bool IsLeaf() const;
};


class BlocksEngine::DynamicAABBTreeOverlapCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    DynamicAABBTreeOverlapCallback() = default;

    DynamicAABBTreeOverlapCallback(const DynamicAABBTreeOverlapCallback&) = default;
    DynamicAABBTreeOverlapCallback& operator=(const DynamicAABBTreeOverlapCallback&) = default;

    DynamicAABBTreeOverlapCallback(DynamicAABBTreeOverlapCallback&&) = default;
    DynamicAABBTreeOverlapCallback& operator=(DynamicAABBTreeOverlapCallback&&) = default;

    virtual ~DynamicAABBTreeOverlapCallback() = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------
    virtual void NotifyOverlappingNode(int nodeId) = 0;
};


class BlocksEngine::DynamicAABBTreeRaycastCallback
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    DynamicAABBTreeRaycastCallback() = default;

    DynamicAABBTreeRaycastCallback(const DynamicAABBTreeRaycastCallback&) = default;
    DynamicAABBTreeRaycastCallback& operator=(const DynamicAABBTreeRaycastCallback&) = default;

    DynamicAABBTreeRaycastCallback(DynamicAABBTreeRaycastCallback&&) = default;
    DynamicAABBTreeRaycastCallback& operator=(DynamicAABBTreeRaycastCallback&&) = default;

    virtual ~DynamicAABBTreeRaycastCallback() = default;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    virtual float RaycastBroadPhaseShape(int32_t nodeId, const Ray& ray) = 0;
};

class BlocksEngine::DynamicAABBTree
{
public:
    //------------------------------------------------------------------------------
    // Constructors, Destructors, Assignment & Move
    //------------------------------------------------------------------------------

    DynamicAABBTree(float fatAABBInflatePercentage = 0.0f);

    DynamicAABBTree(const DynamicAABBTree&) = delete;
    DynamicAABBTree& operator=(const DynamicAABBTree&) = delete;

    DynamicAABBTree(DynamicAABBTree&&) = delete;
    DynamicAABBTree& operator=(DynamicAABBTree&&) = delete;

    ~DynamicAABBTree();


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------

    int32_t AddObject(const AABB& aabb, int32_t data1, int32_t data2);

    int32_t AddObject(const AABB& aabb, void* data);

    void RemoveObject(int32_t nodeId);

    bool UpdateObject(int32_t nodeId, const AABB& newAABB, bool forceReinsert = false);

    [[nodiscard]] const AABB& GetFatAABB(int32_t nodeId) const;

    [[nodiscard]] const int32_t* GetNodeDataInt(int32_t nodeId) const;

    [[nodiscard]] void* GetNodeDataPointer(int32_t nodeId) const;

    void ReportAllShapesOverlappingWithShapes(const std::vector<int32_t>& nodesToTest,
                                              size_t startIndex,
                                              size_t endIndex,
                                              std::vector<std::pair<int32_t, int32_t>>& outOverlappingNodes) const;

    void ReportAllShapesOverlappingWithAABB(const AABB& aabb, std::vector<int>& overlappingNodes) const;

    void Raycast(const Ray& ray, DynamicAABBTreeRaycastCallback& callback) const;

    int ComputeHeight();

    [[nodiscard]] AABB GetRootAABB() const;

    void Reset();

private:
    //------------------------------------------------------------------------------
    // Constants
    //------------------------------------------------------------------------------

    const static int INITIAL_NODE_COUNT;

    //------------------------------------------------------------------------------
    // Fields
    //------------------------------------------------------------------------------

    std::vector<TreeNode> nodes_;
    int rootNodeId_;
    int freeNodeId_;
    std::vector<TreeNode>::size_type allocatedNodesCount_;
    std::vector<TreeNode>::size_type nodesCount_;
    float fatAABBInflatePercentage_;


    //------------------------------------------------------------------------------
    // Methods
    //------------------------------------------------------------------------------
    int32_t AllocateNode();

    void ReleaseNode(int32_t nodeId);
    void InsertLeafNode(int32_t nodeId);
    void RemoveLeafNode(int32_t nodeId);
    int32_t BalanceSubTreeAtNode(int32_t nodeId);
    int ComputeHeight(int32_t nodeId);
    int32_t AddObjectInternal(const AABB& aabb);
    void Init();
};

inline bool BlocksEngine::TreeNode::IsLeaf() const
{
    return height == 0;
}

inline const BlocksEngine::AABB& BlocksEngine::DynamicAABBTree::GetFatAABB(const int32_t nodeId) const
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    return nodes_[nodeId].aabb;
}

inline const int32_t* BlocksEngine::DynamicAABBTree::GetNodeDataInt(const int32_t nodeId) const
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    assert(nodes_[nodeId].IsLeaf());
    return nodes_[nodeId].dataInt;
}

inline void* BlocksEngine::DynamicAABBTree::GetNodeDataPointer(const int32_t nodeId) const
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    assert(nodes_[nodeId].IsLeaf());
    return nodes_[nodeId].dataPointer;
}

inline BlocksEngine::AABB BlocksEngine::DynamicAABBTree::GetRootAABB() const
{
    return GetFatAABB(rootNodeId_);
}

inline int32_t BlocksEngine::DynamicAABBTree::AddObject(const AABB& aabb, const int32_t data1, const int32_t data2)
{
    const int32_t nodeId = AddObjectInternal(aabb);

    nodes_[nodeId].dataInt[0] = data1;
    nodes_[nodeId].dataInt[1] = data2;

    return nodeId;
}

inline int32_t BlocksEngine::DynamicAABBTree::AddObject(const AABB& aabb, void* data)
{
    const int32_t nodeId = AddObjectInternal(aabb);

    nodes_[nodeId].dataPointer = data;

    return nodeId;
}
