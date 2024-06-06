#include "int_buffer.h"
#include <iostream>
//#include <algorithm> // tror jag kan ta bort denna.

// Constructor that creates a buffer by dynamically allocating memory & assigning our pointers to the memory
// The constructor is marked as explicit, and can only be explicitly called and not implicit convert data types.
// by defining members with initializer list, you avoid uneccessary copy, overhead, improve performance and
// ensures a strong exception guarantee by avoiding memory leaks -> If memory allocation is not possible the 
// object will not be created, if headPtr is not pointing towards a valid memory tailPtr will not be created.

//if memory allocation fails, then the headPtr won't be pointing to the memory, and there will be no memory 
// leak, and the tailPtr won't be allocated as well, since it's dependent on the existence of the headPtr,
// thus, if memory allocation fails, the program ends up in a valid and safe state, as if the operation never
// occured, which leaves a strong exception guarantee.

// OBS: 
//This will result in a deep copy (and now shallow) when used with Copy CTR because the headPtr of this object 
// will point to a new address of an arrays of ints, and not to the same address as the object that we want
// copies from. 
int_buffer::int_buffer(size_t size) :
	headPtr(new int[size]), tailPtr(headPtr + size) 
{
}


//överväg o markera 2-parametriserad ctr som const så att resurserna inte kan fifflas med

// 2-parameter constructor that creates a buffer with the size and elements of an already existing int-array.
// 'source' is a pointer that points to the beginning of an array which we want to copy to our own buffer.
// 'size' has the size of the array.
// The buffer is created by first allocating the size of our own buffer and assigning it's pointers, 
// when the rescources has been allocated, we copy all elements from the existing int-array to our
// own buffer by using std::copy. When Copying is done we have created the buffer with it's elements.
int_buffer::int_buffer(const int* source, size_t size):

	// to allocate the correct size & rescources for our own buffer we use our 1-parameter constructor, 
	// by resuing our ctr we keep the code DRY and also ensures the exception guarantees provided by the first ctr.
	int_buffer(size) 
{

	// when we have allocated our rescources and set our pointers, we use std::copy to copy all elements from
	// the external array to our own buffer. 'source' points to the beginning of the array. We copy all elements
	// from the range source - (source + size) to headPtr, which points to the beginning of our own buffer. 
	// headPtr will be a valid pointer because of how it's formulated in our 1-parameter constructor
	std::copy(source, source + size, headPtr);
}


// Creates a new buffer that is a copy of an existing buffer 'rhs',
// the copying is done by calling the 2-parameter constructor with rhs. headPtr & rhs.size as arguments,
// which is the pointer to the beginning of the buffer that we want to copy, and the size of that buffer so
// that we can allocate the right amount of memory for our own buffer. rhs.size() calls a function that 
// returns the size of the buffer.
// basically, it is the same operation as calling the 2-parameter constructor, but instead of doing the copy
// of an existing int-array, we do the copy of an existing int_buffer. By re-using our constructors we keep our
// code DRY.
int_buffer::int_buffer(const int_buffer& rhs):
	int_buffer(rhs.headPtr, rhs.size()){}


// Constructs a new object by taking over the resources from an existing object instead of copying.
// The takeover is achieved by swapping the pointers of the 'move from' (rhs) and 'move to' (this) object.
// First, the pointers of this object is set to nullPtr, to ensure that rhs.pointer will be set to nullPtr.
// By setting the pointers of rhs to nullptr, ownership of the old object is released,
// ensuring the pointers cannot be used to access the data, and leaving rhs in a safe valid state.
// The constructor is marked as 'noexcept' to ensure no exceptions during moves, saving potential overhead.
int_buffer::int_buffer(int_buffer&& rhs) noexcept : headPtr(nullptr), tailPtr(nullptr) { 
	swap(rhs);
}


// assigns values/rescources of an existing buffer (rhs) to our own buffer
// the argument is an existing buffer named rhs which has the rescources that we want to copy to our own buffer
// a guard check makes sure that if this and rhs share the same adress and rescources, no operation is needed.
// if this != rhs, we call copy-ctr and create a new temporary object that takes the values of rhs,
// and then swaps the pointers of this with temp, exchanging ownership and rescources.
// the reason that we can't pass rhs into the swap function directly is because it's marked as 'const', and thus
// we need to create a temporary modifiable object first. 
int_buffer& int_buffer::operator=(const int_buffer& rhs)
{
	if (this != &rhs) {
		int_buffer tempObject = rhs; //calls the copy constructor to create a new temporary object

		// swaps the ownership (pointers) of the temporary object and *this. 
		// the destructor of temp will clean up rescources and prevent memory leaks
		swap(tempObject);
	}
	return *this;
}


// moves new rescources into this buffer instead of copying it.
// the argument rhs is passed in as r-value reference, which means that the rescources is a temporary value.
// the move is done by first setting the pointers of this to nullptr, and then swapping it with pointers of rhs.
// the swap operation will result in swap of rescources -> this object will have ownership of rhs and rhs
// will be set to nullptr. It's necessary that rhs points to nullptr to prevent dangling pointers
// function is marked as noexcept to tell the compiler that the rescources can be moved safely (and not copied).
int_buffer& int_buffer::operator=(int_buffer&& rhs) noexcept
{
	if (this != &rhs) {
		this->headPtr = nullptr; // sets the pointers to nullptr before the swap so rhs will be in a safe 
		this->tailPtr = nullptr; // state after the operation, preventing dangling pointers.
		swap(rhs);
	}
	return *this; 
}

//returns amount of elements insie the buffer (amount of elements between begin and end).
size_t int_buffer::size() const
{
	//return end() - begin(); ska se om ja kan få detta o funka sen. köra me angivna gränssnittet.
	return tailPtr - headPtr; //detta visar hur många element/ints som finns mellan buffer och buffer_end.
}

// returns a pointer to the beginning of the buffer. This can be used to modify values
int* int_buffer::begin()
{
	return headPtr; 
}

//returns pointer to the end of the buffer.  This can be used to modify values
int* int_buffer::end()
{
	return tailPtr; 
}

// same as above, but this function is only used for access (getter), and cannot modify value
const int* int_buffer::begin() const
{
	return headPtr;
}

const int* int_buffer::end() const
{
	return tailPtr;
}

//swaps the rescources of rhs and this buffer, this is done by swapping the pointers of the two objects.
// the argument is a referens to another object that is to be swapped with. The swap is done by calling std::swap
void int_buffer::swap(int_buffer& rhs){
	std::swap(this->headPtr, rhs.headPtr);
	std::swap(this->tailPtr, rhs.tailPtr);
}

//if i understaand this correctly - you ask for an index for the buffer and it returns the value of that index, you can also
//use this to change the index. 
int& int_buffer::operator[](size_t index)
{
	//printf("non const indexopevator ");
	return headPtr[index];
}

//same as above but not modifiable
const int& int_buffer::operator[](size_t index) const
{
	printf("const indexoperator ");
	return headPtr[index];
}


int_buffer::~int_buffer(){
	delete[] headPtr;
}
