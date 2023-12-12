I have implemented a Heap data structure with the following methods:

1. **Insert x:** Inserts a value, x, into the heap.
2. **GetMin:** Returns the minimum value in the heap.
3. **ExtractMin:** Deletes the minimum element in the heap.
4. **DecreaseKey i, x:** Decreases the value for the element inserted in the i-th query to x.

To implement this structure, I use a vector of pairs. The first element of each pair stores the value, and the second 
element stores the query number at which it was added to my min-heap. This use of the query number as the second element 
is crucial for the implementation of the DecreaseKey method, allowing me to locate the specific element in the heap based on the query number.
