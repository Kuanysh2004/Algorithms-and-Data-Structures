#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

namespace Constants {
const int32_t kFour = 4;
}

class SparseTable {
 public:
  SparseTable(std::vector<int32_t>& vec) {
    arr_size_ = vec.size();
    deg_.resize(arr_size_ + 1, 0);
    for (int32_t i = 2; i < arr_size_ + 1; ++i) {
      if ((i & (i - 1)) == 0) {
        deg_[i] = deg_[i - 1] + 1;
      } else {
        deg_[i] = deg_[i - 1];
      }
    }
    table_.resize(deg_[arr_size_] + 1,
                  std::vector<std::pair<int32_t, int32_t>>(arr_size_));
    for (int32_t i = 0; i < arr_size_; ++i) {
      table_[0][i] = std::make_pair(i, i);
    }
    auto comparator = [&vec](int32_t first, int32_t second) {
      return vec[first] < vec[second];
    };
    std::vector<int32_t> compare(Constants::kFour);
    for (int32_t i = 1; i < deg_[arr_size_] + 1; ++i) {
      for (int32_t j = 0; j < arr_size_; ++j) {
        if (j + (1LL << i) > arr_size_) {
          break;
        }
        compare[0] = table_[i - 1][j].first;
        compare[1] = table_[i - 1][j].second;
        compare[2] = table_[i - 1][j + (1LL << (i - 1))].first;
        compare[3] = table_[i - 1][j + (1LL << (i - 1))].second;
        std::sort(compare.begin(), compare.end(), comparator);
        for (int32_t k = 1; k < Constants::kFour; ++k) {
          if (compare[k] != compare[k - 1]) {
            table_[i][j] = std::make_pair(compare[k - 1], compare[k]);
            break;
          }
        }
      }
    }
  }

  int32_t SecondStat(std::vector<int32_t>& vec, int32_t left, int32_t right) {
    int32_t height = deg_[right - left + 1];
    auto comparator = [&vec](int32_t first, int32_t second) {
      return vec[first] < vec[second];
    };
    std::vector<int32_t> compare(Constants::kFour);
    compare[0] = table_[height][left].first;
    compare[1] = table_[height][left].second;
    compare[2] = table_[height][right - (1LL << height) + 1].first;
    compare[3] = table_[height][right - (1LL << height) + 1].second;
    std::sort(compare.begin(), compare.end(), comparator);
    for (int32_t i = 1; i < Constants::kFour; ++i) {
      if (compare[i] != compare[i - 1]) {
        return vec[compare[i]];
      }
    }
    return 0;
  }

 private:
  std::vector<std::vector<std::pair<int32_t, int32_t>>> table_;
  std::vector<int32_t> deg_;
  int32_t arr_size_;
};

int main() {
  int32_t num;
  int32_t query_count;
  std::cin >> num >> query_count;
  std::vector<int32_t> arr(num);
  for (int32_t i = 0; i < num; ++i) {
    std::cin >> arr[i];
  }
  SparseTable table(arr);
  for (int32_t i = 0; i < query_count; ++i) {
    int32_t left_border;
    int32_t right_border;
    std::cin >> left_border >> right_border;
    std::cout << table.SecondStat(arr, left_border - 1, right_border - 1)
              << "\n";
  }
}
