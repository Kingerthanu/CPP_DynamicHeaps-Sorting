// Benjamin R Spitzauer
// lab5.cpp
// 3/9/2023

/*

  Purpose: This lab displays understanding of sorting algorithms and their 
  techniques for traversal for comparisons. This is done with 3 different types
  of sorting, heap, merge, and quick sort. Each will be tested with varying 
  input sizes and their computational time will be compared to eachother to 
  show how each handles under varying test sizes. This will show how different
  techniques can do better than others when getting to bigger inputs as well as
  how comparison-based algorithm's architeture can look like.

*/

#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
#include <random>

using namespace std; 


// ~~~~~~~~~~~~~~~~~~~~~Prototypes~~~~~~~~~~~~~~~~~~~~~

/*

  Pre-Condition: Need array containing elements to swap as well as each index.

  Post-Condition: Will swap A[parentIndex]'s value to A[childIndex]; likewise
  for A[childIndex]'s value which will be then placed into A[parentIndex].

*/
void swap(int A[], int parentIndex, int childIndex);

/*

  Pre-Condition: Array to sort and its size.

  Post-Condition: Will create a temporary heap with values of A. These
  values will be sorted in heap-ordering fashion and will be removed
  based on priority which will be its int value of each element in the
  heap. These popped values from the heap will be added into the A
  array, sorting it by the end as lowest priority will be lowest
  value in A.

*/
void heapSort(int A[], int n);

/*

  Pre-Condition: Array of values to merge; start, middle, and end index of
  array.

  Post-Condition: Will merge together the left and right side of A[] in 
  ascending order.

*/
void merge(int A[], int s, int m, int e);

/*

  Pre-Condition: Array of values to merge; start, and end index of array.

  Post-Condition: Will split from s -> e into two arrays and add them
  together after to sort them in ascending order (merge-sort algorithm).

*/
void mergeSort(int A[], int s, int e);

/*

  Pre-Condition: Array of values to sort and the size of the array.
  Post-Condition: Will call mergeSort(A, s, e) which works as the merge-sort
  algorithm.

*/
void mergeSort(int A[], int n);

/*

  Pre-Condition: Array of values to partition; start, and end index of array.

  Post-Condition: Will select a pivot index of a 0th element in the array
  and will compare all other values to this value and will place them in a 
  lesser (or equal) or greater than bucket. After it will swap with the last
  element in the lesser or equal to bucket with the pivot and return the new
  spot of the pivot value.

*/
int partition(int A[], int s, int e);

/*

  Pre-Condition: Array of values to merge; start, and end index of array.

  Post-Condition: Will split from s -> e into two arrays after partitioning
  in ascending order from a -> e (quick-sort algorithm).

*/
void quickSort(int A[], int s, int e);

/*

  Pre-Condition: Array of values to sort and the size of the array.

  Post-Condition: Will call quickSort(int A[], int s, int e) which works as
  the quick-sort algorithm.

*/
void quickSort(int A[], int n);

/*

  Pre-Condition: Array of values to merge; start, and end index of array.

  Post-Condition: Will grab a random element in the array and will use it
  as a pivot in quick-sort algorithm by swapping it with the first and then
  running partition to sort the sub arrays.

*/
int randomized_partition(int A[], int s, int e);

/*

  Pre-Condition: Either empty; or with a array and its size to set to a heap.
  Post-Condition: Initialize this->heap either to empty or to given array.

*/
class heap;

// ~~~~~~~~~~~~~~~~~END Prototypes~~~~~~~~~~~~~~~~~~~~~


class Heap
{ 

  private:           

    vector<int> heap;

    /*

      Pre-Condition: Index of element in heap.

      Post-Condition: Will place the value inside the index's element into its
      parent node if it's greater than its parent (max-heap). It will then call
      itself again to check oncemore at this new index if it's in the right
      spot in the heap according to its priority.

    */
    void percolateUp(int index); 

    /*

      Pre-Condition: None.
      Post-Condition: Returns index 0 in the vector which should be highest
      prioritied pet and returns it as a pointer. Else, return exception if
      it's empty.

    */
    int peekMax(); 

    /*

      Pre-Condition: Index of element in heap.

      Post-Condition: Will place the value inside the index's element into its
      greatest prioritied child node if it's greater than its child (max-heap).
      It will then call itself again to check oncemore at this new index if 
      it's in the right spot in the heap according to its priority.

    */
    void percolateDown(int index); 

    /*

      Pre-Condition: None.

      Post-Condition: Removes root element and swaps it with the last element
      in the heap. This element is then checked with percolate down to make
      sure it's added in the right spot of the heap to maintain the heap
      property.

    */
    void deleteMax(); 

    /*

      Pre-Condition: Index of child to swap; index of parent to swap w/ child.
      Post-Condition: Will swap the Nodes positions with one another.
      Invariant: Should always be indexes in heap.

    */
    void swap(int childIndex, int parentIndex);

  public: 

    /*

      Pre-Condition: A pointer to a int to add into the heap.

      Post-Condition: Inserts int into heap at end and calls percolate
      up to find its spot in the heap.

    */
    void insert(int num); 

    /*

      Pre-Condition: None.

      Post-Condition: Returns pointer to the greatest int in the heap and
      removes them from heap. If the heap is empty, it will throw a error.

    */
    int adoptHighest(); 

    /*

      Pre-Condition: None.
      Post-Condition: Will initalize this->heap.

    */
    Heap()
    {  

      heap = vector<int>();

    } 

    /*

      Pre-Condition: Array to copy into a heap and its size.
      Post-Condition: Will insert all values from 0-size in
      newArray into this->heap.

    */
    Heap(int* newArray, int size)
    {
      heap = vector<int>();

      for(int i = 0; i < size; i++){  // Add all from newArray to this->heap
        this->insert(newArray[i]); 
      }
    }

}; 

 
void Heap::swap(int childIndex, int parentIndex)
{

  int tmp = this->heap[parentIndex];

  this->heap[parentIndex] = this->heap[childIndex];
  this->heap[childIndex] = tmp;

}

int Heap::peekMax()
{ 

  if(((int)this->heap.size()) == 0){ // If empty
    throw std::underflow_error("Heap is empty.");
  }

  return this->heap[0];

} 

void Heap::percolateUp(int index)
{ 
  if(index == 0){
    return;
  }

  int parentIndex = (index-1)/2;

  if(this->heap[index] > this->heap[parentIndex]){ // If child > parent
    this->swap(index, parentIndex);
  }
  else{ // If in it's correct spot, stop
    return;
  }

  percolateUp(parentIndex); // Now check this spot, and it's parent

} 

 
void Heap::percolateDown(int index)
{ 

  int leftChildIndex = (index*2)+1;
  int rightChildIndex = (index*2)+2;
  int swapChildIndex; // Contains index of child for next recursive call

  if(((leftChildIndex) >= ((int)this->heap.size())) && ((rightChildIndex) >= ((int)this->heap.size()))){ // Leaf
    return;
  }
  else if(((rightChildIndex) >= ((int)this->heap.size()))){ // One left child

    if(this->heap[index] < this->heap[leftChildIndex]){

      this->swap(index, leftChildIndex);
      swapChildIndex = leftChildIndex; // Set for recursive call

    }
    else{ // Else in correct spot
      return;
    }
  }
  else{ // Two children
  
    // Get greatest child index
    if(this->heap[leftChildIndex] >= this->heap[rightChildIndex]){
      swapChildIndex = leftChildIndex;
    }
    else{
      swapChildIndex = rightChildIndex;
    }

    if(this->heap[index] < this->heap[swapChildIndex]){
      swap(index, swapChildIndex);
    }
    else{ // Else node is in correct spot
      return;
    }
  }

  percolateDown(swapChildIndex); // Now check new spot and its children

}

 
void Heap::deleteMax()
{ 

  this->swap(0, this->heap.size()-1);

  this->heap.pop_back(); // Remove max

  // Check new root if right spot in heap
  this->percolateDown(0);

} 

 
void Heap::insert(int num)
{ 
    
  this->heap.push_back(num); // Add to end

  // Check element's parent if just in heap (size-1 as index starts at 0)
  this->percolateUp(this->heap.size()-1);

} 

 
int Heap::adoptHighest()
{ 

  if(((int)this->heap.size()) == 0){
    throw std::underflow_error("Heap is empty.");
  }

  int oldest = this->peekMax();

  this->deleteMax(); // Now remove max

  return oldest; 

} 


void swap(int A[], int parentIndex, int childIndex)
{

  int tmp = A[parentIndex];

  A[parentIndex] = A[childIndex];
  A[childIndex] = tmp;

}


void heapSort(int A[], int n)
{ 
  
  Heap toSort(A, n);  // Create heap based on A

  for(int i = n-1; i >= 0; i--){  // First adopt will be greatest so count down
    A[i] = toSort.adoptHighest();
  }

} 


void merge(int A[], int s, int m, int e)
{

  vector<int> B((e-s) + 1);

  int left = s;
  int right = m+1;
  int bIndex = 0; // Will index B[] for next avaiable spot

  while(left != m+1 && right != e+1){

    if(A[left] <= A[right]){ // If left subarray has least
      B[bIndex] = A[left];
      left++;
    }
    else{
      B[bIndex] = A[right];
      right++;
    }

    bIndex++;

  }

  while(left != m+1){   // If left still has values put them in
    B[bIndex] = A[left];
    left++;
    bIndex++;
  }

  while(right != e+1){ // If right still has values put them in
    B[bIndex] = A[right];
    right++;
    bIndex++;
  }

  for(int c = 0; s != e+1; c++, s++){ // Set each in sublist of A to B
    A[s] = B[c];

  }

  

}


int randomized_partition(int A[], int s, int e) 
{

  int pos = s + rand() % (e-s+1); // Get random index in index-range

  swap(A[s], A[pos]); // Set to first for sorting

  return partition(A, s, e);  // Now sort with random pivot

}


void mergeSort(int A[], int s, int e)
{

  if(s < e){

    int mid = (s+e) / 2;

    mergeSort(A, s, mid); // Start to middle
    mergeSort(A, mid+1, e); // Middle to end

    merge(A, s, mid, e); // Combine parts

  }

}


void mergeSort(int A[], int n)
{ 
  
  mergeSort(A, 0, n-1);

} 


int partition(int A[], int s, int e)
{

  int piv = s;
  int m = piv+1; // Starts one after pivot

  for(int i = m; i <= e; i++){

    if(A[i] <= A[piv]){ // Place in lesser sublist
      swap(A, m, i);
      m++;

    }

    // Else, place in greater sublist (i++)

  }
  
  swap(A, piv, m-1); // swap with last in less than sublist

  return m-1;

}


void quickSort(int A[], int s, int e)
{

  if(s < e){
  
    int pivotInd = randomized_partition(A, s, e);

    // Now check both subarrays and their values (not pivot because sorted)
    quickSort(A, s, pivotInd-1);
    quickSort(A, pivotInd+1, e);

  }

}


void quickSort(int A[], int n)
{ 

  quickSort(A, 0, n-1); 

} 
 

int main() 
{ 

  // Sizes of input to test
  int size[] = {10, 100, 1000, 10000, 100000}; 

  std::random_device rand;   // Making seeded gen
  std::mt19937 seed(rand()); // Seeding with quicker gen with seed
  std::uniform_int_distribution<int> randVal(0, 99999); // Range of values

  // Set file to write results into
  ofstream myTimes("sortingTimes.txt", fstream::app);

  cout << "\n--------------------------------------------------------------";

  for(int i = 0; i < 5; i ++){ 

    int* unsortedArray = new int[size[i]];  
    cout << "\n\nThe current array size is: " << size[i] << endl; 

    // Set random values
    for(int j=0; j<size[i]; j++){ 
      unsortedArray[j] = randVal(seed); 
    } 

    if (size[i] == 10){ 

      cout << "\nUnsorted (Same for each algorithm): \n";

      for(int j=0; j<size[i]; j++){ 
        cout << unsortedArray[j] << ", "; 
      }
      cout << endl; 

    }

    // Use same values for quicksort, mergesort, and heapsort (unordered)
    int* unsortedArray2 = new int[size[i]];  
    int* unsortedArray3 = new int[size[i]]; 

    // Set all sortings to same unordered set of values
    for(int j=0; j < size[i]; j++){
      unsortedArray3[j] = unsortedArray[j];
      unsortedArray2[j] = unsortedArray[j];
    }


    myTimes << "Set " << i+1 << " size " << size[i] << " dataset \n";

    //~~~~~~~~~~~~~~~~~~~~~~~~~Getting Runtimes~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    auto start = chrono::high_resolution_clock::now();
    quickSort(unsortedArray, size[i]);
    auto stop = chrono::high_resolution_clock::now();

    auto duration = stop - start;
    auto duration_ns = chrono::duration_cast<chrono::nanoseconds>(duration);
    myTimes << "Quicksort time - " << duration_ns.count() << '\n';

    start = chrono::high_resolution_clock::now();
    mergeSort(unsortedArray2, size[i]);
    stop = chrono::high_resolution_clock::now();

    duration = stop - start;
    duration_ns = chrono::duration_cast<chrono::nanoseconds>(duration);
    myTimes << "Mergesort time - " << duration_ns.count() << '\n';

    start = chrono::high_resolution_clock::now();
    heapSort(unsortedArray3, size[i]);
    stop = chrono::high_resolution_clock::now();

    duration = stop - start;
    duration_ns = chrono::duration_cast<chrono::nanoseconds>(duration);
    myTimes << "Heapsort time - " << duration_ns.count() << "\n\n";

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    if(size[i] == 10){  // Show sorted of size 10 to user for each algorithm

      cout << "\nSorting via Quick-Sort: \n";
      for(int j=0; j < size[i]; j++){ 
        cout << unsortedArray[j] << ", "; 
      }

      cout << "\n\nSorting via Merge-Sort: \n";
      for(int j=0; j < size[i]; j++){ 
        cout << unsortedArray2[j] << ", "; 
      }

      cout << "\n\nSorting via Heap-Sort: \n"; 
      for(int j=0; j < size[i]; j++){ 
        cout << unsortedArray3[j] << ", "; 
      }

      cout << "\n\n----------------------------------------------------------";

    }


    // Free after use
    delete[] unsortedArray;
    delete[] unsortedArray2;
    delete[] unsortedArray3;

  } 

  myTimes << "\n\n\n";
  myTimes.close();

  cout << "\n\n\n"; // Clear from sys prompts

} 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 