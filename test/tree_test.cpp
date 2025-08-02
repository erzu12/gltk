#include <gtest/gtest.h>
#include "../src/layout/tree.h"

namespace gltk {

TEST(TreeTest, Insert) {
    Tree<int> tree;
    auto root = tree.insert(1);
    ASSERT_EQ(root->data, 1);
}

TEST(TreeTest, InsertChildren) {
    Tree<int> tree;
    auto root = tree.insert(1);
    tree.insert(root, 2);
    tree.insert(root, 3);
    auto children = root->children;
    ASSERT_EQ(children.size(), 2);
    ASSERT_EQ(children[0]->data, 2);
    ASSERT_EQ(children[1]->data, 3);
}

TEST(TreeTest, RootSet) {
    Tree<int> tree;
    auto root = tree.insert(2);
    ASSERT_EQ(tree.root.value(), root);
}

TEST(TreeTest, RangeBasedForLoopChildren) {
    Tree<int> tree;
    auto root = tree.insert(1);
    tree.insert(root, 2);
    tree.insert(root, 3);
    int i = 2;
    for (auto node : root->children) {
        ASSERT_EQ(node->data, i);
        i++;
    }
}

TEST(TreeTest, DepthFirstTraversal) {
    Tree<int> tree;
    auto root = tree.insert(1);
    auto child1 = tree.insert(root, 2);
    auto child2 = tree.insert(root, 3);
    auto child3 = tree.insert(child1, 4);
    auto child4 = tree.insert(child1, 5);
    auto child5 = tree.insert(child2, 6);
    auto child6 = tree.insert(child2, 7);
    std::vector<int> expected = {1, 2, 4, 5, 3, 6, 7};
    std::vector<int> actual;
    tree.runDepthFirst([&actual](Node<int> *node) {
        actual.push_back(node->data);
    });

}
} // namespace gltk
