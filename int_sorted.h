#ifndef INT_SORTED_H
#define INT_SORTED_H

#include "int_buffer.h"
#include <string>

//class
class int_sorted {


public: 
	
	int_sorted(); //default constructor (xtra tillägg - kanske inte behövs).
	int_sorted(const int* source, size_t size); //startar tvåparametriserad buffer-ctr och sorterar den. ?? 
	size_t size() const; //member that asks for sz (size) which is internal state of the private sz member 
	//size_t capacity() const; //member that asks for buffer capacity.(extra tillägg - kanske inte behövs).
	int* insert(int value); // inserts an element in the buffer/container
	const int* begin() const; //return buffeer här?
	const int* end() const; 
	int_sorted sort(const int* begin, const int* end); //sort-function. Sorts a buffer.
	void selectionSort();
	int_sorted merge(const int_sorted& merge_with) const;
	bool is_sorted(const int* begin, const int* end);

	template<typename SortFunc>
	int_sorted& measureSortTime(SortFunc& sortFunc, const std::string& funcType);

	//int_sorted measureSortTime(int_sorted &obj, const int* begin, const int* end, const std::string& funcType);
	//int_sorted measureSortTime(int_sorted &obj, const std::string& funcType);

private:
	int_buffer buffer; //handles rescource allocations.
	int sortMethod = -1;
};
#endif
