﻿#pragma once
#include <vector>

/*
 * Interface for sorted list implementations for performance comparison.
 */
template <typename T>
class sorted_list
{
public:
	// return name of the sorted container
	virtual std::string GetName() const = 0;
	
	// insert val into its sorted position in the list
	virtual void Insert(T val) = 0;

	// remove val from list, returns false if val not in list
	virtual bool Remove(T val) = 0;

	// returns true if list contains val
	virtual bool Contains(T val) = 0;

	// remove all elements from the list
	virtual void Clear() = 0;

	// returns the number of elements in the list
	virtual size_t Size() const = 0;

    // clear and fill the container with sequential elements from min to max
	virtual void Fill(T min, T max) = 0;
	
	// return list as a vector
	virtual std::vector<T> AsVector() const = 0;

	virtual ~sorted_list() = default;

	// less than or equal comparison using only < operator
	inline static bool Less_Or_Equal(T a, T b){ return !(b < a); }

	// greater than or equal comparison using only < operator
	inline static bool Greater_Or_Equal(T a, T b){ return !(a < b); }

	// equality comparison using only < operator
	inline static bool Equal(T a, T b) { return !(a < b || b < a); }
};
