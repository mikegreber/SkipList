/*
*
 * Template for sorted STL container for comparison with skip list. Should work with any iterable list type STL container.
 * Insert(), Remove(), and Contains() use standard library algorithms.
 *
 * Should work with any container element type that defines <,>, ==, and !=.
 *
 * Author: Mike Greber
 */

#pragma once
#include <iostream>
#include <ostream>

#include "sorted.h"

template <class Container>
class sorted_container final : public sorted<typename Container::value_type>
{
	typedef typename Container::value_type T;
	
public:

	// Constructor
	explicit sorted_container(std::string name) : name_(std::move(name)) {}

	// return name of the sorted container
	std::string GetName() const override { return name_; }

	// insert val into its sorted position in the list
	void Insert(T val) override;

	// remove val from list, returns false if val not in list
	bool Remove(T val) override;

	// returns true if list contains val
	bool Contains(T val) override;

	// remove all elements from the list
	void Clear() override;

	// returns the number of elements in the list
	size_t Size() const override;

	// return list as a vector
	std::vector<T> AsVector() const override;
	
	// prints the contents of container
	void Print();

private:
	std::string name_;
	Container container_;
};

/*
 * Returns true if the container contains val
 */
template <class Container>
bool sorted_container<Container>::Contains(T val)
{
	return std::find(container_.begin(), container_.end(), val) != container_.end();
}

/*
 * Inserts val into the container in its sorted position
 */
template <class Container>
void sorted_container<Container>::Insert(T val)
{
	auto it =
		std::find_if(container_.begin(), container_.end(),
		[&](const T& a) { return a >= val; });

	container_.insert(it, val);
}

/*
 * Removes the first instance of val from the container.
 * Returns false if val is not in the container.
 */
template <class Container>
bool sorted_container<Container>::Remove(T val)
{
	auto it = std::find(container_.begin(), container_.end(), val);
	if (it != container_.end())
	{
		container_.erase(it);
		return true;
	}
	return false;
}

/*
 * Clears all elements from the container.
 */
template <class Container>
void sorted_container<Container>::Clear()
{
	container_.clear();
}

/*
 * Returns the number of elements in the container.
 */
template <class Container>
size_t sorted_container<Container>::Size() const
{
	return container_.size();
}

/*
 * Returns container values as a vector.
 */
template <class Container>
std::vector<typename sorted_container<Container>::T> sorted_container<Container>::AsVector() const
{
	
#if __cplusplus >= 201703L // constexpr needs at least C++17
	if constexpr (std::is_same_v<Container, std::vector<T>>) return container_;
#endif
	
	std::vector<T> vector;
	vector.reserve(container_.size());
	for (auto i : container_) vector.push_back(i);
	return vector;
}

/*
 * Prints the contents the container.
 */
template <class Container>
void sorted_container<Container>::Print()
{
	for (auto i : container_) std::cout << i << " ";
	std::cout << std::endl;
}


