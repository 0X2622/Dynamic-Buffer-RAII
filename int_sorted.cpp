#include "int_sorted.h";
#include <iostream>
#include <algorithm>
#include <chrono>
#include <functional>


// explicitly default constructor - initiates and creates a buffer with size 1
// reduce the chance of uncessecary overhead allocation
// flexibility - easy to create a larger buffer if needed, but if a small one is needed then the size of 1 exists.
int_sorted::int_sorted() :buffer(1)
{
}

//initiates an int_sorted objects that takes two arguments and initiates the 2-parameter CTR for int_buffer. 
//if more than 1 element inside the buffer -> sort function will be called and sort the buffer in ascending order
//The sort function takes in two pointers as arguments that points to the beginning and the end of the buffer.
//the *this keyword is used to make sure that the sorted range of value and output is being passed to this current object, 
//we are in fact referencing to this current object by using *this keyword, Without the *this keyword the buffer inside sort() 
//would still be sorted, but that would only be a temporary buffer and not the one that we are referencing to.
int_sorted::int_sorted(const int* source, size_t size) :buffer(source, size){
	
	if (size > 1) {
		//this approach means that a buffer can only be sorted once during its lifetime.
		std::cout << '\n' << "Select: 0 for mergeSort " << '\n' << "Select: 1 for selectSort." << '\n' 
		<< "Select 2 for std::sort: "<< '\n' << "Select: Any other key for no sort" << std::endl;
		std::cin >> this->sortMethod; //alows user to select sort method.
		std::string funcType; //declares the string that will hold the name of the sort type.
		switch (this->sortMethod) {
		case 0: {	


			// Kladd nedan att bearbera och gå igenom.

			//*this = sort(buffer.begin(), buffer.end()); //gives the total range of the elements thaat are to be sorted. beginning and end of buffer.		
			
			//This defines a lambda/anonymous function, 
			// which is a function object - object that behaves like a function and that can be passed into function,
			// this function object allows specific instances/objects of the int_sorted class to be called like functions,
			
			// So, this code creates a variable "m_sort" that holds the function object that
			// can be passed as a parameter into the 'measureSortTime' function, this way we can use the member functions
			//sort and selectionSort inside 'measureSortTime' to calculate the duration of each sort method.

			//The lambda expression works by first capturing the 'this' pointer by value - which means that we are creating
			//a copy of the 'this' pointer that points to this current object. By capturing the 'this' pointer we are
			// binding the lambda function to this current instance (object) of the class, and thus can access all of it's 
			// data (members and variables). Because of this we don't need to specify and parameters and thus can leave the
			// second brackets '()' empty. 
			// 
			// After the empty brackets we have '-> int_sorted', which indicates that this lambda function
			// return a type of int_sorted. A lambda function does return "void" by default. And we need it to return
			// a type of int_sorted, because inside 'measuresortTime' we assign '*this' (this current instance)
			// to the return value of the lambda function (*this = sortFunc), then we return this object from that function,
			// so we need the lambda function that is called inside measureSortTime to return the type of int_sorted.
			// 
			// inside the {} brackets we have the body of the function and it's logical operations:
			// The lambda function creates an int_sorted object 'obj', and it's assigned values is the result of the 
			// member function sort() from the buffer of this current instance. When the operation is done, the lambda
			// function returns the new sorted buffer that is assigned to 'obj', and assigns it to *this inside 'measureSortTime',
			//  which assigns the sorted object to the current instaance of the object, and then the function returns the
			//  object (this) with the new sorted values.
			// 
			// Additionally worth noting:
			// the function uses the captures 'this-pointer' that points to this current instance, to access the chosen
			// sort-method, in this case 'sort' and 'selectionSort', and also accessing it's necessary parameters when needed.
			// this lambda function becomes a function object ( an object that behaves like a function ), that consists of 
			// the sort methods of this class, that is then stores inside the m_sort/s_sort variables and then passed into 
			// 'measuredsortTime'
			
			
			auto m_Sort = [this]() -> int_sorted { int_sorted obj = this->sort(buffer.begin(), buffer.end()); 
			return obj;  };
			funcType = "merge Sort";
			measureSortTime(m_Sort, funcType);		

			//measureSortTime(*this, this->buffer.begin(), this->buffer.end(), funcType); functoin oveerload
			
			break; }
		case 1: {
			//this->selectionSort();
			
			//since the selenctionSort algorithm sorts the buffer direct in place, it doesn't return a new sorted
			// buffer, thus there is no need to create a new int_sorted object. capture the *this pointer
			//by value that will point to the current instance of the buffer and then we use the pointer to
			//access the objects sort function, and then we return the object.
			funcType = "Selection Sort";
			auto s_Sort = [this]() -> int_sorted& {this->selectionSort(); return *this; };
			measureSortTime(s_Sort, funcType);
			
			
			//measureSortTime(*this, funcType); function overload
			break; }
		case 2: {

			//this case uses the built in std::sort algorithm. The std::sort algorithm takes 2 pointers
			//that points to the begining and the end of the buffer as parameters. 
			//this sort algorithm also sorts the buffer in place and thus does not need to create a new object
			// to hold the sorted values.
			funcType = "std::sort";
			auto stdSort = [this]() -> int_sorted& {std::sort(buffer.begin(), buffer.end()); return *this; };
			measureSortTime(stdSort, funcType);
		}
		default:
			std::cout << "No sort method chosen." << std::endl;
		}		
	}
}

//returns interal staate of sz member which is size of buffer. sz is always <= capacity.
size_t int_sorted::size() const
{
	return buffer.size();
}

// inserts an element to the buffer and returns the insertion point
int* int_sorted::insert(int value)
{
	//creates a temporary int_sorted object that contains one value, and a reference to that value to avoid copy.
	//calls the merge function off the current object to merge the values of the two buffers from the int_sorted objects 
	// the result of the merge will be a new buffer with the merged values. That buffer is accessed with the .Dot operator
	// the buffer in our current object (LFH) is assigned the values of the new merged buffer
	// valPtr is an integer pointer that will be pointing to the value "value" inside the buffer, using the std::find() function.
	//the valPtr is then returned to the user.
	buffer = merge(int_sorted(&value, 1)).buffer;
	std::cout << "insertion succcessfull" << std::endl;
	int* valPtr = std::find(buffer.begin(), buffer.end(), value);
	return valPtr;
}

const int* int_sorted::begin() const
{
	return buffer.begin();
}

const int* int_sorted::end() const
{
	return buffer.end(); //returnera början + size så får man slutet av buffern.
}

int_sorted int_sorted::sort(const int* begin, const int* end)
{
	if (begin == end) return int_sorted(); //if the frontpointer == the backpointer of the list == 1 element inside
	if (begin == end - 1) return int_sorted(begin, 1); //if begin and end are adjacent pointers and they are already sorted
	ptrdiff_t half = (end - begin) / 2; //half == difference between end & begin, / 2. ptrdiff_t stores the difference between the pointers
	const int* mid = begin + half; //mid is a pointer that points to the midpoint between end and begin.
	
	//this returnstatement sorts the divided object in 2 parts, from begin to mid and from mid to end
	//and then it merges these two sorted objects into a whole and returns the whole buffer.
	//Important note is that this statement calls the sort function recursively, which means that this function
	//will keep calling itself and keep dividing each subrange until each subrange only consists of 1 or 2 elements,
	//and then it will start to merge the subranges together in a "top down" approach until we have constructed
	// a complete int_sorted object with the original range in a sorted order, and then returns it.
	return sort(begin, mid).merge(sort(mid, end)); 
}

//in place-sort algorithm. Operates directly on the buffer that this object is holding
//thus it does not need to allocate any new memory or create any new buffer, and thus "void" is sufficent
// since this method is a member of the int_sorted class, it has direct access to and operates directly
// on the private datamembers and the buffer that is to be sorted, thus no arguments is needed.  
void int_sorted::selectionSort()
{
	//int_buffer sorted = int_buffer(this->size());

	int* current = this->buffer.begin(); //points towards the beginning of the buffer. 
	int* next = this->buffer.begin() + 1; // will point to one element away from current
	int* end = this->buffer.end(); //points towards the end of the buffer

	//this loop will iterate until the pointer current is pointing towards the second to last element of the buffer.
	//for each iteration current will point towards the element that is being examined and compared.
	for (current; current < end - 1; current++) {
		int* minIndex = current; //sets the minInex pointer to the current smallest value of the buffer
		next = current + 1; //makes sure that the next pointer is always one step away from current.
		
		//this loop will iterate through the unsorted part of the buffer and keep looking for an element < minIndex
		for (next; next < end; next++) {
			if (*next < *minIndex) { 
				minIndex = next; //this keeps incrementing minIndex if a smaller value is found inside the buffer.
			}
		}
		//if the value of minIndex != i means that a smaller value has been found.
		if (minIndex != current) {
			//since minIndex is currently pointing to the smallest value, we swap that value with i to make sure that the
			//array gets sorted in ascending order. This step is done every time that minIndex finds a value smaller than i
			std::swap(*current, *minIndex);
		}
	}
}

int_sorted int_sorted::merge(const int_sorted& merge_with) const
{
	
	const int* thisPtr = this->begin(); //a is a pointer that points to the beginning of this object
	const int* mergePtr = merge_with.begin(); //b is a pointer that points to the beginning of the passed in object merge_with
	const int* thisEnd = this->end();
	const int* mergeEnd = merge_with.end();

	int_buffer C = int_buffer((*this).size() + merge_with.size()); //buffer c has the capacity of a + b. gets size of a and b.
	int* cPtr = C.begin(); //pointer to beginning of C

	//iterates as long as the pointer has not reached the end of the buffer (for both buffers)
	while (thisPtr != thisEnd && mergePtr != mergeEnd) {
	
		//if the element at this < merge. Put insert the element into C. makes sure that the lowest elements are first
		if (*thisPtr < *mergePtr) {
			*cPtr = *thisPtr;
			thisPtr++;
		}
		else {
			*cPtr = *mergePtr;
			mergePtr++;
		}
		cPtr++; //increment cPtr to the next index
	}

	//makes sure to write over remaining elements
	while (thisPtr != thisEnd) {
		*cPtr = *thisPtr;
		thisPtr++;
		cPtr++;
	}
	while (mergePtr != mergeEnd) {
		*cPtr = *mergePtr;
		mergePtr++;
		cPtr++;
	}

	
	int_sorted merged = int_sorted(); //creates a sorted object that holds a buffer with 0 elements
	merged.buffer = std::move(C); //moves all elements from buffer C to the buffer inside merged by using std::move
	//std::cout << "merge successfull!" << std::endl;
	//std::cout << "size of merge: " << merged.size() << std::endl;
	return merged; //returns the sorted object
}

//this function returns true if a buffer is sorted and false if it's not sorted in ascending order.
bool int_sorted::is_sorted(const int* begin, const int* end )
{
	const int* thisPtr = begin; // points to the beginning and current index of this buffer
	const int* thisEnd = end; //points to the end of this buffer
	
	//while A.hasNext() this function compares the current element to the next element and checks if the current element is
	//bigger than the next, if true then the object is not sorted. If false for all elements then it means that the object is sorted.
	while (thisPtr != thisEnd - 1) { //iterates to the second to last element, make sure that nextPtr is always within the buffer range.
		const int* nextPtr = thisPtr+1; //nextPtr points to the next element in the buffer
		if (*thisPtr > *nextPtr) {
			printf("buffer not sorted!");
			return false; //means that the next index is smaller and thus not sorted.
		}
		thisPtr = nextPtr; //iterates thisPtr to the next index/element in the buffer
	}
	printf("buffer sorted!");
	return true;
}

// Generic function that takes a callable object as argument, the reason to make it generic is because 
// "sort" and selectionSort does not take in same types as arguments. 

template<typename SortFunc>
int_sorted& int_sorted::measureSortTime(SortFunc& sortFunc, const std::string& funcType) {
	auto start = std::chrono::high_resolution_clock::now(); //gives us the current start time.
	*this = sortFunc(); //sortFunc is the lamba function that is being called, and the return value -> *this
	auto end = std::chrono::high_resolution_clock::now(); //gives us the current end time
	std::chrono::duration<double> totalDuration = end - start; //gets the total duration (end - start). 
	std::cout << "The sort method '" << funcType << "' took: " << totalDuration.count() << " seconds.\n"; //outputs the duration
	return *this;
}


//below is implementation for function overloading instead of template.

/*
int_sorted int_sorted::measureSortTime(int_sorted &obj, const int* begin, const int* endd, const std::string& funcType) {
	auto start = std::chrono::high_resolution_clock::now(); //gives us the current start time.
	*this = obj.sort(begin,endd);
	auto end = std::chrono::high_resolution_clock::now(); //gives us the current end time
	std::chrono::duration<double> totalDuration = end - start; //gets the total duration (end - start). 
	std::cout << "The sort method '" << funcType << "' took: " << totalDuration.count() << " seconds.\n"; //outputs the duration
	return *this;
}

int_sorted int_sorted::measureSortTime(int_sorted& obj, const std::string& funcType) {
	auto start = std::chrono::high_resolution_clock::now(); //gives us the current start time.
	obj.selectionSort();
	auto end = std::chrono::high_resolution_clock::now(); //gives us the current end time
	std::chrono::duration<double> totalDuration = end - start; //gets the total duration (end - start). 
	std::cout << "The sort method '" << funcType << "' took: " << totalDuration.count() << " seconds.\n"; //outputs the duration
	return *this;
}*/

