## C-C++ Farklılıkları

C++11 (Modern C++)
C++14
C++17
C++20
C++23


C++ multi-paradigm bir dildir. 

	* Prosedürel p
	* Nesne Tabanlı
	* Generic (Türden Bağımsız Programlama) 
	Örn: Template
	* Functional Programming
	Örn: Lambda Expression
	* Data Abstraction
	Örn: Operator Overloading
	

C++ efficient bir dildir. 
* Memory ve CPU'yu etkin bir şekilde kullanabilirsiniz.

**Not:** 
* Undefined Behaviour: Compile time'da geçerli fakat run time'da belirsizlik yaratan davranışlardır.
* Unspecified Behaviour: Compiler'a göre davranışı değişen kod.

Undefined Behaviour örnekleri:
* int x;' i initialized etmeden kullanmak.
* nullptr'yi dereference yapmak.
* Buffer overflow

Unspecified Behaviour örnekleri:
* `a= f1() + f2() * f3();` Fonksiyonun çağrılması sırası compiler'a bağlıdır. 
* const char * p1= "Adnan";




