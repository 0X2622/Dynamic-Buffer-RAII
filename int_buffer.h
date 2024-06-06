#ifndef INT_BUFFER_H
#define INT_BUFFER_H

#include "cstdlib"; //kolla varför detta behövs


class int_buffer {
public:

	//static const int MIN_CAP = 16;
	//int_buffer(); //default

	//Konstruerare med 1 paremeeter = typomvandlingskonstruerare. 
	//Då alla typeer kan bli konverterade till typen size_T så sätts explicit framför ->
	//så att implicit typomvandling inte tillåts.
	//denna konstruktor skapar en buffeer med en speciel storlek (storlek size).
	explicit int_buffer(size_t size); // size_t is defined in cstdlib
	
	//Standard-construktor. -> blir en kopia av en befintlig int-tals array
	//tar emot en uppsättning pekare (src) + ett antal (size).
	//denna är inte en typomandlingskonstruerare -> en konstruerare som tar emot två argument
	//så måste man uttrycka sin initiering av int_buffer för att aktiveras. Så denna behöver inte vara märkt som 
	// explicit.
	int_buffer(const int* source, size_t size); 
	int_buffer(const int_buffer& rhs); // copy construct 1 
	int_buffer(int_buffer&& rhs) noexcept; // move construct 2. Kanske inte behöver noexcept?

	int_buffer& operator =(const int_buffer& rhs); // copy assign -> skapar möjlighet till kedjekopiering. A=B=C 4 
	int_buffer& operator =(int_buffer&& rhs) noexcept; // move assign 5
	size_t size() const; //used for constructor that returns the size of the buffer

	//int_buffer& operator=(int_buffer copied_or_moved); detta gör samma sak som copy + move assign

	//shows instances of buffer start and buffer end. can alter values
	int* begin(); 
	int* end(); 

	//instances of the buffer -> const after parameters ensures that the functions body won't be able to alter the objects values
	//Const before the variable ensures that the values of the adresses thaata the pointers is pointing to is not modifiable either.
	//this ensures that the buffer has the same state before and after use of function.
	//to be abale to use these const methods, the object has to be declared const itself. e.g. " const int_buffer bla; "
	const int* begin() const;
	const int* end() const;

	//index operator -> takes an index within the brackets -> size_t is used because we don't want negative values.
	//it takes element of ints and is allowed to change values, which is why reference to the adress is used.
	int& operator [](size_t index);
	const int& operator [](size_t index) const; //this index operatos has no allowaance to change any values.
	~int_buffer(); //3

private:

	void swap(int_buffer& rhs);

	//pekare som håller reda på resurser och även pekar ut head + tail
	int* headPtr;
	int* tailPtr; 
};

#endif // !BUFFER_H;
