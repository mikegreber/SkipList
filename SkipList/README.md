## Skip List

A skip list is a data structure for holding an ordered sequence with O(logn) search, insertion, and deletion time complexity.

A standard ordered array allows for O(logn) search time (using quick sort or merge sort for example), but insertion and
deletion have O(n) time complexity. A standard linked list allows for O(logn) insertion and deletion time complexity,
but has O(n) search time complexity.

The skip list is a linked list-like structure that builds a linked hierarchy of subsequences, allowing for O(logn) 
search time complexity (like a sorted array) combined with the O(logn) insertion and deletion time complexity of
a standard linked list. This faster speed comes with the cost of an increased memory footprint (roughly double) 
compared to a standard linked list. This implementation meets these criteria.

### Files

#### skip_list.h
   - contains the skip list template data structure.

#### sorted_container.h
   - contains a template implementation for making sorted lists with STL containers and algorithms 
     for performance comparison. Used to create a sorted vector list and a sorted linked list for performance
     comparisons with skip list.

#### sorted_list.h
   - contains an interface class requiring Insert(), Remove(), Contains(), and Size() methods. 
   - skip_list and sorted_container implement this interface to simplify testing and performance comparisons.

#### tests.h / tests.cpp
   - contain test logic for running performance tests, correctness tests, and for an interactive visual
     test of the skip list.

#### main.cpp
   - contains program interface logic giving options for tests to run.


### Compilation/Running Instructions

1. Open a shell with g++ installed in the SkipList directory


2. Enter the command below, it will build and run the program:
       
       make

3. To just run the program after building, enter the command:

       make run

### Output Interpretation

1. Performance Test

   - Reports and compares execution time for Insert(), Remove(), and Contains() for skip list, sorted linked list,
     and sorted vector list.
   - Results include raw execution time in milliseconds, and the comparative % speed up of skip list versus the other lists
     for each method.
   - Results are reported in a table after each method test, as well as in a summary at the end of the test.


2. Correctness Test
   - Tests all 3 list types after many executions of each of Insert(), Remove(), and Contains().
   - Ensures each list remains in the correct sorted state after each function call.
   - Should just get a Passed message (test makes sure implementations are working correctly).
   

3. Free Test
   - An interactive test showing the internal state of the skip list after each method call.
   - Initial n input auto populates the list. Don't make this too big or this won't be very useful as a visualization tool.
   - Input instructions are given as a prompt, with options to Insert(), Remove(), Contains(), and Clear(). As an example, to
     call Input(5), enter:
   
         i5 
   
   - Entering q will return to the main menu.



### Resources
1. https://en.wikipedia.org/wiki/Skip_list