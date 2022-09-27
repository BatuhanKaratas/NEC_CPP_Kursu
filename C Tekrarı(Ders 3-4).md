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

| Operator  | Açıklama                  |
|-----------|---------------------------|
| [ ]       | Index                     |
| ()        | Function call             |
| .         | Access                    |
| ++, --    | Incremental , Decremental |
| *         | Dereferencing             |
| !         | Invert                    |
| &         | Address                   |
| sizeof    | Size                      |
| *         | Multiplication            |
| / , %     | Division, Mod Operator    |
| "<< , >>" | Bitwise Left-Right Shift  |
| == , !=   |                           |
| "> , <"   |                           |
| &         | Bitwise And               |
| ^         | Xor                       |
| |         | Or                        |
| ? :       | Ternary                   |
| ||        |                           |
| &&        |                           |
| +=        |                           |
| ,         |                           |

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
