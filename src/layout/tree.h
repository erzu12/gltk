#pragma once


#include <functional>
#include <memory>
#include <optional>
#include <vector>


template <typename T>
class Tree;


template <typename T>
class Node {
    void addChild(Node<T> *child) {
        children.push_back(child);
    }
    friend class Tree<T>;
public:
    T data;

    std::vector<Node<T>*> children;
    std::optional<Node<T>*> parent;

    Node(T data) : data(data) {}

};

template <typename T>
class Tree {
    std::vector<std::unique_ptr<Node<T>>> nodes;
public:
    std::optional<Node<T>*> root;

    Tree() {}

    void runDepthFirst(std::function<void(Node<T>*)> func) {
        std::vector<Node<T>*> stack;
        while (!stack.empty()) {
            Node<T> *node = stack.back();
            stack.pop_back();
            for (Node<T> *child : node->children) {
                stack.push_back(child);
            }
            func(node);
        }
    }
    
    Node<T> *insert(const T& data) {
        auto node = std::make_unique<Node<T>>(data);
        if (root.has_value()) {
            Node<T> *old_root = root.value();
            root = node.get();
            root.value()->addChild(old_root);
            old_root->parent = root.value();
        }
        else {
            root = node.get();
        }
        nodes.push_back(std::move(node));
        return nodes.back().get();
    }

    Node<T> *insert(Node<T> *parent, const T& data) {
        if (!parent) {
            return insert(data);
        }
        auto node = std::make_unique<Node<T>>(data);
        parent->addChild(node.get());
        node->parent = parent;
        nodes.push_back(std::move(node));
        return nodes.back().get();
    }
};
