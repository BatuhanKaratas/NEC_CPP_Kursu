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

* ```
  const char * p1= "Adnan";
  const char * p2= "Adnan"; 
  ```
  String literalleri static ömürlüdür. (program sonuna kadar tutulur.) "Adnan Aydın" ifadesi memory'de iki farklı yerde mi tutulacak? (Compiler'a bağlıdır)
 
Implementation Defined Behaviour örnekleri;
* `char c; c << 4;`
  Bu durumda c'nin signed unsigned olduğu bilinmiyor. Shift işlemi sign'lı bitten itibaren mi kaydırılmaya başlanacak?

* `int a;`
  Bellekte ne kadar yer tutacak? (4 byte or 8 byte)

### C-C++ Farklılıkları

* Implicit Int
  ```
  func(int x) { 
   return x+1; 
  }
  ```
  Function return type   belirtilmezse C'de default unsigned int kabul edilir.
  

* Non-Return Statement: C'de void dışında bir return type'a sahip bir function kendi içinde return etmesede olur.

* Implicit Function Decleration: C'de tanımlanmamış fonksiyonlar bile default olarak tanımlanır. (`"int bar()"` şeklinde) 

* Void Func Parameter: C'de "int foo()" ile "int foo(void)" aynı değildir.
   * "int foo()": Parametre alabilir.
   * "int foo(void)": Kesinlikle parametre alamaz.

* Function Parameter Name: C'de "void func(int)" bir syntax error'dur. 
 Not: C++'da bu parametre isim zorunluluğunun olma sebebi;
   * Kullanılmayacak parametrelere isim verilmez.(Clean Code)
   * Function signature'ını overloading için değiştirmekte kullanılır.
 
 * Inner Loop Extra Block
 ```
 for(int i=5; ; )
 {
    {
    	int i= 66;
    }
 }
 ```
 C'de compiler for içinde ekstra bir blok açar.(Assembly code) Bu yüzden for içerisindeki ve blok içerisindeki değişkne ismi aynı olsa bile syntax hatası oluşmaz.
 
#### Türlere ve Tür Dönüşümlerine İlişkin Farklılıklar

* "Bool" veya "bool" type'ın değeri unsigned int'dir. C++'da li gibi gerçekten true-false değildir.
	* Non-zero tüm değerler true'dur.
      Zero değerler ise false'dur.
    * C++17 ile bool değişkeni ++ veya -- ile çalışması engellendi. Çünkü bu veri tipi 1 byte ve bitsel olarak memory'de tutulamıyor.
    * C++'da aritmetik type'lardan bool'a dönüşüme müsade edilir. 
    ```
    int x= 45;
    bool b= x; // Implicit conversion. 
    // Legaldir. Value:true
    ```
   * C++'da pointer to bool'a dönüşüme müsade edilir.
    ```
    int x=100;
    int * ptr= &x;
    bool b1= ptr; // Legal Value: true;
    int *ptr2; 
    // Pointer default initialization değeri nullptr'dir.
    bool b2= ptr2; // Legal Value: false;
    // nullptr = false, !nullptr = true.
    ```

  * C++'da bool'dan aritmetiğe dönüşüm var fakat pointer'a dönüşüm yoktur.
  ```
  bool b= true;
  int x= b; // Legal value: 1
  bool b1= false;
  int y= b1; // Legal value: 0
  int *ptr= b; // Illegal Syntax Error
  ``` 
 * User Defined Type (Struct, Enum, Union)
 C'de User Defined Type'ların type def olarak tanımlanması lazımdır.
 ```
 typedef struct Data{
	 int a, b, c;
 } Data;
 ```
 
#### Const anahtar sözcüğüne yönelik farklılıklar
* Const Variable Initialization
  C'de "cont int x;" // Legaldir. Initialize zorunluluğu yoktur. Default initialize edilir.
  Fakat C++'da const değişkenler initialize edilmek zorunda. (Global değişkenler hariç)
* Const Expression Usage
```
const int x= 10;
int a[x]= {0}; // C için syntax hatası
// C compiler'ı x'i const expression olarak değerlendirmez.
```
 
Not: C++'da const değilkenleri initialize ederken sabit bir sayı verirsek const özelliğini kaybetmeden kullanabiliriz. Fakat  global fonksiyon ile initialize edilirse const özelliği kaybolur.

```
const int size= 100;
const int ds= getsize();

int a1[size]
int a2[ds] // Syntax error
```

#### External - Internal Linkage

C++'da;

```
const int x; // External linkage
const static int x; // Internal linkage
```


C++'da global const nesneler internal linkage'dır.
C'de global const nesneler external linkage'dır.

#### Genel C++ Farklılıkları

Not: 
```
`int x= 10; // Bu adresin türü(&x) int *'dır. `
`const int x= 10; // Bu adresin türü(&x) 
// const int *'dır.`
```

* C'de "const T *" to "T *"'a implicit conversion vardır.
C++'da bu dönüşüm gerçekleşmez.

```
cosnt int x= 10;
int *p= &x; // C++ için syntax error. C'de OK.
```

```
int x= 10;
int * const ptr= &x; // const ptr to int
// "ptr" hayatı boyunca x'i göstermek zorunda.
int y{};
ptr= &y; //Syntax error
*ptr= 45; (Pointer dereferencing)

const int * ptr= &x; // ptr to const int
// "*ptr" ptr'nin gösterdiği değer değişemez.

*ptr= 45; // Syntax error

const int * const p = &x; // const ptr to const int
// "*ptr" ptr'nin gösterdiği değer 
// ve değişken değişemez.

```

```
void func(T *p) // Out param (Read-Write)
void foo(const T * p) // In param (Just Read)
```

* C++'da undefined behaviour' a sebebiyet vermemek için değişkenlerin ilk değerleri her zaman atanmalıdır.

```
const int * ptr; // C++'da legaldir.
int * const ptr; // C++'da Syntax Error'dur. 
// ptr'ye ilk değer atanmak zorundadır.
```

* C++'da T to T * implicit dönüşümü yoktur.
```
// C'de
int x= 35;
int *p= x; // Legal implicit conversion int to int *

//C++'da
int *p = x; // Syntax Error
``` 

* C++'da farklı tür pointer'lar arası implicit dönüşüm yoktur.

```
//C'de
double dval= 45.2;
char *p= &dval; double * to char * implicit conversion var.
//C++'da
char *p= &dval; double * to char * implicit conversion yok.
// Syntax Error
```   

* C++'da void * to int * implicit dönüşümü yoktur.
```
//C'de
int * p= malloc(10 * sizeof(int); // void * to int * 
//dönüşümü var.

//C++'da
int * p= malloc(10 * sizeof(int); // void * to int * 
//dönüşümü yok. Syntax Error

```

#### Enum Data Structure Farkları

* C'de enum bileşenlerin underlying type'ı (default type) int'dir. Değiştirilemez. C++'da enum bileşenlerinin type'ını user seçebilir veya enum bileşene atanan değer range dışındaysa  örneğin; double'a çekilir. 

* C'de aritmetik türlerden enum türlere dönüşüm var. Hatta farklı enum türleri arasında da dönüşüm var.

```
enum Color {W, G, B};
enum Color myColor;
myColor = 2; // // Aritmetik to Enum
// C'de Legal 
// C++'da Illegel Syntax error

enum Color myColor2= G;
int x= myColor2; // Enum to aritmetik
// Legal for C and C++
// Fakat "enum class" or "scoped enum" yapısı ile 
// C++'da bu dönüşümede müsade edilmedi.                          
```

#### Ek Bilgi:
#### Scope Leakage

Bir değişkenin kullanım alanı dışındada varlığını sürdürmesi.

```
std::string getstr();

void func()
{
	std::string s= getstr(); // Scope Leakage oluştu.
	
	if (s.size() > 10)
	{
		
	}
}

-----------------------------------------------------

void func()
{	
	if (std::string s= getstr(); s.size() > 10)
	{
		// S'in scope'unu kısıtladık. If scope'u içine 
		//  koyduk. Çünkü bu scope yeterli bizim için.
		// If içinde initialization C++17 ile gelen bir 
		// özelliktir.
	}
}
```

* Scope leakage'ın bir diğer dezavantajı ise örnepin string nesnesi memory'de tutuluyor. Gereksiz yere bu nesne yok olana kadar memory'de yer tutmaya devam ediyor. 

```
if (int x= foo()) 
// x'i foo'nun return value'su ile initialize eder ve
// x != 0 kontrolü yapar.

if (int * x= foo()) 
// x'i foo'nun return value'su ile initialize eder ve
// x != nullptr kontrolü yapar.

```
