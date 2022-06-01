/*
 * Skip List implementation for University of Alberta CMPUT 403 final project Winter 2022.
 * 
 * Based on description from https://en.wikipedia.org/wiki/Skip_list.
 * A skip list is an ordered sequence data structure with O(logn) running time for Insert(), Remove(), and Contains().
 * Advantages are fast searching like an ordered array, combined with fast insertion/deletion like a linked list.
 *
 * Should work with any type T that defines <,>, ==, !=, and operator--().
 * 
 * Author: Mike Greber
 */

#pragma once

#include <vector>
#include "skip_list.h"
#include "sorted_list.h"


/*
 * Skip list wrapped class to implement sorted_list interface for testing.
 */
template <typename T>
class skip_list_test : public skip_list<T>, public sorted_list<T> 
{
public:
	// Constructor
	skip_list_test(float p = 0.5) : skip_list<T>(p) {}

	// sorted_list interface begin
	std::string GetName() const override { return "skip list"; }
	void Insert(T val) override { skip_list<T>::Insert(val); }
	bool Remove(T val) override { return skip_list<T>::Remove(val); }
	bool Contains(T val) override { return skip_list<T>::Contains(val); }
	void Clear() override { skip_list<T>::Clear(); }
	size_t Size() const override { return skip_list<T>::Size(); }
	void Fill(T min, T max) override { Clear(); for (T i = max; min < i; --i) Insert(i); }
	
	std::vector<T> AsVector() const override
	{
		std::vector<T> v;
		v.reserve(Size());
		for (auto& i : *this) v.push_back(i);
		return v;
	}
	// sorted_list interface end
};
