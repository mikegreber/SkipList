/*
*
 * Template for sorted linked list for comparison with skip list.
 * Uses std::list<T>.
 * Insert(), Remove(), and Contains() use STL find algorithms.
 *
 * Should work with any container element type that defines <,>, ==, !=, and operator++().
 *
 * Author: Mike Greber
 */

#pragma once

#include "sorted_list.h"

template <typename T>
class sorted_linked_list final : public sorted_list<T>
{
public:
	// return name of the sorted container
	std::string GetName() const override { return "sorted linked list"; }

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

	// clear and fill the container with sequential elements from min to max
	void Fill(T min, T max) override;

	// return list as a vector
	std::vector<T> AsVector() const override;
	
	// prints the contents of container
	void Print();

private:
	std::list<T> list_;
};

/*
 * Returns true if the container contains val
 */
template <typename T>
bool sorted_linked_list<T>::Contains(T val)
{
	return std::find(list_.begin(), list_.end(), val) != list_.end();
}

/*
 * Inserts val into the container in its sorted position
 */
template <typename T>
void sorted_linked_list<T>::Insert(T val)
{
	auto it =
		std::find_if(list_.begin(), list_.end(),
		[&](const T& a) { return a >= val; });

	list_.insert(it, val);
}

/*
 * Removes the first instance of val from the container.
 * Returns false if val is not in the container.
 */
template <typename T>
bool sorted_linked_list<T>::Remove(T val)
{
	auto it = std::find(list_.begin(), list_.end(), val);
	if (it != list_.end())
	{
		list_.erase(it);
		return true;
	}
	return false;
}

/*
 * Clears all elements from the container.
 */
template <typename T>
void sorted_linked_list<T>::Clear()
{
	list_.clear();
}

/*
 * Returns the number of elements in the container.
 */
template <typename T>
size_t sorted_linked_list<T>::Size() const
{
	return list_.size();
}

/*
 * clear and fill the container with sequential elements from min to max
 */
template <typename T>
void sorted_linked_list<T>::Fill(T min, T max) 
{
	Clear();
	for (T i = min; i <= max; ++i) list_.push_back(i);
}

/*
 * Returns container values as a vector.
 */
template <typename T>
std::vector<T> sorted_linked_list<T>::AsVector() const
{
	std::vector<T> vector;
	vector.reserve(list_.size());
	for (auto i : list_) vector.push_back(i);
	return vector;
}

/*
 * Prints the contents the container.
 */
template <typename T>
void sorted_linked_list<T>::Print()
{
	for (auto i : list_) std::cout << i << " ";
	std::cout << std::endl;
}


