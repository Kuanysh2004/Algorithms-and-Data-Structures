#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <unordered_map>
#include <vector>

class Treap {
 private:
  struct Node {
    int64_t min;
    int64_t priority;
    int64_t size_tree;
    int64_t value;
    std::unique_ptr<Node> left_child = nullptr;
    std::unique_ptr<Node> right_child = nullptr;
    Node(int64_t min_val, int64_t prio, int64_t size, int64_t val)
        : min(min_val), priority(prio), size_tree(size), value(val) {}
    virtual void SetUpdate(){};
    virtual void PushUpdate(){};
    virtual ~Node(){};
  };

  struct ReverseNode : Node {
    bool reversed = false;
    ReverseNode(int64_t min_val, int64_t prio, int64_t size, int64_t val,
                bool reversed)
        : Node(min_val, prio, size, val), reversed(reversed){};
    void SetUpdate() override { reversed ^= 1; }
    void PushUpdate() override {
      if (!reversed) {
        return;
      }
      std::swap(Node::left_child, Node::right_child);
      reversed = false;
      if (Node::left_child != nullptr) {
        Node::left_child->SetUpdate();
      }
      if (Node::right_child != nullptr) {
        Node::right_child->SetUpdate();
      }
    }
  };

 public:
  static void SetUpdate(std::unique_ptr<Node>& node) {
    if (node != nullptr) {
      node->SetUpdate();
    }
  }

  static void PushUpdate(std::unique_ptr<Node>& node) {
    if (node != nullptr) {
      node->PushUpdate();
    }
  }

  static int64_t GetNodeSize(std::unique_ptr<Node>& node) {
    return node == nullptr ? 0 : node->size_tree;
  }

  static void SetNodeSize(std::unique_ptr<Node>& node) {
    node->size_tree =
        GetNodeSize(node->left_child) + GetNodeSize(node->right_child) + 1;
  }

  static int64_t GetNodeMin(std::unique_ptr<Node>& node) {
    return node == nullptr ? std::numeric_limits<int64_t>::max() : node->min;
  }

  static void SetMinNode(std::unique_ptr<Node>& node) {
    node->min =
        std::min(GetNodeMin(node->left_child), GetNodeMin(node->right_child));
    node->min = std::min(node->value, node->min);
  }

  std::pair<std::unique_ptr<Node>, std::unique_ptr<Node>> Split(
      std::unique_ptr<Node>& node, const int64_t& pos) {
    if (node == nullptr) {
      return std::make_pair(nullptr, nullptr);
    }
    PushUpdate(node);
    if (GetNodeSize(node->left_child) == pos) {
      auto left_ans = std::move(node->left_child);
      node->left_child = nullptr;
      PushUpdate(node);
      SetNodeSize(node);
      SetMinNode(node);
      return std::make_pair(std::move(left_ans), std::move(node));
    }
    if (GetNodeSize(node->left_child) > pos) {
      auto [left, right] = Split(node->left_child, pos);
      node->left_child = std::move(right);
      PushUpdate(node);
      SetNodeSize(node);
      SetMinNode(node);
      return std::make_pair(std::move(left), std::move(node));
    }
    auto [left, right] =
        Split(node->right_child, pos - GetNodeSize(node->left_child) - 1);
    node->right_child = std::move(left);
    PushUpdate(node);
    SetNodeSize(node);
    SetMinNode(node);
    return std::make_pair(std::move(node), std::move(right));
  }

  std::unique_ptr<Node> Merge(std::unique_ptr<Node> first,
                              std::unique_ptr<Node> second) {
    if (first == nullptr) {
      PushUpdate(second);
      return second;
    }
    if (second == nullptr) {
      PushUpdate(first);
      return first;
    }
    PushUpdate(first);
    PushUpdate(second);
    if (first->priority < second->priority) {
      second->left_child =
          (Merge(std::move(first), std::move(second->left_child)));
      SetNodeSize(second);
      SetMinNode(second);
      return second;
    }
    first->right_child =
        (Merge(std::move(first->right_child), std::move(second)));
    SetNodeSize(first);
    SetMinNode(first);
    return first;
  }

  void Insert(int64_t pos, int64_t value) {
    auto [left, right] = Split(root_, pos);
    std::unique_ptr<Node> one_node_tree =
        std::make_unique<ReverseNode>(value, rand_(), 1, value, false);
    root_ = (Merge(Merge(std::move(left), std::move(one_node_tree)),
                   std::move(right)));
  }

  int64_t GetMin(int64_t left, int64_t right) {
    auto [first, second] = Split(root_, right);
    auto [left_first, right_first] = Split(first, left - 1);
    int64_t res = GetNodeMin(right_first);
    root_ = (Merge(Merge(std::move(left_first), std::move(right_first)),
                   std::move(second)));
    return res;
  }

  void Reverse(int64_t left, int64_t right) {
    auto [first, second] = Split(root_, right);
    auto [left_first, right_first] = Split(first, left - 1);
    SetUpdate(right_first);
    root_ = Merge(std::move(left_first),
                  Merge(std::move(right_first), std::move(second)));
  }

 private:
  std::unique_ptr<Node> root_ = nullptr;
  std::mt19937_64 rand_;
};
