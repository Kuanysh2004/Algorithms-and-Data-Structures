I've implemented an Implicit Treap data structure that can be initialized in O(NlogN) time. It efficiently provides answers for 
Range Minimum Queries (RMQ) and supports reversing subarrays in the range [l, r] in O(logN) time. To optimize memory usage, I opted 
for std::unique_ptr over std::shared_ptr.
