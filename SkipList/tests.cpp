/*
* Test functions for comparing skip_list and sorted_linked_list performance and correctness.
*/
#include <cassert>
#include <chrono>
#include <functional>
#include <random>
#include <vector>
#include <string>

#include "tests.h"

#include <list>

#include "skip_list.h"
#include "sorted.h"
#include "sorted_container.h"


/*
 * calls Insert() with all elements in input on list
 */
template<typename T>
void insertList(const std::vector<T>& input, sorted<T>& list)
{
	assert(list.Size() == 0);
	for (const auto i : input) list.Insert(i);
}

/*
 * calls Insert() with all elements in input on list, adds modifier to each input
 */
template<typename T>
void insertList(const std::vector<T>& input, sorted<T>& list, T modifier)
{
	assert(list.Size() == 0);
	for (const auto i : input) list.Insert(i + modifier);
}


/*
 * calls Remove() with all elements in input on list
 */
template<typename T>
void removeList(const std::vector<T>& input, sorted<T>& list)
{
	for (const auto i : input) list.Remove(i);
}


/*
 * calls Contains() with all elements in input on list
 */
template<typename T>
void containsList(const std::vector<T>& input, sorted<T>& list)
{
	for (const auto i : input) list.Contains(i);
}


/*
 * returns true list is sorted in non-decreasing order
 */
template <typename T>
bool isSorted(const sorted<T>& list)
{
	auto vector = list.AsVector();
	for (unsigned i = 1; i < vector.size(); ++i)
		if (vector[i-1] > vector[i])
		{
			std::cout << "   Fail!" << std::endl;
			std::cout << "     " << list.GetName() << " order invalid!" << std::endl;
			return false;
		}
			
	return true;
}


/*
 * returns true if sorted lists a and b are equivalent
 */
template <typename T>
bool equal(const sorted<T>& x, const sorted<T>& y)
{
	auto a = x.AsVector();
	auto b = y.AsVector();
	
	if (a.size() != b.size()) return false;

	for (unsigned i = 0; i < a.size(); ++i)
		if (a[i] != b[i])
		{
			std::cout << "   Fail!" << std::endl;
			std::cout << "     " << x.GetName() << " and " << y.GetName() << " are not equivalent!" << std::endl;
			return false;
		}
			

	return true;
}

/*
 * returns true if sorted lists a, b, and c are equivalent
 */
template <typename T>
bool equal(const sorted<T>& x, const sorted<T>& y, const sorted<T>& z)
{
	auto a = x.AsVector();
	auto b = y.AsVector();
	auto c = z.AsVector();
	
	if (a.size() != b.size() || b.size() != c.size()) return false;

	for (unsigned i = 0; i < a.size(); ++i)
		if (a[i] != b[i] || b[i] != c[i])
			return !equal(x,y) || !equal(x,z);

	return true;
}

/*
 * returns true if all sorted lists are equivalent
 */
template <typename T>
bool equal(const std::vector<sorted<T>*>& lists)
{
	if (lists.empty()) return true;

	std::vector<std::vector<T>> vectors;
	vectors.reserve(lists.size());
	
	for (auto list : lists)
		vectors.push_back(list->AsVector());

	for (unsigned i = 0; i < vectors[0].size(); ++i)
		for (unsigned j = 1; j < vectors.size(); ++j)
			if (vectors[0][i] != vectors[j][i])
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
	struct results
	{
		results(const sorted<unsigned long long>* list)
		: list_name(list->GetName()), insert_time(0), remove_time(0), contains_time(0) {}
		std::string list_name;
		unsigned long long insert_time;
		unsigned long long remove_time;
		unsigned long long contains_time;
	};
	
	std::cout << "\n******************************************************************************************************"<< std::endl;
	std::cout << "\n Performance tests for Skip List vs Sorted Linked List\n" << std::endl;
	
	std::cout << "\n Enter n for performance tests (recommend 5000): ";
	long long n;
	getInput(n);

	std::cout << "\n      Enter repetitions per test (recommend 5): ";
	unsigned int repetitions;
	getInput(repetitions);
	
	std::vector<unsigned long long> input(n);
	for (int i = 0; i < n; ++i) input[i] = i;
	
	std::random_device rd;
	std::mt19937 g(rd());
	
	skip_list<unsigned long long> skipList;
	sorted_container<std::list<unsigned long long>> linked_list("sorted linked list");
	sorted_container<std::vector<unsigned long long>> vector_list("sorted vector list");

	const std::vector<sorted<unsigned long long>*> lists { &skipList, &linked_list, &vector_list };
	std::vector<results> results{ &skipList, &linked_list, &vector_list };
	
	std::cout << " -----------------------------------------------------------------------------------------------------"<< std::endl;
	std::cout << "\n Testing Insert() for skip list, sorted vector list, and sorted linked list by\n" <<
		" inserting " << n << " elements in random order" << std::endl;

	for (unsigned i = 0; i < lists.size(); ++i)
	{
		auto& list = lists[i];
		auto& result = results[i];
		
		result.insert_time = time(
		"\n  Testing Insert() for " + list->GetName(),
		[&]
		{
			list->Clear();
			std::shuffle(input.begin(), input.end(), g);
		},
		[&]()
		{
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
	std::cout << "\n Testing Remove() for skip list, sorted vector list, and sorted linked list list by\n" <<
		" calling Remove() on " << n << " elements in random order where 1/2 of calls are misses." << std::endl;

	const unsigned long long n_half = n>>1;

	for (unsigned i = 0; i < lists.size(); ++i)
	{
		auto& list = lists[i];
		auto& result = results[i];
		
		result.remove_time = time(
		"\n  Testing Remove() for " + list->GetName(),
		[&]
		{
			// refill list
			list->Clear();

			// make half miss in function with n_half modifier
			insertList(input, *list, n_half);

			// shuffle input
			std::shuffle(input.begin(), input.end(), g);
		},
		[&]()
		{
			// call remove on list with all input
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
	std::cout << "\n Testing Contains() for skip list, sorted vector list, and sorted linked list list by\n" <<
		" calling Contains() on " << n << " elements in random order where 1/2 of calls are misses." << std::endl;
	
	for (const auto list : lists)
	{
		list->Clear();
		
		// make half miss in function with n_half modifier
		insertList(input, *list, n_half);
	
		// shuffle input
		std::shuffle(input.begin(), input.end(), g);
	}
	
	for (unsigned i = 0; i < lists.size(); ++i)
	{
		auto& list = lists[i];
		auto& result = results[i];
		
		result.contains_time =  time(
			"\n  Testing Contains() for skip list",
			[&]
			{
				// shuffle input
				std::shuffle(input.begin(), input.end(), g);
			},
			[&]()
			{
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
	std::cout << "\n Performance results for " << n << " method calls for skip list, sorted linked list and sorted vector list:\n" << std::endl;
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
	
	skip_list<unsigned long long> skipList;
	sorted_container<std::vector<unsigned long long>> vector_list("sorted vector list");
	sorted_container<std::list<unsigned long long>> linked_list("sorted linked list");

	const std::vector<sorted<unsigned long long>*> lists { &skipList, &linked_list, &vector_list };
	
	std::cout << " - checking if skip list, sorted linked list, and sorted vector list remain sorted and equivalent\n   with correct size after Insert():";
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
	

	std::cout << " - checking if skip list, sorted linked list, and sorted vector list remain sorted and equivalent\n   with correct size after Remove() (no misses):";
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
	
	std::cout << " - checking if skip list, sorted linked list, and sorted vector list return the same response\n   for Contains() (50% misses):";
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

	
	std::cout << " - checking if skip list, sorted linked list, and sorted vector list remain sorted and equivalent\n   with correct size after Remove() (50% misses):";
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
void run_free_test()
{
	std::cout << "\n******************************************************************************************************"<< std::endl;
	std::cout << "\n Free Test for Skip List\n" << std::endl;
	
	std::cout << "\n Enter initial n for skip list (20 recommended): ";
	int n;
	getInput(n);
	
	skip_list<long long> skip;
	if (n > 0)
	{
		std::vector<long long> input;
		input.push_back(1);
		for (int i = 1; i < n; ++i) input.push_back(input.back() + 1 + 10 * static_cast<long long>(static_cast<float>(rand()))/RAND_MAX);

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(input.begin(), input.end(), g);
	
	
		for (const auto i : input)
		{
			std::cout << " Insert(" << i << ")" << std::endl;
			skip.Insert(i);
			skip.Print(true);
		}
	}
	else skip.Print();
	
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
		case 'r':
			{
				if (!getInput(i, "Invalid input", false)) continue;
				std::cout << "Remove(" << i << ") => ";
				std::cout << (skip.Remove(i) ? "True" : "False") << std::endl;
				skip.Print(true);
				break;
			}
		case 'c':
			{
				if (!getInput(i,  "Invalid input", false)) continue;
				std::cout << "Contains(" << i << ") => ";
				std::cout << (skip.Contains(i) ? "True" : "False") << std::endl;
				skip.Print(true);
				break;
			}
		case 'i':
			{
				if (!getInput(i, "Invalid input", false)) continue;
				std::cout << "Insert(" << i << ")" << std::endl;
				skip.Insert(i);
				skip.Print(true);
				break;
			}
		case 'x':
			{
				skip.Clear();
				skip.Print(true);
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



