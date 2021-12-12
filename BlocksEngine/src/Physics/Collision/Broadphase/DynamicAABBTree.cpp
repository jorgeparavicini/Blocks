#include "BlocksEngine/pch.h"
#include "BlocksEngine/Physics/Collision/Broadphase/DynamicAABBTree.h"

#include <stack>

using namespace BlocksEngine;

constexpr int32_t TreeNode::NULL_TREE_NODE = -1;

constexpr int DynamicAABBTree::INITIAL_NODE_COUNT = 8;

DynamicAABBTree::DynamicAABBTree(const float fatAABBInflatePercentage)
    : fatAABBInflatePercentage_{fatAABBInflatePercentage}
{
    Init();
}

DynamicAABBTree::~DynamicAABBTree() = default;

void DynamicAABBTree::Init()
{
    rootNodeId_ = TreeNode::NULL_TREE_NODE;
    nodesCount_ = 0;
    allocatedNodesCount_ = INITIAL_NODE_COUNT;

    nodes_ = std::vector<TreeNode>(INITIAL_NODE_COUNT);

    for (int i = 0; i < allocatedNodesCount_ - 1; ++i)
    {
        nodes_[i].nextNodeId = i + 1;
        nodes_[i].height = -1;
    }

    nodes_[allocatedNodesCount_ - 1].nextNodeId = TreeNode::NULL_TREE_NODE;
    nodes_[allocatedNodesCount_ - 1].height = -1;
    freeNodeId_ = 0;
}

void DynamicAABBTree::Reset()
{
    // TODO: Verify that the existing vector releases its data correctly
    Init();
}

int32_t DynamicAABBTree::AllocateNode()
{
    if (freeNodeId_ == TreeNode::NULL_TREE_NODE)
    {
        assert(nodesCount_ == allocatedNodesCount_);

        allocatedNodesCount_ *= 2;
        nodes_.resize(allocatedNodesCount_);

        for (int i = nodesCount_; i < allocatedNodesCount_ - 1; ++i)
        {
            nodes_[i].nextNodeId = i + 1;
            nodes_[i].height = -1;
        }
        nodes_[allocatedNodesCount_ - 1].nextNodeId = TreeNode::NULL_TREE_NODE;
        nodes_[allocatedNodesCount_ - 1].height = -1;
        freeNodeId_ = nodesCount_;
    }

    const int freeNodeId = freeNodeId_;
    freeNodeId_ = nodes_[freeNodeId].nextNodeId;
    nodes_[freeNodeId_].parentId = TreeNode::NULL_TREE_NODE;
    nodes_[freeNodeId_].height = 0;
    ++nodesCount_;

    return freeNodeId;
}

void DynamicAABBTree::ReleaseNode(const int32_t nodeId)
{
    assert(nodesCount_ > 0);
    assert(nodeId >= 00 && nodeId < allocatedNodesCount_);
    assert(nodes_[nodeId].height >= 0);
    nodes_[nodeId].nextNodeId = freeNodeId_;
    nodes_[nodeId].height = -1;
    freeNodeId_ = nodeId;
    --nodesCount_;
}

int32_t DynamicAABBTree::AddObjectInternal(const AABB& aabb)
{
    const int nodeId = AllocateNode();

    const Vector3 gap{aabb.GetExtent() * fatAABBInflatePercentage_ * 0.5f};
    nodes_[nodeId].aabb.SetMin(aabb.GetMin() - gap);
    nodes_[nodeId].aabb.SetMax(aabb.GetMax() + gap);

    nodes_[nodeId].height = 0;

    InsertLeafNode(nodeId);
    assert(nodes_[nodeId].IsLeaf());
    assert(nodeId >= 0);

    return nodeId;
}

void DynamicAABBTree::RemoveObject(const int32_t nodeId)
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    assert(nodes_[nodeId].IsLeaf());

    RemoveLeafNode(nodeId);
    ReleaseNode(nodeId);
}

bool DynamicAABBTree::UpdateObject(const int32_t nodeId, const AABB& newAABB, const bool forceReinsert)
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    assert(nodes_[nodeId].IsLeaf());
    assert(nodes_[nodeId].height > 0);

    if (!forceReinsert && nodes_[nodeId].aabb.Contains(newAABB))
    {
        return false;
    }

    RemoveLeafNode(nodeId);

    nodes_[nodeId].aabb = newAABB;
    const Vector3 gap{newAABB.GetExtent() * fatAABBInflatePercentage_ * 0.5f};
    nodes_[nodeId].aabb.SetMin(nodes_[nodeId].aabb.GetMin() - gap);
    nodes_[nodeId].aabb.SetMax(nodes_[nodeId].aabb.GetMax() + gap);

    assert(nodes_[nodeId].aabb.Contains(newAABB));

    InsertLeafNode(nodeId);

    return true;
}


void DynamicAABBTree::InsertLeafNode(const int32_t nodeId)
{
    if (rootNodeId_ == TreeNode::NULL_TREE_NODE)
    {
        rootNodeId_ = nodeId;
        nodes_[rootNodeId_].parentId = TreeNode::NULL_TREE_NODE;
        return;
    }

    assert(rootNodeId_ != TreeNode::NULL_TREE_NODE);

    const AABB newNodeAABB = nodes_[nodeId].aabb;
    int currentNodeId = rootNodeId_;
    while (!nodes_[currentNodeId].IsLeaf())
    {
        int leftChild = nodes_[currentNodeId].children[0];
        int rightChild = nodes_[currentNodeId].children[1];

        const float volumeAABB = nodes_[currentNodeId].aabb.GetVolume();
        const AABB mergedAABB = AABB::Merge(nodes_[currentNodeId].aabb, newNodeAABB);
        const float mergedVolume = mergedAABB.GetVolume();

        const float costS = 2.0f * mergedVolume;
        const float costI = 2.0f * (mergedVolume - volumeAABB);

        float costLeft;
        AABB currentAndLeftAABB = AABB::Merge(newNodeAABB, nodes_[leftChild].aabb);
        if (nodes_[leftChild].IsLeaf())
        {
            costLeft = currentAndLeftAABB.GetVolume() + costI;
        }
        else
        {
            const float leftChildVolume = nodes_[leftChild].aabb.GetVolume();
            costLeft = costI + currentAndLeftAABB.GetVolume() - leftChildVolume;
        }

        float costRight;
        AABB currentAndRightAABB = AABB::Merge(newNodeAABB, nodes_[rightChild].aabb);
        if (nodes_[rightChild].IsLeaf())
        {
            costRight = currentAndRightAABB.GetVolume() + costI;
        }
        else
        {
            const float rightChildVolume = nodes_[rightChild].aabb.GetVolume();
            costRight = costI + currentAndRightAABB.GetVolume() + rightChildVolume;
        }

        if (costS < costLeft && costS < costRight) break;

        currentNodeId = costLeft < costRight ? leftChild : rightChild;
    }

    const int siblingNode = currentNodeId;

    const int oldParentNode = nodes_[siblingNode].parentId;
    const int newParentNode = AllocateNode();
    nodes_[newParentNode].parentId = oldParentNode;
    nodes_[newParentNode].aabb = AABB::Merge(nodes_[siblingNode].aabb, newNodeAABB);
    nodes_[newParentNode].height = nodes_[siblingNode].height + 1;
    assert(nodes_[newParentNode].height > 0);

    if (oldParentNode != TreeNode::NULL_TREE_NODE)
    {
        assert(nodes_[oldParentNode].IsLeaf());
        if (nodes_[oldParentNode].children[0] == siblingNode)
        {
            nodes_[oldParentNode].children[0] = newParentNode;
        }
        else
        {
            nodes_[oldParentNode].children[1] = newParentNode;
        }

        nodes_[newParentNode].children[0] = siblingNode;
        nodes_[newParentNode].children[1] = nodeId;
        nodes_[siblingNode].parentId = newParentNode;
        nodes_[nodeId].parentId = newParentNode;
    }
    else
    {
        nodes_[newParentNode].children[0] = siblingNode;
        nodes_[newParentNode].children[1] = nodeId;
        nodes_[siblingNode].parentId = newParentNode;
        nodes_[nodeId].parentId = newParentNode;
        rootNodeId_ = newParentNode;
    }

    currentNodeId = nodes_[nodeId].parentId;
    assert(!nodes_[currentNodeId].IsLeaf());

    while (currentNodeId != TreeNode::NULL_TREE_NODE)
    {
        currentNodeId = BalanceSubTreeAtNode(currentNodeId);
        assert(nodes_[nodeId].IsLeaf());

        assert(!nodes_[currentNodeId].IsLeaf());
        const int leftChild = nodes_[currentNodeId].children[0];
        const int rightChild = nodes_[currentNodeId].children[1];
        assert(leftChild != TreeNode::NULL_TREE_NODE);
        assert(rightChild != TreeNode::NULL_TREE_NODE);

        nodes_[currentNodeId].height = std::max(nodes_[leftChild].height, nodes_[rightChild].height) + 1;
        assert(nodes_[currentNodeId].height > 0);

        nodes_[currentNodeId].aabb = AABB::Merge(nodes_[leftChild].aabb, nodes_[rightChild].aabb);
        currentNodeId = nodes_[currentNodeId].parentId;
    }

    assert(nodes_[nodeId].IsLeaf());
}

void DynamicAABBTree::RemoveLeafNode(const int32_t nodeId)
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    assert(nodes_[nodeId].IsLeaf());

    // If we are removing the root node (root node is a leaf in this case)
    if (rootNodeId_ == nodeId)
    {
        rootNodeId_ = TreeNode::NULL_TREE_NODE;
        return;
    }

    const int parentNodeId = nodes_[nodeId].parentId;
    const int grandParentNodeId = nodes_[parentNodeId].parentId;
    int siblingNodeId;
    if (nodes_[parentNodeId].children[0] == nodeId)
    {
        siblingNodeId = nodes_[parentNodeId].children[1];
    }
    else
    {
        siblingNodeId = nodes_[parentNodeId].children[0];
    }

    // If the parent of the node to remove is not the root node
    if (grandParentNodeId != TreeNode::NULL_TREE_NODE)
    {
        // Destroy the parent node
        if (nodes_[grandParentNodeId].children[0] == parentNodeId)
        {
            nodes_[grandParentNodeId].children[0] = siblingNodeId;
        }
        else
        {
            assert(nodes_[grandParentNodeId].children[1] == parentNodeId);
            nodes_[grandParentNodeId].children[1] = siblingNodeId;
        }
        nodes_[siblingNodeId].parentId = grandParentNodeId;
        ReleaseNode(parentNodeId);

        // Now, we need to recompute the AABBs of the node on the path back to the root
        // and make sure that the tree is still balanced
        int currentNodeId = grandParentNodeId;
        while (currentNodeId != TreeNode::NULL_TREE_NODE)
        {
            // Balance the current sub-tree if necessary
            currentNodeId = BalanceSubTreeAtNode(currentNodeId);

            assert(!nodes_[currentNodeId].IsLeaf());

            // Get the two children of the current node
            const int leftChildId = nodes_[currentNodeId].children[0];
            const int rightChildId = nodes_[currentNodeId].children[1];

            // Recompute the AABB and the height of the current node
            nodes_[currentNodeId].aabb = AABB::Merge(nodes_[leftChildId].aabb,
                                                     nodes_[rightChildId].aabb);
            nodes_[currentNodeId].height = std::max(nodes_[leftChildId].height,
                                                    nodes_[rightChildId].height) + 1;
            assert(nodes_[currentNodeId].height > 0);

            currentNodeId = nodes_[currentNodeId].parentId;
        }
    }
    else
    {
        // If the parent of the node to remove is the root node

        // The sibling node becomes the new root node
        rootNodeId_ = siblingNodeId;
        nodes_[siblingNodeId].parentId = TreeNode::NULL_TREE_NODE;
        ReleaseNode(parentNodeId);
    }
}

int32_t DynamicAABBTree::BalanceSubTreeAtNode(const int32_t nodeId)
{
    assert(nodeId != TreeNode::NULL_TREE_NODE);

    TreeNode& nodeA = nodes_[nodeId];

    // If the node is a leaf or the height of A's sub-tree is less than 2
    if (nodeA.IsLeaf() || nodeA.height < 2)
    {
        // Do not perform any rotation
        return nodeId;
    }

    // Get the two children nodes
    int nodeBId = nodeA.children[0];
    int nodeCId = nodeA.children[1];
    assert(nodeBId >= 0 && nodeBId < allocatedNodesCount_);
    assert(nodeCId >= 0 && nodeCId < allocatedNodesCount_);
    TreeNode& nodeB = nodes_[nodeBId];
    TreeNode& nodeC = nodes_[nodeCId];

    // Compute the factor of the left and right sub-trees
    const int balanceFactor = nodeC.height - nodeB.height;

    // If the right node C is 2 higher than left node B
    if (balanceFactor > 1)
    {
        assert(!nodeC.IsLeaf());

        const int nodeFId = nodeC.children[0];
        const int nodeGId = nodeC.children[1];
        assert(nodeFId >= 0 && nodeFId < allocatedNodesCount_);
        assert(nodeGId >= 0 && nodeGId < allocatedNodesCount_);
        TreeNode& nodeF = nodes_[nodeFId];
        TreeNode& nodeG = nodes_[nodeGId];

        nodeC.children[0] = nodeId;
        nodeC.parentId = nodeA.parentId;
        nodeA.parentId = nodeCId;

        if (nodeC.parentId != TreeNode::NULL_TREE_NODE)
        {
            if (nodes_[nodeC.parentId].children[0] == nodeId)
            {
                nodes_[nodeC.parentId].children[0] = nodeCId;
            }
            else
            {
                assert(nodes_[nodeC.parentId].children[1] == nodeId);
                nodes_[nodeC.parentId].children[1] = nodeCId;
            }
        }
        else
        {
            rootNodeId_ = nodeCId;
        }

        assert(!nodeC.IsLeaf());
        assert(!nodeA.IsLeaf());

        // If the right node C was higher than left node B because of the F node
        if (nodeF.height > nodeG.height)
        {
            nodeC.children[1] = nodeFId;
            nodeA.children[1] = nodeGId;
            nodeG.parentId = nodeId;

            // Recompute the AABB of node A and C
            nodeA.aabb = AABB::Merge(nodeB.aabb, nodeG.aabb);
            nodeC.aabb = AABB::Merge(nodeA.aabb, nodeF.aabb);

            // Recompute the height of node A and C
            nodeA.height = std::max(nodeB.height, nodeG.height) + 1;
            nodeC.height = std::max(nodeA.height, nodeF.height) + 1;
            assert(nodeA.height > 0);
            assert(nodeC.height > 0);
        }
        else
        {
            // If the right node C was higher than left node B because of node G
            nodeC.children[1] = nodeGId;
            nodeA.children[1] = nodeFId;
            nodeF.parentId = nodeId;

            // Recompute the AABB of node A and C
            nodeA.aabb = AABB::Merge(nodeB.aabb, nodeF.aabb);
            nodeC.aabb = AABB::Merge(nodeA.aabb, nodeG.aabb);

            // Recompute the height of node A and C
            nodeA.height = std::max(nodeB.height, nodeF.height) + 1;
            nodeC.height = std::max(nodeA.height, nodeG.height) + 1;
            assert(nodeA.height > 0);
            assert(nodeC.height > 0);
        }

        // Return the new root of the sub-tree
        return nodeCId;
    }

    // If the left node B is 2 higher than right node C
    if (balanceFactor < -1)
    {
        assert(!nodeB.IsLeaf());

        int nodeFId = nodeB.children[0];
        int nodeGId = nodeB.children[1];
        assert(nodeFId >= 0 && nodeFId < allocatedNodesCount_);
        assert(nodeGId >= 0 && nodeGId < allocatedNodesCount_);
        TreeNode& nodeF = nodes_[nodeFId];
        TreeNode& nodeG = nodes_[nodeGId];

        nodeB.children[0] = nodeId;
        nodeB.parentId = nodeA.parentId;
        nodeA.parentId = nodeBId;

        if (nodeB.parentId != TreeNode::NULL_TREE_NODE)
        {
            if (nodes_[nodeB.parentId].children[0] == nodeId)
            {
                nodes_[nodeB.parentId].children[0] = nodeBId;
            }
            else
            {
                assert(nodes_[nodeB.parentId].children[1] == nodeId);
                nodes_[nodeB.parentId].children[1] = nodeBId;
            }
        }
        else
        {
            rootNodeId_ = nodeBId;
        }

        assert(!nodeB.IsLeaf());
        assert(!nodeA.IsLeaf());

        // If the left node B was higher than right node C because of the F node
        if (nodeF.height > nodeG.height)
        {
            nodeB.children[1] = nodeFId;
            nodeA.children[0] = nodeGId;
            nodeG.parentId = nodeId;

            // Recompute the AABB of node A and B
            nodeA.aabb = AABB::Merge(nodeC.aabb, nodeG.aabb);
            nodeB.aabb = AABB::Merge(nodeA.aabb, nodeF.aabb);

            // Recompute the height of node A and B
            nodeA.height = std::max(nodeC.height, nodeG.height) + 1;
            nodeB.height = std::max(nodeA.height, nodeF.height) + 1;
            assert(nodeA.height > 0);
            assert(nodeB.height > 0);
        }
        else
        {
            // If the left node B was higher than right node C because of node G
            nodeB.children[1] = nodeGId;
            nodeA.children[0] = nodeFId;
            nodeF.parentId = nodeId;

            // Recompute the AABB of node A and B
            nodeA.aabb = AABB::Merge(nodeC.aabb, nodeF.aabb);
            nodeB.aabb = AABB::Merge(nodeA.aabb, nodeG.aabb);

            // Recompute the height of node A and B
            nodeA.height = std::max(nodeC.height, nodeF.height) + 1;
            nodeB.height = std::max(nodeA.height, nodeG.height) + 1;
            assert(nodeA.height > 0);
            assert(nodeB.height > 0);
        }

        // Return the new root of the sub-tree
        return nodeBId;
    }

    // If the sub-tree is balanced, return the current root node
    return nodeId;
}

void DynamicAABBTree::ReportAllShapesOverlappingWithShapes(const std::vector<int32_t>& nodesToTest,
                                                           const size_t startIndex,
                                                           const size_t endIndex,
                                                           std::vector<std::pair<int32_t, int32_t>>& outOverlappingNodes
) const
{
    // Create a stack with the nodes to visit
    std::stack<int> stack;

    // For each shape to be tested for overlap
    for (size_t i = startIndex; i < endIndex; i++)
    {
        assert(nodesToTest[i] != -1);

        stack.push(rootNodeId_);

        const AABB& shapeAABB = GetFatAABB(nodesToTest[i]);

        // While there are still nodes to visit
        while (stack.size() > 0)
        {
            // Get the next node ID to visit
            const int nodeIdToVisit = stack.top();
            stack.pop();

            // Skip it if it is a null node
            if (nodeIdToVisit == TreeNode::NULL_TREE_NODE) continue;

            // Get the corresponding node
            const TreeNode& nodeToVisit = nodes_[nodeIdToVisit];

            // If the AABB in parameter overlaps with the AABB of the node to visit
            if (shapeAABB.TestCollision(nodeToVisit.aabb))
            {
                // If the node is a leaf
                if (nodeToVisit.IsLeaf())
                {
                    // Add the node in the list of overlapping nodes
                    outOverlappingNodes.push_back(std::pair(nodesToTest[i], nodeIdToVisit));
                }
                else
                {
                    // If the node is not a leaf

                    // We need to visit its children
                    stack.push(nodeToVisit.children[0]);
                    stack.push(nodeToVisit.children[1]);
                }
            }
        }
    }
}


void DynamicAABBTree::ReportAllShapesOverlappingWithAABB(const AABB& aabb, std::vector<int>& overlappingNodes) const
{
    // Create a stack with the nodes to visit
    std::stack<int> stack;
    stack.push(rootNodeId_);

    // While there are still nodes to visit
    while (stack.size() > 0)
    {
        // Get the next node ID to visit
        const int nodeIdToVisit = stack.top();
        stack.pop();

        // Skip it if it is a null node
        if (nodeIdToVisit == TreeNode::NULL_TREE_NODE) continue;

        // Get the corresponding node
        const TreeNode& nodeToVisit = nodes_[nodeIdToVisit];

        // If the AABB in parameter overlaps with the AABB of the node to visit
        if (aabb.TestCollision(nodeToVisit.aabb))
        {
            // If the node is a leaf
            if (nodeToVisit.IsLeaf())
            {
                // Notify the broad-phase about a new potential overlapping pair
                overlappingNodes.push_back(nodeIdToVisit);
            }
            else
            {
                // If the node is not a leaf

                // We need to visit its children
                stack.push(nodeToVisit.children[0]);
                stack.push(nodeToVisit.children[1]);
            }
        }
    }
}

void DynamicAABBTree::Raycast(const Ray& ray, DynamicAABBTreeRaycastCallback& callback) const
{
    float maxFraction = ray.maxFraction;

    std::stack<int> stack;
    stack.push(rootNodeId_);

    // Walk through the tree from the root looking for colliders
    // that overlap with the ray AABB
    while (stack.size() > 0)
    {
        // Get the next node in the stack
        const int nodeId = stack.top();
        stack.pop();

        // If it is a null node, skip it
        if (nodeId == TreeNode::NULL_TREE_NODE) continue;

        // Get the corresponding node
        const TreeNode& node = nodes_[nodeId];

        Ray rayTemp(ray.point1, ray.point2, maxFraction);

        // Test if the ray intersects with the current node AABB
        if (!node.aabb.TestRayIntersect(rayTemp)) continue;

        // If the node is a leaf of the tree
        if (node.IsLeaf())
        {
            // Call the callback that will raycast again the broad-phase shape
            const float hitFraction = callback.RaycastBroadPhaseShape(nodeId, rayTemp);

            // If the user returned a hitFraction of zero, it means that
            // the raycasting should stop here
            if (hitFraction == 0.0f)
            {
                return;
            }

            // If the user returned a positive fraction
            if (hitFraction > 0.0f)
            {
                // We update the maxFraction value and the ray
                // AABB using the new maximum fraction
                if (hitFraction < maxFraction)
                {
                    maxFraction = hitFraction;
                }
            }

            // If the user returned a negative fraction, we continue
            // the raycasting as if the collider did not exist
        }
        else
        {
            // If the node has children

            // Push its children in the stack of nodes to explore
            stack.push(node.children[0]);
            stack.push(node.children[1]);
        }
    }
}

int DynamicAABBTree::ComputeHeight()
{
    return ComputeHeight(rootNodeId_);
}

int DynamicAABBTree::ComputeHeight(const int32_t nodeId)
{
    assert(nodeId >= 0 && nodeId < allocatedNodesCount_);
    const TreeNode& node = nodes_[nodeId];

    if (node.IsLeaf())
    {
        return 0;
    }

    const int leftHeight = ComputeHeight(node.children[0]);
    const int rightHeight = ComputeHeight(node.children[1]);

    return +std::max(leftHeight, rightHeight);
}
