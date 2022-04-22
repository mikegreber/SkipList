/*
*
 * Template for sorted vector for comparison with skip list.
 * Uses std::vector<T>.
 * Insert(), Remove(), and Contains() use STL Binary Search algorithms.
 *
 * Should work with any container element type that defines <,>, ==, !=, and operator++().
 *
 * Author: Mike Greber
 */

#pragma once

#include "sorted_list.h"

template <typename T>
class sorted_vector final : public sorted_list<T>
{
public:
	// return name of the sorted container
	std::string GetName() const override { return "sorted vector list"; }

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
	std::vector<T> vector_;
};

/*
 * Returns true if the container contains val
 */
template <typename T>
bool sorted_vector<T>::Contains(T val)
{
	return std::binary_search(vector_.begin(), vector_.end(), val);
}

/*
 * Inserts val into the container in its sorted position
 */
template <typename T>
void sorted_vector<T>::Insert(T val)
{
	vector_.insert(std::lower_bound(vector_.begin(), vector_.end(), val), val);
}

/*
 * Removes the first instance of val from the container.
 * Returns false if val is not in the container.
 */
template <typename T>
bool sorted_vector<T>::Remove(T val)
{
	auto it = std::lower_bound(vector_.begin(), vector_.end(), val);
	if (it != vector_.end() && *it == val)
	{
		vector_.erase(it);
		return true;
	}
	return false;
}

/*
 * Clears all elements from the container.
 */
template <typename T>
void sorted_vector<T>::Clear()
{
	vector_.clear();
}

/*
 * Returns the number of elements in the container.
 */
template <typename T>
size_t sorted_vector<T>::Size() const
{
	return vector_.size();
}

/*
 * clear and fill the container with sequential elements from min to max
 */
template <typename T>
void sorted_vector<T>::Fill(T min, T max)
{
	Clear();
	for (T i = min; i <= max; ++i) vector_.push_back(i);
}

/*
 * Returns container values as a vector.
 */
template <typename T>
std::vector<T> sorted_vector<T>::AsVector() const
{
	return vector_;
}

/*
 * Prints the contents the container.
 */
template <typename T>
void sorted_vector<T>::Print()
{
	for (auto i : vector_) std::cout << i << " ";
	std::cout << std::endl;
}


