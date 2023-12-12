This task was part of my Algorithms and Data Structures course, where I needed to answer N queries to find the second smallest number in the range [l, r] for a 
given array. I used the Sparse Table data structure to efficiently solve this problem.

In the Sparse Table, each cell stores the indices of the first and second smallest numbers for the corresponding subarray. When moving to the next level of the 
Sparse Table, I only kept the first two distinct smallest indices out of the four possibilities. This process was repeated iteratively to find the second smallest
number efficiently for each query.
