#include <iostream>
#include "int_buffer.h"
#include "int_sorted.h"


// "buf" has the value of int_buffer(v1), where v1 is of type: int_buffer v1(10)
//Constructors that is being used before f: 
// 1: Först enparametriserad konstruktor för att skapa objekt v1.
// 2: Sedan copy-ctr för att kopiera objekt v1 till funktionen f som argument
// 3: Copy-ctr kommer att starta två-parametriserad ctr med sin initiering
// 4: Tvåparametriserad Ctr startar enparametriserad Ctr med sin initering.

//Constructors used within f: enparametriserad ctr för att skapa och deklarera objekt "buffer" som tar emot värden från 
// argumentet "buf". 
//när scope går ut så används destruktor för uppstädning.
//metoder som används för tilldelning av värden är icke-konst metoder 'begin()' och 'end()' då objektet inte är typen const.
//metoder som används för utskrift är const begin() const och const end() const, pga. jag skapade en
// const-referens till buffern som används för just utskriften.

void f(int_buffer buf) {

	printf("\n"); printf("\n"); printf("Function f starts here: "); printf("\n");

	int_buffer buffer(buf.size()); //buf.size = 10
	int j = 0; //value for loop

	//adds the element 1 - 10 to the buffer
	// i is a pointer that points through every index of the buffer, starting at the begining to the end.
	//for every pointed index, the value of that index will be set to j+1, which will be the values 1 to 10.
	for (int* i = buffer.begin(); i != buffer.end(); i++) {
		*i = j + 1;
		j++;

		//check that value of J is matching the value of index i:
		std::cout << "Value of J: " << j << ". " << "Value at index i: " << *i << std::endl;
	}

	//prints out all elements in buffer. Output is 1 - 10 
	std::cout << "testing ranged based forloop:";
	for (auto p : buffer) {
		std::cout << p << " ";
	}
	printf("\n");

	std::cout <<"temp begin:" << *buffer.begin() << "temp end: " << * (buffer.end() - 1); //output is 1 and 10

	//creates a const reference to the buffer, to make sure that the output loop uses the const begin()const version.
	//since when only accessing/getting, there is no need to be able to change the values.
	const int_buffer& buffer2 = buffer; 

	printf("For loop output buffer2: ");
	//prints all elements of the buffer. Output is 2,3,4,5,6,7,8,9 and -3019384. 
	for (const int* i = buffer2.begin(); i != buffer2.end(); i++) {
		std::cout << *i << " ";
	}
}

//obs, har just nu designen att buffer_end() pekar på ett element efter sista elementet.
int main(){
	
	//Testresurser att jobba med
	int N = 10; 
	int M = 20;
	int O = 30;

	//skapar en buffer som dynamiskt allokerar minne för en array med 5 heltal
	int arr[] = {1,2,3,4,5};
	int_buffer buff(arr, 5); //2-parametriserad construktor.

	int val = *(buff.end()-1); // pekar på sista tal i buffern
	std::cout << "val:" << val << std::endl; //testprint

	std::cout << "buff begin: " << *buff.begin() << std::endl;
	std::cout << "buff end: " << *(buff.end()-1) << std::endl << std::endl;

	//testutskrift -> de två lopparna nedan ger exakt samma resultat, på olika vis.
	for (auto temp : buff) {
		std::cout <<"Buff1: " << temp << std::endl;
	}
	std::cout << '\n';

	// använder pekare för iteration istället.
	for (int* i = buff.begin(); i < buff.end(); i++) {
		std::cout << "Element: " << *i; // dereference pekarens värde
	}
	std::cout << '\n';
	


	const int_buffer buff2 = buff; //testar copy constructor 
	std::cout << '\n' << '\n';

	for (auto temp : buff2) {
		std::cout << "buff 2: " << temp << std::endl; // funkar.
	}

	// testdata
	int c[] = { 3,4,5,6,7 };
	int d[] = { 4,5,6,7,8 };
	int e[] = { 5,6,7,8,9 };

	int_buffer buffC(c, 5);
	int_buffer buffD(d, 5);
	int_buffer buffE(e, 5);
	std::cout << '\n';
	std::cout << "buff C end: " << *(buffC.end()-1) << std::endl;

	std::cout << "BuffC: ";
	for (auto temp : buffC) {
		std::cout << temp;
	}
	std::cout << '\n' << '\n';

	//Copy assign test -> Works
	buffC = buffE; //kommer nu få elementen 5,6,7,8,9
	std::cout << '\n'; std::cout << '\n';

	std::cout << "buffC after Copy assign: ";
	for (auto temp : buffC) {
		std::cout  << temp;
	}
	std::cout << '\n' << '\n';



	//copy constructor test -> Works
	int_buffer buffF(buffC);
	std::cout << "copy done." << std::endl;
	std::cout << '\n' << '\n';

	std::cout << "buffF values after copy construct: ";
	for (auto temp : buffF) {
		std::cout << temp;
	}
	

	//test move constructor
	int_buffer moveBuff1(std::move(buffF)); //testar moveconstructor via std::move. std::move == (type&&) fast mer eleganat.
	
	//movebuff1 håller resurserna från buffF, fungerar korrekt.
	std::cout << std::endl;
	std::cout << "MOVEBUFF1: ";
	for (auto temp : moveBuff1) {
		std::cout << temp;
	}	

	std::cout << std::endl;
	std::cout << "buffF EFTER MOVE:  ";
	for (auto temp : buffF) { //denna varning makes sense då det nu är ett moved object
		std::cout << temp;
	}
	
	//konverterar moveBuff1 till en r-value referens. dvs objektet movebuff1 kan behandlas som ett r-value
	//och flytta sina resurer över till moveBuff2.
	int_buffer moveBuff2 ((int_buffer&&)moveBuff1); //flyttar moveCtr via casta obj1 till r-valuee. 
	std::cout << std::endl;


	//std::cout << "BUFF1 : " << *(buffF.begin()) << *(buffF.end()); //funkar inte -> returnerar nullptr, bra.

	std::cout << std::endl;
	//first we move value from buffF to moveBuff1, then from moveBuff1 to moveBuff2, moveBuff2 should be == 
	// to buffF: True.
	std::cout << "MOVEBUFF 2: ";
	for (auto temp : moveBuff2) {
		std::cout << temp;
	}
	printf("\n");
	

	//test move assign:
	//std::move gör en typomvandling av buffD, från ett l-value till ett r-value. en type-cast
	moveBuff2 = std::move(buffD);


	for (auto temp : moveBuff2) {
		std::cout << temp;
	}

	//testa indexpekare::
	std::cout << std::endl << "index operators: " << std::endl;
	std::cout << "index operator 1: " << moveBuff2[1] << std::endl;

	//indexpekare för att modifiera element i buffern:
	int arr4[] = { 1,2,3 };
	int_buffer arr_4(arr4, 3);
	for (auto temp : arr_4) {
		std::cout << "arr_4 values before index: " << temp;
	}
	printf("\n");

	arr_4[1] = 7;
	for (auto temp : arr_4) {
		std::cout <<"arr_4 after index operator insertion: " << temp;
	}

	//const index operator::

	const int_buffer mBuff3 = moveBuff2;
	std::cout << "index operator 2: " << mBuff3[1] << std::endl;

	int_buffer v1{10};
	f(v1);
	printf("\n"); printf("\n"); printf("\n");

	int arr5[] = { 2,1,4,3,5 };
	int_sorted s1(arr5,5);
	//s1.merge();
	int a = 6;
	std::cout << "insert: " << *s1.insert(a) << std::endl;

	for (auto temp : s1) {
		std::cout <<"s1 values: " << temp << std::endl;
	}

	std::cout << "s1 begin: " << *s1.begin() << " S1 end: " << *(s1.end()-1) << std::endl;

	int arr6[] = { 5,1,3,4,2 };
	int_sorted s2(arr6, 5);
	for (auto temp : s2) {
		std::cout << "s2 values: " << temp << std::endl;
	}
	std::cout << "s2 begin: " << *s2.begin() << " S2 end: " << *(s2.end() - 1) << std::endl;
	

	s1.is_sorted(s1.begin(), s1.end());
	s2.is_sorted(s2.begin(), s2.end());

	//trial of merge:
	int_sorted s3 = s1.merge(s2); //merges s1 + s2 in sorted order.

	std::cout << "merge s1 + s2: ";
	for (auto temp : s3) {
		std::cout << temp;
	}
	printf("\n");


	//Uppgift 3 : implementera selection sort + testa:: 
	
	//creates random array of 40000 elements
	int const sz = 40000;
	int random[sz];

	for (int i = 0; i < sz; i++) {
		random[i] = (rand() % 40000); //generates number between 0 and 40000
	}

	//for (int i = 0; i < sz; i++) {
		//std::cout << "element at index [" << i << "] " << random[i] <<std::endl;
	//}

	std::cout << "s5 values: " << std::endl;
	int_sorted s5(random, 40000);
	for (auto temp : s5) {
		std::cout << temp << std::endl;
	}

	system("pause");

	const int K = 10;
	const int k = 5;

	std::cin.get();

	return 0;
}