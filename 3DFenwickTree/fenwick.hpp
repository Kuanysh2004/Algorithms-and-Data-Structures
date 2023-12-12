#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Constants {
const int32_t kMaxCords = 128;
}  // namespace Constants

class FenwickTree {
 public:
  FenwickTree() {
    border_length_ = Constants::kMaxCords;
    tree_.resize(border_length_,
                 std::vector<std::vector<int32_t>>(
                     border_length_, std::vector<int32_t>(border_length_, 0)));
  }

  void Update(int32_t x_cord, int32_t y_cord, int32_t z_cord, int32_t delta) {
    for (int32_t i = x_cord; i < border_length_; i = (i | (i + 1))) {
      for (int32_t j = y_cord; j < border_length_; j = (j | (j + 1))) {
        for (int32_t k = z_cord; k < border_length_; k = (k | (k + 1))) {
          tree_[i][j][k] += delta;
        }
      }
    }
  }

  int32_t GetSum(std::pair<int32_t, int32_t>& x_cords,
                 std::pair<int32_t, int32_t>& y_cords,
                 std::pair<int32_t, int32_t>& z_cords) {
    return GetPref(x_cords.second, y_cords.second, z_cords.second) -
           GetPref(x_cords.second, y_cords.second, z_cords.first - 1) -
           GetPref(x_cords.second, y_cords.first - 1, z_cords.second) -
           GetPref(x_cords.first - 1, y_cords.second, z_cords.second) +
           GetPref(x_cords.second, y_cords.first - 1, z_cords.first - 1) +
           GetPref(x_cords.first - 1, y_cords.second, z_cords.first - 1) +
           GetPref(x_cords.first - 1, y_cords.first - 1, z_cords.second) -
           GetPref(x_cords.first - 1, y_cords.first - 1, z_cords.first - 1);
  }

  int32_t GetPref(int32_t x_cord, int32_t y_cord, int32_t z_cord) {
    int32_t res = 0;
    for (int32_t i = x_cord; i >= 0; i = (i & (i + 1)) - 1) {
      for (int32_t j = y_cord; j >= 0; j = (j & (j + 1)) - 1) {
        for (int32_t k = z_cord; k >= 0; k = (k & (k + 1)) - 1) {
          res += tree_[i][j][k];
        }
      }
    }
    return res;
  }

 private:
  std::vector<std::vector<std::vector<int32_t>>> tree_;
  int32_t border_length_;
};
