# TEMEL C BİLGİLERİ
Not: C-C++'da diğer dillerden farklı olarak undefined behaviour olmasının sebebi compiler'ın bu dilleri optimize etmesidir. C++ compiler'ı undefined behaviour olmayacağını varsayarak assembly kodu üretir.

#### Unspecified Behaviour

* `x = f1() + (f2() * 5);`

   Burada f1 mi f2 mi önce çağrılır belli değil bu bu bir   unspecified behaviour'dur.
   

* 
```cpp
const char * p1= "oytun";
const char * p2= "oytun";
  
if(p1 == p2)
```    
If case'de compiler iki string'i aynı diye optimizasyon yapıp aynı adrese  point ettirebilir veya ettirmeyebilir.
    

#### Implementation Defined
* Unspecified behaviour'un alt kümesidir.
* int tipi'nin bellekteki yeri compiler'a göre değişkenlik gösterebilir. (2 or 4 or 8 byte olabilir) Compiler dokümanında bu bilgi yer alır ve hep aynı sonuç alınır. Diğer behaviour'lardan farklı olarak.
* char tipinin unsigned veya signed olması compiler'a bağlıdır. Bu seçim compiler dökümantasyonunda belirtilir. 


#### Basic Types

- char - signed char - unsigned char > 1 byte 
- short - unsigned short > 2 byte
- int - unsigned int > 4 byte
- long - unsigned long > 8 byte
- long long - unsigned long long > least 8 byte
- bool > 1 byte
- float > 4 byte (Reel)
- double > 8 byte (Reel)
- long double > least 8 byte (Reel)

Unsigned to Signed dönüşümü için 2's Complement Yöntemi kullanılır.


+17 > 0001 0001 
-17 > 1110 1111

Not: 2' Complement'de least significant bitten başlanarak 4 bit sayılır. 1'e denk gelene kadar aynı bitler korunur. 1'den sonra diğer bitlerin tersi alınarak işlem tamamlanır.

Bir sayıyı C'de;

- decimal
- octal (0 ile başlar)
- hexadecimal (0x ile başlar)
- binary (ob ile başlar) ile ifade edebiliriz.

##### Sonuç Tipi

* i1 / i2 > int
* i1 / 5. > double
* i1 * 1000 > int
* i1 * 1000l > long
* 12 > int (default)
* 12u > unsigned int
* 12ul or 12lu > unsigned long
* 12ll > long long
* 12ull or 12llu > unsigned long long
* -2'147'483'648 > 2 ^ 31 max int value'dur. Tipi long long'dur. - operatörü sayıya dahil değildir. Bu bir çarpım işlemidir.
* 0x1122u > unsigned int
* 0b1001'0001ll > long long
* 4.5 > double(default)
* 4.5f > float 
* 3.4L > long double

#### Scope - Name Lookup

Compiler derlemeden önce kodu tokenize eder. Variable isimlerini saptar ve kime ait olduklarını bulmaya çalışır. Buna name - lookup denir.

```cpp
{
	int printf= 5;
	printf("mrb"); 
}
```

Burada syntax hatası oluşur. Çünkü compiler önce printf'i bulunduğu local scope'da arar ve onun bir int olduğuna kanaat getirir. Int değişkenle func call ise hataya sebep olur.

```cpp
{
	printf("mrb"); 
	int printf= 5;	
}
```

Burada ise bir hata yok. Local'de printf fonksiyonunu arar sonra stdio.h header'ında ilgili fonksiyonu bulur.

* Name lookup belli bir sıraya göre yapılır. (Local to Global Scope )
* İsim bulunduğunda arama sonlandırılır.
* Aynı scope'da birden fazla isim bulunursa ambiguity oluşur.

C++ Scope türleri;
* Block scope
* Namespace Scope
* Function prototype Scope
* Function Scope
* Class Scope

Not: Aynı scope'daki name identifier'lara aynı isim verilirse syntax hatası oluşur.

Not: C++'da name lookup'ın nereden başlayacağını belirtebiliriz.

Örnek;
```cpp
{
	int x=5; 
	::x= 10; // Global scope'da aramaya başla 
	// Local'de arama demektir.
}
```

#### Expression - Statement

Expression örneği; x + 5
Statement örneği; x + 5;

Her expression'ının **Data Type**'ı ve **Value Category**'si bulunur. 

| Expressions | C       | C++     |
|:-----------:|---------|---------|
| +x          | R value | R value |
| ++x         | R value | L value |
| --y         | R value | L value |
| a, b        | R value | L value |
| ? :         | R value | L value |
| assignment  | R value | L value |

`void func (int && r);`
R parametresinin;
**Data type**'ı: R value'dur.
**Value Category**'si L value'dur.

#### Constant Expressions
Compiler time'da değişkenin değeri biliniyorsa constant expression'dır.

* int a[constant expression]
```cpp
const int x= 10;
int a[x]; // C'de compile time error'dur. x değişkeni C'de constant kabul edilmez. C++'da bu legaldir.
```
* Global değişkenlerin initializer'ı olan ifadeler.
 ```cpp
int x= 30;
int y= x + 5; // Illegal for C Syntax error, Legal for C++
```
* Switch case ifadeleri
* Bit field member
```cpp
struct nec {
int x: 5; // ':' sayesinde x değişkeni 5 bitlik alanda tutulur. Bu değer constant olmalıdır.  
}
```
#### Operators
Öncelik sırasına göre (baştan - sona) operatörler;

| Presedence | Operator                                                                                                         | Description                                                                                                                                                                                                                                                                                                                                      | Associativity |
|:----------:|------------------------------------------------------------------------------------------------------------------|:------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:-------------:|
| 1          | ::                                                                                                               | Scope resolution                                                                                                                                                                                                                                                                                                                                 | Left-to-right |
| 2          | * a++   a--	
* type()   type{}	
* a()	
* a[]	
* .   ->                                                           |  Suffix/postfix increment and decrement
 Functional cast
 Function call
 Subscript
 Member access                                                                                                                                                                                                                                                | Left-to-right |
| 3          | * ++a   --a	
* +a   -a	
* !   ~	
* (type)	
* *a	
* &a
* sizeof	
* co_await	
* new   new[ ]
* delete   delete[]	
 | Prefix increment and decrement	
Unary plus and minus
Logical NOT and bitwise NOT
C-style cast
Indirection (dereference)
Address-of
Size-of[note 1]
await-expression (C++20)
Dynamic memory allocation
Dynamic memory deallocation                                                                                                                | Right-to-left |
| 4          | .*   ->*                                                                                                         | Pointer-to-member                                                                                                                                                                                                                                                                                                                                |               |
| 5          | a*b   a/b   a%b                                                                                                  | Multiplication, division, and remainder                                                                                                                                                                                                                                                                                                          |               |
| 6          | a+b   a-b                                                                                                        | 
Addition and subtraction
                                                                                                                                                                                                                                                                                                                       |               |
| 7          | <<   >>                                                                                                          | 
Bitwise left shift and right shift
                                                                                                                                                                                                                                                                                                             |               |
| 8          | <=>                                                                                                              | 
Three-way comparison operator (since C++20)
                                                                                                                                                                                                                                                                                                    |               |
| 9          | 
<   <=   >   >=
                                                                                                | 
For relational operators < and ≤ and > and ≥ respectively
                                                                                                                                                                                                                                                                                      |               |
| 10         | ==   !=                                                                                                          | 
For equality operators = and ≠ respectively
                                                                                                                                                                                                                                                                                                    |               |
| 11         | 
a&b
                                                                                                            | 
Bitwise AND
                                                                                                                                                                                                                                                                                                                                    |               |
| 12         | 
^
                                                                                                              | Bitwise XOR (exclusive or)                                                                                                                                                                                                                                                                                                                       |               |
| 13         | 
|
                                                                                                              | Bitwise OR (inclusive or)                                                                                                                                                                                                                                                                                                                        |               |
| 14         | &&                                                                                                               | 
Logical AND
                                                                                                                                                                                                                                                                                                                                    |               |
| 15         | 
||
                                                                                                             | 
Logical OR
                                                                                                                                                                                                                                                                                                                                     |               |
| 16         | * a?b:c	
* throw	
* co_yield	
* =	
* +=   -=	
* *=   /=   %=	
* <<=   >>=	
* &=   ^=   |=                        |  Ternary conditional                 throw operator
yield-expression (C++20)
Direct assignment (provided by default for C++ classes)
Compound assignment by sum and difference
Compound assignment by product, quotient, and remainder
Compound assignment by bitwise left shift and right shift
Compound assignment by bitwise AND, XOR, and OR |               |
| 17         |                                                                                                                  |                                                                                                                                                                                                                                                                                                                                                  |               |

`int * p; // Bir declerator'dür. Operatör değildir. `

##### Short Circuit Behaviour

`False && ...` ise sağ operand evaluate edilmez.
`True || ...` ise sağ operand evaluate edilmez.

Bu durumlara short circuit behaviour denir.

##### Comma Operator
`int a[10]= {1,(Seperator comma) 2, (3,(Operator comma) 4)};`

##### Sequence Point
Kaynak kodda belirtilen bir noktadır.

```cpp
int x= 10;
x++;
// Bir sonraki line'da x= 11 olduğu garantidir. Çünkü ";" bir sequence point'tir.  
```
Sequence Points;

* Statement ";"
* Comma "," 
```cpp
`int x= 5, y = ++x, z= ++y;// Legaldir. Çünkü , bir sequence pointtir.` 
```
```cpp
if(++x, f(x) != 0)
```
```cpp
int a[5]= {1, (2, 3, 4, 5)};
// a array content sırasıyla: 1, 5, 0, 0, 0'dır.
```
* if(exp)
```cpp
if(x++ > 10) {
    foo(x) // x++'lı değerin kullanılması burada garantidir.
}    
```
* while(exp)
* for( ; ; ), Her bir semicolon sequence pointtir.
* "&&" 
```cpp
int x= 4;
x++ && f(x); // Burada f(x)'e kesinlikle 5 değeri gider.
``` 

Not(Operatör Önceliği): 

```cpp
int x=10, y= 20;
int z;
z= x, y; // İşlem sırası 1- z= x; 2- y; 
// Çünkü assignment operatörünün comma ya göre önceliği vardır.
// z'nin değeri burada 10'dur.
z= (x, y);// İşlem sırası 1- (x, y) 2- z= y;
// Çünkü parantez operatörünün assignment'a göre önceliği vardır.
// z'nin değeri burada 20'dir.
```

##### sizeof Operatörü
* sizeof değeri bir constant expressiondır.
* Bu operatörün ürettiği değerin data type'ı size_t'dir. (Compiler'a göre unsigned long veyan unsigned long long olabilir.)
* Unevaluated context'dir. x++ ve y– değerleri kalıcı olmaz.

##### > Operatörü
*  if( sizeof(x) >signedIntVal )
Compare operatöründe iki operand'ın değeride unsigned veya signed olmalıdır. Böyle olmazsa signed kısım implicit conversion ile unsigned'a dönüştürülür.
* Reel sayıların compare edilmemesi gerekir. 
```cpp
double x= 0.;
for(int i{0}, i < 10; ++i) {
    x+= .1;
}

if(x == 1.) // False olur çünkü reel sayının tüm basamakları 1. reel sayısına eşit değildir.
```

##### >> Operatörü
```cpp
int x= 10, y= -3, z= 33;

x << z; // Undefine behaviour max 32 olmalı shift sayısı. Çünkü int 4 byte (varsayılan)
y << x; // undefined behaviour
x << y; // undefined behavior negatif sayılar shift operatörünün bir operand'ı olmamalıdır.

short a= 5;
int b= 17;
a << b; // Legaldir. Çünkü short değişkeni int ile operand olduğu için "integral promotion" olur ve implicitly int'e cast edilir. 
```
####  Type Conversion
* Implicit Type Conversion (Arithmetic ve Assignment conversion)
* Explicit Type Conversion (Compiler'ı dönüşüme zorlamak)
    - Static cast
    - Dynamic cast
    - ReInterpret cast
    - Const cast

Implicit Arithmetic Conversion Kuralları
* Farklı rank'ler söz konusu işaretler aynı ise işlem yüksek rank'e sahip operand'ın türüne dönüştürülür.
* Farklı rank'ler ve farklı işaretler söz konusu ise yüksek rank'e sahip unsigned operand'ın türüne dönüşüm yapılır. 
* Int veri tipinden düşük tipteki operand'lar için;
    "short + short" veya "char + char" sonucu int türüne dönüştürülür. (**Integral Promotion**)
* Aynı rank ve farklı işaretler ise dönüşüm "unsigned" operand'ın türüne dönüştürülür. 
    "unsigned int a + int b" sonucu unsigned int'e dönüştürülür. 
    int x= -1;
    unsigned int y = 1;
    if(x > y) // Implicit conversion olur.
    x unsigned int'e dönüştürülür. 
 
**Mülakat Sorusu:**
```cpp
#define asize(a) (sizeof(a) / sizeof(*a))
int main() {
    int a[5]= {1, 2, 3, 4, 5};

    for(int i= 2; i < asize(a) - 2; i++)
    {
        printf("%d", a[i + 2]);
    }
}
```
Output: Print yapılmaz. Çünkü size_t unsigned tipinde bir değişkendir. Implicit conversion sonucunda i değeri unsigned int'e dönüştürülür. 

* Undefined behaviour - Signed Types Arithmetic Operations Overflow

i1 + i2; 
i1 * i2;

Bu işlemlerin sonucu int'i aşarsa undefined behaviour oluşur.

Not: İşaretsiz türlerde overflow yoktur. En büyük işaretsiz sayıya +1 eklersek değeri 0'a döner. Bu yüzden undefined behaviour oluşmaz.

##### Contol Statements

* `while ( n > 0) { n--; }` ile `while (n--) { }` aynıdır.

Not: Maximal Munch Rule

```cpp
int x= 5;
int y= 9;
int a = x+++y; // Legal soldan sağa doğru compiler token ederken parse edeceği maksimum karakteri alır. "x++ + y" şeklinde ayrım yapılır.
int b= x++++y; // Illegal Syntax error. Çünkü compiler bu ifadeyi mmr kuralına göre "x++ ++y" şeklinde ayırır. Bu bir syntax hatası oluşturur.

```

Not: `for ( ; ; )` // Legal ortadaki conditional state default olarak true kabul edilir.

##### Array Decay
```cpp
int a[10];
a[5]= *(a + 5);
5[a] = *(a + 5); // Legal'dir.
```
Tek başına a ifadesi a dizisinin ilk elemanının adresini döner. Yani (&a[0] -> Right value)

C-C++'da arraylar assignment operatörü ile birbirine kopyalanamazlar.

int a[10]= {0};
int b[10]= {0};

a = b; // Syntax error

char str[100] = "murat";
str= "alican"; // Syntax error

Sebebi; str array decay sonucunda &str[0]  Right value'dur. Right value'ya assignment yapılmaz.

Not: (* a) yani (* &a[0])  (a[0])'dır.'

##### Pointers (Address)

* Object Pointers (Nesnenin adresi)
* Function Pointers (Fonksiyonun adresi)

int * p; // Pointer to Int

Not: Pointer'ların amacı bir nesnenin adresini tutmaktır.

int * p;
double * c;
char * d; 

sizeof(p)= sizeof(c)= sizeof(d)

Her pointer memory'de aynı yer kaplar (Örn: 4 byte). Fakat point ettiği pointee'nin boyutu değişebilir. 

int *gp; // Global değişken static ömürlü

void func(int *p) // p yerel otomatik ömürlü
{
    int *xp; // xp yerel otomatik ömürlü
    static int * sp; // Global değişken static ömürlü
}

* gp ve sp pointerlarının ilk değeri nullptr'dir. Çünkü global değişkenlerin initialization'ı zero initialization olur. 
* Otomatik ömürlü pointer'ların ilk değeri garbage value olur.

C/C++ Storage Türleri
* Automatic Storage
* Static Storage
* Dynamic Storage
—

Ara Not:
```cpp
typedef int * ptr;
ptr p1, p2, p3;
```
##### Valid-Invalid Pointer
* int *p; // Otomatik scope bu yüzden Invalid pointer. Çünkü value'su garbage'dır.
```cpp
int a[10] ; // Array decay -> &a[0]
a-1; // Invalid address
a+15; // Invalid address, Array overflow
```
* int a[5]= {0};
    int * p= &a[5]; // Valid pointer. Array'in sonunu gösteren pointer'lar hala valid'dir. Fakat bu pointer'ı dereferencing etmek invaliddir.
    &a[6] // Invalid pointer.
 * int *p= nullptr; // Valid pointer, garbage value değildir. 

##### '&' Address of Operator
* ''&' operatörünün operand'ı left value olmak zorundadır.
```cpp
int x= 10;
int * ptr1= &x; // Legal Type: int *
int ** ptr2= &ptr1; // Legal, Pointer to pointer
int ***ptr3= &ptr2; // Legal, Pointer to pointer to pointer
```
##### Operator Öncelik Yönü
* !~++x; // Bu ifadede ki operatörlerin önceliklerinin aynı olduğunu varsayalım.
    Öncelik yönü sağdan sola doğrudur.
* a % 10 * x * y / z; // Bu ifadede ki operatörlerin önceliklerinin aynı olduğunu varsayalım.  Öncelik yönü soldan sağa doğrudur.
* x++ * y > 10 // Öncelik yönü soldan sağa doğrudur.

##### Dereferencing Operatörü
* * operatörünün operand'ı adres tipinde olmak zorundadır.
```cpp
int x= 10;
*x; // Illegal'dir. Syntax error  
```
* * operatörü ile adresteki nesneye erişebiliriz.
```cpp
int x= 10;
int * ptr= &x;
// "*ptr" tipi int'dir.
++*ptr; // Tipi int'dır.
&*ptr; // Tipi int *'dır.
```

```cpp
int z{};
int i{};
int *q= &z;
int *p= &i;
q= p; // Valid Q artık p'nin point ettiği nesneye point edecektir.
```

```cpp
int * foo(); // foo'nun return type'ı bir adrestir. (pointer)
int * ptr= foo();
```

##### Pointer Aritmetiği
```cpp
int a[10]= {1, 2, 4, 6};
&a[3] + 1; // Pointer 1 arttırılarak bir sonraki array elemanının pointer'ına geçiş yapılmıştır. &a[4]
int * ptr= a;
++ptr; // &a[1]'dir.
a[b]; // *(a + b) demektir.
ptr[3]; // *(ptr + 3);
ptr[-2]; // *(ptr - 2);
// a ve a+1 pointerları (adresleri) arasındaki fark 4'tür. Çünkü bir pointer 4 byte'lık yer kaplar.
// &a ve &a + 1 pointerları (adresleri) arasındaki fark ise 40'dır. 
// &a array'in tamamını gösterir. & operatörü kullanıldığında array decay çalışmaz.

int b[4][64]= {0};
// b -> First row ve b+1 -> Second row olur.

void func(int *p);
void foo(int p[]); 
// Bu iki fonksiyonunda parametreleri aslında aynıdır. Çünkü foo parametresinde p array decay çalışır ve bununda türü int *'dır.

int c[10] = {0};
auto p= &a; 
// p'nin türü int(*p)[10]'dur. Çünkü & operatörü array isminin önüne gelirse array decay çalışmaz. Array'in kendisini döner.
```
##### Dangling Pointer
```cpp
int * get() {
    int x{};
    x= 5;
    return &x;
}

int * p= get(); 
*p; // Eğer p'yi dereferencing etmeye çalışırsak dangling pointer olduğu için yani pointee'si get fonksiyonunda ki otomatik scope'dan dolayı yok olduğu için bu undefined behaviour'dur.
```
Not: Sağlıklı bir adres(pointer) return'ü için;

* Statik ömürlü nesne adresi
    * Global
    * Local Static
    * String literals
 * Dynamic ömürlü nesne adresi
 * Call'dan gelen adres kullanılabilir.


char s1[]= "alican";
char s2[]= "alican";

if(s1 == s2) // Farklı array'ler farklı adreslerde tutulur kesinlikle false'dur.

Not:
char *p;
p bir yazıyı gösteriyor ve p'nin gösterdiği yazı boş olmayabilir.

if(p != nullptr && *p ! = '\0' ) // Eğer short circuit uygulanmasaydı p'nin nullptr olduğu durumda * dereferencing alındığında undefined behaviour oluşacaktı.
—
```cpp
int a[]= {1, 2, 3, 4, 5};

int *p= a;
int *pl= a + 5;
while(p != pl) {
    cout << *p++;
}
```

Bu while loop içerisinde ki contol ifadesi bir C/C++ idiom'udur. p pointer'ı önce arttırılır sonra dereferencing'i alınıp bastırılır. Sonunda p pl'ye ulaştığında (endpoint gibi düşünebiliriz) loop'dan çıkar.

```cpp
int a{5}, b{6};
    
int * ptr{&b};


void foo(int **p) // Pointer to point
{
    std::cout << (*(*p)) << "\n"; // Output 6
    *p= &a; // Dereferencing ile pointe'ye ulaşılır ve pointe başka bir pointer ile değiştirilmiş olur. 
    std::cout << (*(*p)) << "\n"; // Output 5
}

int main()
{
    foo(&ptr);    
    return 0;
}
```
