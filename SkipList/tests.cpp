/*
* Test functions for comparing skip_list and sorted_linked_list performance and correctness.
*/
#include <chrono>
#include <functional>
#include <random>
#include <vector>
#include <string>
#include <list>

#include "tests.h"

#include <forward_list>
#include <ostream>

#include "skip_list_test.h"
#include "sorted_linked_list.h"
#include "sorted_vector.h"


class test_class
{
public:
	test_class(const unsigned long long val) : val(val) {}
	
	// required for testing setup for fill() function
	test_class& operator++()
	{
		++val;
		return *this;
	}

	// required for testing setup for fill() function
	test_class& operator--()
	{
		--val;
		return *this;
	}

	// required for stl find functions for sorted_vector and sorted_linked_list
	friend bool operator==(const test_class& lhs, const test_class& rhs) { return lhs.val == rhs.val; }
	
	bool operator<(const test_class& other) const { return val < other.val; }
	friend std::ostream& operator<<(std::ostream& os, const test_class& obj){ return os << obj.val; }
	
private:
	unsigned long long val;
};

/*
 * calls Insert() with all elements in input on list
 */
template<typename T, typename L>
void insertList(const std::vector<T>& input, L& list)
{
	for (const auto i : input) list.Insert(i);
}


/*
 * calls Remove() with all elements in input on list
 */
template<typename T, typename L>
void removeList(const std::vector<T>& input, L& list)
{
	for (const auto i : input) list.Remove(i);
}


/*
 * calls Contains() with all elements in input on list
 */
template<typename T, typename L>
void containsList(const std::vector<T>& input, L& list)
{
	for (const auto i : input) list.Contains(i);
}


/*
 * returns true list is sorted in non-decreasing order
 */
template <typename L>
bool isSorted(const L& list)
{
	auto vector = list.AsVector();
	for (unsigned i = 1; i < vector.size(); ++i)
		if (vector[i] < vector[i-1])
		{
			std::cout << "   Fail!" << std::endl;
			std::cout << "     " << list.GetName() << " order invalid!" << std::endl;
			return false;
		}
			
	return true;
}

/* equality comparison using only < operator */
template <typename T>
bool equal(T a, T b) { return (a < b) == (b < a); }

/*
 * returns true if all sorted lists are equivalent
 */
template <typename T>
bool equal(const std::vector<sorted_list<T>*>& lists)
{
	if (lists.empty()) return true;

	std::vector<std::vector<T>> vectors;
	vectors.reserve(lists.size());
	
	for (auto list : lists)
		vectors.push_back(list->AsVector());

	for (unsigned i = 0; i < vectors[0].size(); ++i)
		for (unsigned j = 1; j < vectors.size(); ++j)
			if (!equal(vectors[0][i], vectors[j][i]))
			{
				std::cout << "   Fail!" << std::endl;
				std::cout << "     " << lists[0]->GetName() << " and " <<
					lists[j]->GetName() << " are not equivalent!" << std::endl;
				
				return false;
			}
	
	return true;
}


/*
 * Gets time in microseconds to run function() averaged over a number of repetitions.
 * before() is called prior to timing function() for any needed setup.
 */
unsigned long long time(const std::string& message, const std::function<void()>& before, const std::function<void()>& function, unsigned repetitions = 1)
{
	assert(repetitions >= 1);
	
	std::cout << message <<  " (" << repetitions << " repetitions)" << std::endl;

	unsigned long long duration = 0;
	const unsigned runs = repetitions;
	
	while (repetitions--)
	{
		before();
		
		const auto start = std::chrono::high_resolution_clock::now();
		function();
		const auto stop = std::chrono::high_resolution_clock::now();
		const auto t = std::chrono::duration_cast<std::chrono::microseconds>(stop-start).count();
		
		std::cout << "                      time: " << t << " microseconds" << std::endl;
		duration += t;
	}

	duration /= runs;
	
	std::cout << "    Average execution time: " << duration << " microseconds" << std::endl;

	return duration;
}


/*
 * gets an input of type T, repeats and displays error message if unsuccessful and repeat = true, and clears the input stream
 */
template <typename T>
bool getInput(T& integer, const std::string& fail_message = " Invalid input ", bool repeat = true)
{
	while (true)
	{
		std::cout << " ";
		if (std::cin >> integer) return true;
		if (!fail_message.empty()) std::cout << fail_message << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (!repeat) return false;
	}
}


/*
 * Runs a performance test comparing the skip list implementation with a sorted linked list
 */
void run_performance_test()
{

std::forward_list<int> t;
	
	struct results
	{
		results(std::string name)
		: list_name(std::move(name)), insert_time(0), remove_time(0), contains_time(0) {}
		std::string list_name;
		unsigned long long insert_time;
		unsigned long long remove_time;
		unsigned long long contains_time;
	};
	
	std::cout << "\n******************************************************************************************************" << std::endl;
	std::cout << "\n Performance tests for Skip List vs Sorted Linked List and Sorted Vector List\n" << std::endl;
	
	std::random_device rd;
	std::mt19937 g(rd());
	
	// skip_list_test<unsigned long long> skip_list;
	// sorted_linked_list<unsigned long long> linked_list;
	// sorted_vector<unsigned long long> vector_list;
	//
	// std::vector<sorted_list<unsigned long long>*> lists { &skip_list };

	skip_list_test<test_class> skip_list;
	sorted_linked_list<test_class> linked_list;
	sorted_vector<test_class> vector_list;

	std::vector<sorted_list<test_class>*> lists { &skip_list };
	
	std::vector<results> results{ skip_list.GetName() };
	

	std::cout << "\n Compare with Sorted Linked List (slow)? (y/n): ";
	char compare_linked = '0';
	while (getInput(compare_linked) && compare_linked != 'y' && compare_linked != 'n')
		std::cout << "\n                                         (y/n): ";
	
	if (compare_linked == 'y')
	{
		lists.push_back(&linked_list);
		results.emplace_back(linked_list.GetName());
	}
	
	std::cout << "\n             Compare with Sorted Vector? (y/n): ";
	char compare_vector = '0';
	while (getInput(compare_vector) && compare_vector != 'y' && compare_vector != 'n')
		std::cout << "\n                                         (y/n): ";
	
	if (compare_vector == 'y')
	{
		lists.push_back(&vector_list);
		results.emplace_back(vector_list.GetName());
	}

	if (compare_linked == 'y') 
		std::cout << "\n ** N < 10,000 recommended for Sorted Linked List test (slow search) **" << std::endl;
	else if (compare_vector == 'y')
		std::cout << "\n ** N > 100,000 recommended for Sorted Vector List test to show benefit of Skip List **" << std::endl;
	
	std::cout << "\n                 Enter N for performance tests: ";
	long long n;
	getInput(n);
	
	std::cout << "\n                    Enter repetitions per test: ";
	unsigned int repetitions;
	getInput(repetitions);
	
	constexpr int multiplier = 5;
	std::vector<unsigned long long> input(n);
	for (long long i = 0; i < n; ++i) input[i] = i * multiplier;
	const long long n_existing = n * multiplier;
	
	std::cout << " -----------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\n Testing Insert() for";
	for (const auto list : lists) std::cout << " { " << list->GetName() << " }";
	std::cout <<" by\n inserting " << n << " elements in random order into a list containing " << n_existing << " elements." << std::endl;

	for (unsigned i = 0; i < lists.size(); ++i)
	{
		auto& list = lists[i];
		auto& result = results[i];
	
		result.insert_time = time(
		"\n  Testing Insert() for " + list->GetName(),
		[&]
		{
			// refill list
			list->Fill(0, n_existing);
		
			// shuffle input
			std::shuffle(input.begin(), input.end(), g);
		},
		[&]()
		{
			// call Insert() on list with all inputs
			insertList(input, *list);
		},
		repetitions);
	}

	std::cout << "\n\n Insert() Results (ms = microseconds):\n" << std::endl;
	std::cout << "                     ";
	for (const auto& result : results) printf("%20s", result.list_name.c_str());
	std::cout << std::endl;
	std::cout << " Execution Time      ";
	for (const auto& result : results) printf("%17lld ms", result.insert_time);
	std::cout << std::endl;
	std::cout << " Skip List Speed Up %";
	for (unsigned i = 0; i < results.size(); ++i)
		printf("%19.2f%%", 100 * static_cast<double>(results[i].insert_time) / static_cast<double>(results[0].insert_time) - 100);
	std::cout << std::endl;




	std::cout <<"\n -----------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\n Testing Remove() for";
	for (const auto list : lists) std::cout << " { " << list->GetName() << " }";
	std::cout << " by\n calling Remove() with " << n << " elements on a list containing " << n_existing / 2 << " elements." <<
		"\n 50% of calls will be misses. " << std::endl;

	for (unsigned i = 0; i < lists.size(); ++i)
	{
		auto& list = lists[i];
		auto& result = results[i];
	
		result.remove_time = time(
		"\n  Testing Remove() for " + list->GetName(),
		[&]
		{
			// refill list with only middle elements
			list->Fill(n_existing / 4, n_existing * 3/4);
		
			// shuffle input
			std::shuffle(input.begin(), input.end(), g);
		},
		[&]()
		{
			// call Remove() on list with all inputs
			removeList(input, *list);
		},
		repetitions);
	}

	std::cout << "\n\n Remove() Results (ms = microseconds):\n" << std::endl;
	std::cout << "                     ";
	for (const auto& result : results) printf("%20s", result.list_name.c_str());
	std::cout << std::endl;
	std::cout << " Execution Time      ";
	for (const auto& result : results) printf("%17lld ms", result.remove_time);
	std::cout << std::endl;
	std::cout << " Skip List Speed Up %";
	for (unsigned i = 0; i < results.size(); ++i)
		printf("%19.2f%%", 100 * static_cast<double>(results[i].remove_time) / static_cast<double>(results[0].remove_time) - 100);
	std::cout << std::endl;




	std::cout <<"\n -----------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\n Testing Contains() for";
	for (const auto list : lists) std::cout << " { " << list->GetName() << " }";
	std::cout << " by\n calling Contains() with " << n << " elements on a list containing " << n_existing / 2 << " elements." <<
		"\n 50% of calls will be misses. " << std::endl;

	for (const auto list : lists)
	{
		// refill list with only middle elements
		list->Fill(n_existing / 4, n_existing * 3/4);
	}

	for (unsigned i = 0; i < lists.size(); ++i)
	{
		auto& list = lists[i];
		auto& result = results[i];
	
		result.contains_time =  time(
			"\n  Testing Contains() for " + list->GetName(),
			[&]
			{
				// shuffle input
				std::shuffle(input.begin(), input.end(), g);
			},
			[&]()
			{
				// call Contains() on list with all inputs
				containsList(input, *list);
			},
			repetitions);
	}

	std::cout << "\n\n Contains() Results (ms = microseconds):\n" << std::endl;
	std::cout << "                     ";
	for (const auto& result : results) printf("%20s", result.list_name.c_str());
	std::cout << std::endl;
	std::cout << " Execution Time      ";
	for (const auto& result : results) printf("%17lld ms", result.contains_time);
	std::cout << std::endl;
	std::cout << " Skip List Speed Up %";
	for (unsigned i = 0; i < results.size(); ++i)
		printf("%19.2f%%", 100 * static_cast<double>(results[i].contains_time) / static_cast<double>(results[0].contains_time) - 100);
	std::cout << std::endl;

	
	
	std::cout <<"\n -----------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "\n Performance results for " << n << " method calls for";
	for (const auto list : lists) std::cout << " { " << list->GetName() << " }";
	std::cout << ":\n" << std::endl;
    
	std::cout << " ms = microseconds" << std::endl;
	std::cout << "                     ";
	for (const auto& result : results) printf("%20s", result.list_name.c_str());
	std::cout << std::endl;
	
	std::cout << " Insert():" << std::endl;
	std::cout << " Execution Time      ";
	for (const auto& result : results) printf("%17lld ms", result.insert_time);
	std::cout << std::endl;
	std::cout << " Skip List Speed Up %";
	for (unsigned i = 0; i < results.size(); ++i)
		printf("%19.2f%%", 100 * static_cast<double>(results[i].insert_time) / static_cast<double>(results[0].insert_time) - 100);
	std::cout << std::endl;

	std::cout << "\n Remove():" << std::endl;
	std::cout << " Execution Time      ";
	for (const auto& result : results) printf("%17lld ms", result.remove_time);
	std::cout << std::endl;
	std::cout << " Skip List Speed Up %";
	for (unsigned i = 0; i < results.size(); ++i)
		printf("%19.2f%%", 100 * static_cast<double>(results[i].remove_time) / static_cast<double>(results[0].remove_time) - 100);
	std::cout << std::endl;
	
	std::cout << "\n Contains():" << std::endl;
	std::cout << " Execution Time      ";
	for (const auto& result : results) printf("%17lld ms", result.contains_time);
	std::cout << std::endl;
	std::cout << " Skip List Speed Up %";
	for (unsigned i = 0; i < results.size(); ++i)
		printf("%19.2f%%", 100 * static_cast<double>(results[i].contains_time) / static_cast<double>(results[0].contains_time) - 100);
	std::cout << std::endl;
	
	std::cout << "\n *Skip List Speed Up % = 100 * (List Execution Time) / (Skip List Execution Time) - 100" << std::endl;
}


/*
 * Runs correctness tests on skip-list
 */
void run_correctness_test()
{
	std::cout << "\n******************************************************************************************************"<< std::endl;
	std::cout << "\n Correctness tests for Skip List\n" << std::endl;

	constexpr int n = 1000;
	std::vector<unsigned long long> input(n);
	for (int i = 0; i < n; ++i) input[i] = i;

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(input.begin(), input.end(), g);
	
	skip_list_test<unsigned long long> skip_list;
	sorted_linked_list<unsigned long long> linked_list;
	sorted_vector<unsigned long long> vector_list;

	const std::vector<sorted_list<unsigned long long>*> lists { &skip_list, &linked_list, &vector_list };
	
	std::cout << " - checking if skip list, sorted linked list, and sorted vector list remain sorted and equivalent" <<
        "\n   with correct size after Insert():";
    
	for (const auto i : input)
	{
		for (const auto list : lists)
		{
			auto size = list->Size();
			list->Insert(i);
			++size;
			
			if (!isSorted(*list)) return;
			
			if (size != list->Size())
			{
				std::cout << "   Fail!" << std::endl;
				std::cout << "     " << list->GetName() << " incorrect size after Insert()!" << std::endl;
				return;
			}
		}
		
		if (!equal(lists)) return;
	}
	std::cout << "\n   Passed!\n" << std::endl;
	

	std::cout << " - checking if skip list, sorted linked list, and sorted vector list remain sorted and equivalent" <<
        "\n   with correct size after Remove() (no misses):";
    
	for (const auto i : input)
	{
		for (const auto list : lists)
		{
			auto size = list->Size();
			size -= list->Remove(i);
			
			if (!isSorted(*list)) return;
			
			if (size != list->Size())
			{
				std::cout << "   Fail!" << std::endl;
				std::cout << "     " << list->GetName() << " incorrect size after Remove()!" << std::endl;
				return;
			}
		}
		
		if (!equal(lists)) return;
	}
	std::cout << "\n   Passed!\n" << std::endl;

	
	// refill lists with inputs
	for (const auto list : lists) insertList(input, *list);
	if (!equal(lists)) return;
	
	constexpr auto n_half = n>>1;
	
	std::cout << " - checking if skip list, sorted linked list, and sorted vector list return the same response" <<
        "\n   for Contains() (50% misses):";
    
	for (const auto i : input)
	{
		const bool contains = lists[0]->Contains(i + n_half);
		for (unsigned j = 1; j < lists.size(); ++j)
		{
			if (contains != lists[j]->Contains(i + n_half))
			{
				std::cout << "   Fail!" << std::endl;
				std::cout << "     " << lists[0]->GetName() <<  " and " <<
					lists[j]->GetName() << " Contains() returned different results!" << std::endl;
				return;
			}
		}
	}
	std::cout << "\n   Passed!\n" << std::endl;

	
	std::cout << " - checking if skip list, sorted linked list, and sorted vector list remain sorted and equivalent" <<
        "\n   with correct size after Remove() (50% misses):";
    
	for (const auto i : input)
	{
		for (const auto list : lists)
		{
			auto size = list->Size();
			size -= list->Remove(i + n_half);
			
			if (!isSorted(*list)) return;
			
			if (size != list->Size())
			{
				std::cout << "   Fail!" << std::endl;
				std::cout << "     " << list->GetName() << " incorrect size after Remove()!" << std::endl;
				return;
			}
		}
		
		if (!equal(lists)) return;
	}
	std::cout << "\n   Passed!\n" << std::endl;

	std::cout << " Correctness test passed!" << std::endl;
}


/*
 * Runs an interactive test of a skip-list
 */
void run_demo()
{
	std::cout << "\n******************************************************************************************************"<< std::endl;
	std::cout << "\n Free Test for Skip List\n" << std::endl;
	
	std::cout << "\n Enter initial n for skip list (20 recommended): ";
	int n;
	getInput(n);
	
	skip_list<long long> skip_list;
	if (n > 0)
	{
		// generate randomized input
		std::vector<long long> input;
		input.push_back(1);
		for (int i = 1; i < n; ++i)
            input.push_back(input.back() + 1 + 10
                            * static_cast<long long>(static_cast<float>(rand()))/RAND_MAX);

		// shuffle input
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(input.begin(), input.end(), g);
	
		for (const auto i : input)
		{
			std::cout << " Insert(" << i << ")" << std::endl;
			skip_list.Insert(i);
			skip_list.Print(true);
		}
	}
	else skip_list.Print();
	
	bool loop = true;
	char c;
	long long i;
	
	do
	{
		std::cout << "\n Enter i followed by int for Insert(int)" << std::endl;
		std::cout << " Enter r followed by int for Remove(int)" << std::endl;
		std::cout << " Enter c followed by int for Contains(int)" << std::endl;
		std::cout << " Enter x to Clear() the skip-list" << std::endl;
		std::cout << " Enter q to quit free test\n" << std::endl;
		
		if (!getInput(c, " Invalid input", false)) continue;
		
		switch (c)
		{
			// Remove()
		case 'r':
			{
				if (!getInput(i, "Invalid input", false)) continue;
				std::cout << "Remove(" << i << ") => ";
				std::cout << (skip_list.Remove(i) ? "True" : "False") << std::endl;
				skip_list.Print(true);
				break;
			}
			// Contains()
		case 'c':
			{
				if (!getInput(i,  "Invalid input", false)) continue;
				std::cout << "Contains(" << i << ") => ";
				std::cout << (skip_list.Contains(i) ? "True" : "False") << std::endl;
				skip_list.Print(true);
				break;
			}
			// Insert()
		case 'i':
			{
				if (!getInput(i, "Invalid input", false)) continue;
				std::cout << "Insert(" << i << ")" << std::endl;
				skip_list.Insert(i);
				skip_list.Print(true);
				break;
			}
			// Clear()
		case 'x':
			{
				skip_list.Clear();
				skip_list.Print(true);
				break;
			}
		case 'q':
			{
				loop = false;
				break;
			}
		default:
			{
				std::cout << " Invalid input\n" << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
		}
		
	} while (loop);
}



