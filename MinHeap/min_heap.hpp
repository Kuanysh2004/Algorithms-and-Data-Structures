#include <algorithm>
#include <iostream>
#include <vector>

class MinHeap {
 public:
  void Insert(int64_t elem, int64_t request_ind) {
    heap_.push_back(std::make_pair(elem, request_ind));
    SiftUp(heap_.size() - 1);
  }

  int64_t GetMin() const { return heap_[0].first; }

  static int64_t GetLeftChild(int64_t ind) { return 2 * ind + 1; }

  static int64_t GetRightChild(int64_t ind) { return 2 * ind + 2; }

  void ExtractMin() {
    heap_[0] = heap_[heap_.size() - 1];
    heap_.pop_back();
    SiftDown(0);
  }

  void DecreaseKey(int64_t request, int64_t delta) {
    size_t ind = 0;
    for (; ind < heap_.size(); ++ind) {
      if (heap_[ind].second == request) {
        heap_[ind].first -= delta;
        break;
      }
    }
    SiftUp(ind);
  }

  void SiftUp(int64_t ind) {
    int64_t parent = (ind - 1) / 2;
    while (ind > 0 && heap_[ind] < heap_[parent]) {
      std::swap(heap_[ind], heap_[parent]);
      ind = parent;
      parent = (ind - 1) / 2;
    }
  }

  void SiftDown(int64_t ind) {
    const int64_t kLeftChild = GetLeftChild(ind);
    const int64_t kRightChild = GetRightChild(ind);
    const int64_t kSize = heap_.size();
    int64_t min = 0;
    if (kLeftChild <= kSize) {
      if (kLeftChild == kSize) {
        min = kLeftChild;
      } else {
        if (heap_[kLeftChild] < heap_[kRightChild]) {
          min = kLeftChild;
        } else {
          min = kRightChild;
        }
      }
      if (heap_[min] < heap_[ind]) {
        std::swap(heap_[ind], heap_[min]);
        SiftDown(min);
      }
    }
  }

 private:
  std::vector<std::pair<int64_t, int64_t>> heap_;
};
