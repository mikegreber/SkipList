## Skip List

A skip list is a data structure for holding an ordered sequence with O(logn) search, insertion, and deletion time complexity.

A standard ordered array allows for O(logn) search time (using a binary search), but insertion and deletion have O(n) time 
complexity. A standard linked list allows for O(1) insertion and deletion time complexity, but has O(n) search time complexity. 
A skip list achieves the fast search time complexity of an ordered array with the quick insertion/deletion time of a linked list.

The skip list is a linked list-like structure that builds a linked hierarchy of subsequences, going from more to less sparse
each subsequences, that enables O(logn) search time complexity (like a sorted array), and O(1) insertion and deletion time 
complexity of a linked list. The skip list is therefore able insert/delete elements in their sorted position in O(logn) time 
(O(logn) to find the sorted position, and O(1) time for the insertion/deletion). This faster speed comes with the cost of 
an increased memory footprint (at least double) compared to a standard linked list.

This implementation achieves O(logn) runtime for Insert(), Remove(), and Contains() at high n. 


### Files

#### skip_list.h
   - contains the skip list template data structure.

#### skip_list_test.h
   - contains the skip list wrapped to implement the sorted_list.h interface for performance comparison.

#### sorted_linked_list.h
   - contains a template for a sorted linked list using std::list along with std::find algorithms 
     for performance comparison.

#### sorted_vector.h
   - contains a template for a sorted vector using std::vector along with SLT binary search algorithms 
     for performance comparison.

#### sorted_list.h
   - contains an interface class requiring GetName(), Insert(), Remove(), Contains(), Clear(), Fill(), Size(), and AsVector() methods. 
   - skip_list, sorted_linked_list, and sorted_vector implement this interface to simplify testing and performance comparisons.

#### tests.h / tests.cpp
   - contain test logic for running performance tests, correctness tests, and for an interactive visual
     test of the skip list.

#### main.cpp
   - contains program interface logic giving options for tests to run.


### Compilation/Running Instructions

1. Open a shell with g++ installed in the SkipList directory


2. Enter the command below, it will build and run the program:
       
       make run


### Output Interpretation

1. Performance Test

   - Options presented to run skip list performance test against either or both sorted linked list and sorted vector list. 
   - Reports and compares execution time for Insert(), Remove(), and Contains() for the tested lists.
   - Results include raw execution time in milliseconds, and the comparative % speed up of skip list versus the other lists
     for each method.
   - Results are reported in a table after each method test, as well as in a summary at the end of the test.
   - The speed advantages of the skip list will improve with higher n, but so will the test time since the performance test
     compares against the slower sorted linked list and sorted vector list.
   - The skip list will be outperformed at small n for some operations by the other lists due to having a higher base overhead
     for managing the extra layers. Contains() (search only) will have a similar execution time between skip list and sorted 
     vector list at high n.


2. Correctness Test
   - Tests all 3 list types after many executions of each of Insert(), Remove(), and Contains().
   - Ensures each list remains in the correct sorted state after each function call.
   - Should just get a Passed message (test makes sure implementations are working correctly).
   

3. Free Test
   - An interactive test showing the internal state of the skip list after each method call.
   - Initial n input auto populates the list. Don't make this too big or this won't be very useful as a visualization tool.
   - Input instructions are given as a prompt, with options to Insert(), Remove(), Contains(), and Clear(). As an example, to
     call Insert(5), enter:
   
         i5 
   
   - Entering q will return to the main menu.



### Resources

1. https://en.wikipedia.org/wiki/Skip_list


### Credits

Author: Mike Greber

Instructor: Zachary Friggstad

University of Alberta - CMPUT 403 Winter 2022.
