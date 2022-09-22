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

* Scope leakage'ın bir diğer dezavantajı ise örneğin string nesnesi memory'de tutuluyor. Gereksiz yere bu nesne yok olana kadar memory'de yer tutmaya devam ediyor. 

```
if (int x= foo()) 
// x'i foo'nun return value'su ile initialize eder ve
// x != 0 kontrolü yapar.

if (int * x= foo()) 
// x'i foo'nun return value'su ile initialize eder ve
// x != nullptr kontrolü yapar.

```

* Karakter Türünün Tipi
	* 'A' C'de 4 byte int type
	* 'A' C++'da ise 1 byte char type'dır.

* String Literal Türünün Tipi
    * C'de "akif"'in değeri char[5] (4 karakter + null termination "/0")
    * C++'da "akif"'in değeri const char[5]'dir.
    C++'da string literal'ini değiştirmek undefined behaviour'dur. Çünkü string literal'i kullanıldığında gelen yapı const char *'dır. Yani array decay sonucunda &array[0] gelir;
    
#### SizeOf Operandı
* sizeof operand'ının tipi size_t'dir.
* sizeof unevaluated context'dir.
```
Örnek;
int x= 10;
size_t y= sizeof(x++);
printf("%zu %d/n", y, x); // Output: 4 - 10
```

#### Array Decay'in Uygulanmadığı Durumlar
*
  ```
  int a[10] = {1, 5, 7};
  printf("%zu/n", sizeof(a)); // Output: 40 (4 * 10)
  ```
  Çünkü a array'i sizeof'un içinde çağırıldığında Array Decay çalışmaz ve array'in tamamı gelir.

* a -> &a[0] // a array'i Array Decay açılımı
* &a -> int(*)[10] // Tüm array belirtmek için kullanılan tür. "&" operatörünü uygularsak array'e Array Decay çalışmaz ve array'in tamamı gelir.

```
//Array Decay çalışması:
printf("%p %p \n", a, a+1); Output: a[0]'ın adres değeri
                                    a[1]'in adres değeri
```


* C++'da String Literal Kullanımı
  * char *p= "batuhan"; // Syntax Error. Çünkü "batuhan" string literal'inin array decay'i const char *'dır. const T * to T*'a implicit dönüşüm C++'da yapılamaz.
  * char str[4]= "anıl"; // C'de bu ifade geçerlidir. Fakat array null termination karakterini içerecek kapasitede değil. Bu yüzden kullanımı tehlikelidir. C++'da ise kapasite yeterli olmadığı için bu bir Syntax Error'dur.
 
#### Ek Bilgi: Expressions (C++)

* 10u (unsigned int) 
* 4.f(float)
* 6llu (unsigned long long)
* x+5
* f(x)
* x++
* f(x)>g(x)
* x*x + y*y
* 10ul (unsigned long)


* Her expression'nın bir data type'ı ve value category'si vardır.

* short s1= 5, s2= 7;
  s1 + s2 expression'nın data type'ı integer'dır. Int'ın alt type'ları ile işlem yapılırsa int'e promote olur. (Integral Promotion)

*  char c= 'A'
*  +c veya -c expression'larının data type'ı int olur. Çünkü '+' ve '-' operatörleri ile char to int promotion gerçekleşir.
*  5 + 3.2 expression data type'ı double olur.
*  int x= 10; 
   x > 5 ? 3 : 4.7; // ifadesinin data type'ı double olur.
   

#### C Value Category
* L value expression (Memory'de yer tutan ve bir nesneye karşılık gelen ifadeler)
* R value expression 

##### Value Category Saptama Testi
* & operandını expression'nun önüne koyduğumuzda compiler syntax error vermezse bu ifade bir L value'dur. Hata verirse bir R value'dur.

Örnek;
```
char c = 10; (c bir L value'dur.)
+c ifadesi ise integral promotion'a uğrar 
ve tipi int'a dönüşür. 
Value kategorisi ise R value'ya dönüşür. 
```

#### C++ Value Category
* PR value
* L value
* x value

* int x= 10; (x L valeu'dur. Her değişken ismi bir L value'dur.)

* x++; PR value'dur.
* ++x; Left value'dur.
* 34; PR value
* int foo(); foo() PR value'dur.
* ```
  int x=10;
  int *p = &x; // *p L value'dur. 
  ```
* std::move(x); // X value'dur.
* int & foo(); foo() L value'dur.

#### Ek Bilgi:

#include <cstring> //C'den gelen string header'ı
#include <string> //C++ string header'ı
#include <cstdio> //C++'da bu şekilde çağrılır.
#include <stdio.h> //C'de bu şekilde çağrılır.

---

#### Initialization on C++

* int x; // Default initialization. C++'da otomatik ömürlü değişken default initialize edilirse compiler "garbage value" verir. Buda undefined behaviour'dur.

Static Ömürlü Değişkenler
* Global scope'daki değişkenler
* Static anahtar sözcüklü değişkenler
* String literalleri sonucunda oluşan const char * değişkenleri

Static ömürlü bir değişken tanımlandığında compiler Zero Initialization uygular.
Zero initialization değerleri;

  * Aritmetik değişkenler için 0
  * Boolean değişkenler için false
  * Pointer değişkenler için nullptr'dir.

##### C++ Initialization Türleri
* int x= 10; // Copy Initialization
* int x(98); // Direct Initialization
* int x{6}; //Uniform or Brace Initialization

```
Örnek;
double dval= 5.6;
int i= dval; // Legal narrowing conversion gerçekleşir. Loss of data oluşur.
int i2(dval); // Legal narrowing conversion gerçekleşir. Loss of data oluşur.
int i3{dval}; Illegal Syntax Error. Her zaman Uniform initialization tercih etmek daha güvenlidir. 
```

```
Örnek;
int x{}; // Value initialization  Value: 0.
// Value initialization'da static scope değişkenlere uygulanan zero initialization uygulanır.

int y; // Otomatik ömürlü değişken Garbage value olur ve bu C++'da bir undefined behaviour'dur.

// "int x{}" ile "int x{0}" aynıdır.

int a[4] = {};
int a[4] {};
int a[4] = {0};
// a array'ini initialize eden bu kod satırlarının hepsi aynıdır. Hepside a int array'inin tüm elemanlarını 0 ile initialize eder.
```

Not: const nesneler default initialize edilemez. Syntax error'dur.
Not: int &p referans değişkenler default initialize edilemezler. Syntax error'dur.


#### Ek Bilgi
#### Scoot Meyer - Most Vexing Parse

* Öyle bildirimler bilgi var ki 
  *  Nesne tanımlama (Object Instantiation)
  *  Fonksiyon Bildirimi (Function Decleration)

Bu bildirimlerde function decleration'nın önceliği vardır.

```
struct A {};
struct B { B(A); };
B bx(A()); // Most vexing parse çalışır ve compiler bunu bir fonksiyon bildirimi olarak algılar.
B bx{A()}; // Most vexing parse çalışmaz. Böylece bu bir nesne tanımlama olarak compiler tarafından algılanır.
```

#### Type Deduction

Compile Time ile ilgili şeyler static'dir.
Run Time ile ilgili şeyler dynamic'dir.

int * foo(const int *, size_t);

auto fp = &foo; // Auto type deduction
// Extraction of fp: int * (*fp) (const int *, size_t);

##### AAA(Almost Always Auto)

* Bu bir kod yazma stilidir. Açılımı neredeyse her zaman auto type deduction kullan.

auto x= 10; // Compile Time'da compiler x'in tipini int olarak oluşturur.

Not: auto değişkenin default initialization'ı olmaz.

const auto x = 10; // auto = int Legal
int ival{};
auto * x= &ival; // auto = int

Null Pointer Conversion
int * ptr= 0;
int * ptr= NULL; //NULL stdio.h'da #define edilmiş bir makrodur. Bu yüzden bu header dosyasını #include etmek zorunludur.

C++11'den itibaren;
int *p= nullptr; // Bu keyword 0 ve NULL macrosunun yerine geçmesi için gelmiştir. Keyword olduğu için include'a gerek yoktur. Türü nullptr_t'dir. Sadece pointer'ları assign ederken kullanılır. 


Not:
int *p{nullptr};

p == nullptr ve !p aynı ifadelerdir. 

nullptr PR value'dur.
