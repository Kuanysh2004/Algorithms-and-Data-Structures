#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Constants {
const int32_t kMaxRightBorder = 1 << 30;
}

class DynamicSegmentTree {
 private:
  friend int main();
  struct Node {
    int32_t sum;
    int32_t left_border;
    int32_t right_border;
    std::unique_ptr<Node> right_child = nullptr;
    std::unique_ptr<Node> left_child = nullptr;
  };

 public:
  void Update(std::unique_ptr<Node>& node, int32_t value, int32_t pos) {
    node->sum += value;
    if (node->left_border == node->right_border) {
      return;
    }
    int32_t mid =
        node->left_border + (node->right_border - node->left_border) / 2;
    if (mid >= pos && node->left_child == nullptr) {
      node->left_child = std::make_unique<Node>(0, node->left_border, mid);
    }
    if (mid >= pos) {
      Update(node->left_child, value, pos);
    }
    if (mid < pos && node->right_child == nullptr) {
      node->right_child =
          std::make_unique<Node>(0, mid + 1, node->right_border);
    }
    if (mid < pos) {
      Update(node->right_child, value, pos);
    }
  }

  int32_t GetSum(std::unique_ptr<Node>& node, int32_t left_query,
                 int32_t right_query) {
    if (node == nullptr) {
      return 0;
    }
    if (left_query == node->left_border && right_query == node->right_border) {
      return node->sum;
    }
    int32_t mid =
        node->left_border + (node->right_border - node->left_border) / 2;
    int32_t res = 0;
    if (mid >= left_query) {
      res += GetSum(node->left_child, left_query, std::min(mid, right_query));
    }
    if (mid < right_query) {
      res +=
          GetSum(node->right_child, std::max(left_query, mid + 1), right_query);
    }
    return res;
  }

  std::unique_ptr<Node>& GetRoot() { return root_; }

 private:
  std::unique_ptr<Node> root_ =
      std::make_unique<Node>(0, 0, Constants::kMaxRightBorder);
};
