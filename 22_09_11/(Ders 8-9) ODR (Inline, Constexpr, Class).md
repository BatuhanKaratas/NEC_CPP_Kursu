#### Predefined Symbolic Constants
--cplusplus -> Bu predefined constants sadece C++'da compiler'larında tanımlıdır.

auto x= --cplusplus;
// İçerisinde cplusplus versiyonu ile ilgili bilgiler vardır.

Common problem: C++'dan C fonksiyonu call edilirse linker hata verir.
Çünkü "decorator" C fonksiyonu C++'a göre decorate olur. Fakat C ve C++'da fonksiyon tanımlamaları farklıdır.

C Fonksiyon tanımı: func()
C++ Fonksiyon tanımı: func(i1, i2);

C++'da overload yapılabildiği için parametrelerde belirtilir.

Çözüm;
"cnec.h"

```cpp
#pragma once

#ifdef --cplusplus
    extern "C" { //Bu komutun anlamıscope'u içerisinde ki fonksiyonları C compiler formatına göre object dosyasına işle demektir.
#endif
    int func(int); (C fonksiyonu)
        
#ifdef --cplusplus
}
#endif 

// Böylece func C++ tarafından C formatında derlenir ve link esnasında hata oluşmaz.
```
## ODR(One Definition Rule)
Yazılım varlıkları (function, template, variable) tüm proje içerisinde sadece bir defa tanımlanmalıdır. Bu yapılmazsa proje ill formed olur ve link esnasında hata alabiliriz.

int foo(int); // Decleration

int foo(int) // Definition
{
    return 1;
}

Kural: foo definition tüm projede farklı class'lar için sadece 1 defa define edilmelidir.

```cpp
"header.h"
#pragma once

//Internal Linkage: Sadece o header'a özgü.    
const int x= 10; // Global namespace scope'da const değişken C++'da Internal linkage'dır. ODR'a uyar.
static int x= 10; // Global namespace scope'da static değişken C++'da Internal linkage'dır. ODR'a uyar.
int x= 10; // External linkage'dır. ODR'a uymaz.

static void foo(int) // Global namespace scope'da static fonksiyon C++'da Internal linkage'dır. ODR'a uyar.
{
    
}    

void foo(int) // External linkage'dır. ODR'a uymaz.
{
    
}
```

"header.h" birden fazla kaynak dosyasında include edilirse ve External linkage varsa ODR ihlal edilir ve link esnasında hata alırız.

Eğer yazılım varlıklarının definition'ları aşşağıdaki yapılarla yapılırsa birden fazla kaynak dosyada tanımlarının olması(bu header'ın birden fazla source file'da include edilmesi ile gerçekleşir) ODR açısından sorun oluşturmaz.  

* Class Definitions
* inline Function Definitions
* inline Variable Definitions // C++17'den beri
* constexpr Functions
* constexpre Variables
* Class Templates
* Function Templates

#### Inline Functions
Inline expressions(compile optimization)

C++ compiler'ları kodun davranışı değişmediği sürece kodu yeniden düzenleyerek optimize eder. (Assembly'e dönüştürür)

C++'ın bu kadar efficient olmasının sebebi budur.

Compiler optimization örnekleri;
* Dead code elemination
* Farklı statement'larda yapılan işleri ortaklayarak işlem yükünü azaltır. İlerde kullanılacağını bildiği bir değişkeni ayrı bir memory alanında tutar.
* Loop içerisinde kullanılmayan işlemler varsa, döngü dışına alınır.
* Loop Unrolling: Loop kıyaslamalarının çok maliyet yaratacağını düşünürse loop yerine döngü sayısı kadar statement yazar.
* Loop Reversal: Loop'un begin'den başlarsa daha fazla yük getireceğini tespit ederse loop'u end'den başlatır.

Kendim optimizasyon yapmalı mıyım ?

* Eğer kod yavaş çalışmıyorsa optimizasyonu compiler'a bırakmak en doğrusudur.
* Eğer kod yavaş çalışıyorsa, data structure ve algoritmaya bak gerekli güncellemeleri yaptıktan sonra benchmark yap.

##### Inline Expansion (Efficiency'e katkısı önemli)

```cpp
//Compiler generated codes
a= func();
//Compiler generated codes
```
Eğer func() fonksiyonu compiler'ın göremeyeceği bir scope'da ise (örneğin  bir source file'da define edilmiş) compiler burada optimizasyon yapamıyor ama inline olursa derleyici optimizasyon yapabilir.

Inline expansion'nın uygulanma şartları;
* Compiler kodu görmeli (Definition'ı header'da olmalı)
* Compiler tarafından Inline expansion'nın bir fayda getirdiğinin tespit edilmesi lazım
* Compiler türüne göre inline expansion uygulama kararları değişiklik gösterebilir. (gcc - clang)
* Inline optimizasyonun açık-kapalı oldğunu veya hangi seviyede yapılması gerektiğini uygun flag'lerle compiler'a bildirmeliyiz. ("No-inline" or "Always_inline" or "o-size" or "o-3" ) 

Inline expansion hangi fonksiyonlarda kullanışlı?
* Çok fazla çağrılan
* Kod line'ı az olan

Örnek;
Call chain

a= f1(1); -> f2(1) -> f3(1)

Inline expansion version

f1 Code
f1 Code
f1 Code
f2 Code
f3 Code

Eğer inline expansion f1, f2 ve f3 için uygulanabilirse, Compiler  senaryoya göre bir bütün olarak  f1, f2 ve f3 code'larını optimize edebilir.

```cpp
inline int func(int x, int y)
{   
}    
```

* Header dosyasında define edilebilir. Çünkü ODR rule'u ihlal etmez. Böylece birden fazla source file'da include edildiğinde bu header dosyasının birden fazla definition'ı olacak kaynak dosyalarda ama ODR ihlali gerçekleşmeyecek.
* Böylece ODR'yi düşünmemize gerek kalmadan, inline keyword'ü ile derleyici ilgili source file içinde func(...)'ın kodunu görüp inline expansion yapabilecek.

Header only library: Tüm yazılım bileşenlerinin definition'ı header'da yapılan kütüphanelerdir. 

Not: Cpp'nin standart library'lerinin çoğu header-only library'dir. Çünkü fonksiyon tanımları sadece inline olarak header'da yapılır. Bu da inline expansion'a teşvik olur. 

##### Inline keyword tradeoff'ları;

* Fonksiyonlara inline koyarak include edildiğinde source file'larda tek bir fonksiyonunun oluşmasını sağlar yani ODR ihlal edilmez. Memory'de ilgili header'ın include'unun gerçekleştiği source file'larda func'ın adresi aynıdır. Inline olmasa farklı olurdu. (+)
* Inline fonksiyon olursa header'da define olmak zorunda. Bunu da client'lar görebilir. Örneğin library'yi kullanan diğer programlar  (-)
* 
"hakan.h"
```cpp
#include "data.h"

inline void foo()
{
    Data mydata; // (Complete Type) olduğu için data.h'i include etmek zorundayız. 
}    
```
Fonksiyon inline olduğu için header'da tanımlamamız gerekiyor. mydata complete type olduğu için include etmek zorunda kaldık. Buda include chain'i beraberinde getirdi. 
Bu chain'de bir dosya değişirse tüm dosyalar recompile edilmek zorundadır.(-)
Eğer inline yapılmadan Data struct'ı forward decleration yapılsaydı yada hiç kullanılmasaydı include chain oluşmayacaktı.

Not: Gerekmedikçe include yapmayın forward decleration kullanın. 


#### Complete-Incomplete Type

```cpp
class Nec; // Forward decleration. Compiler bu decleration'la sadece Nec class'ının olduğunu bilir içeriğini bilemez. (Incomplete type)

class Nec
{
    void f1(); // Compiler Nec ile ilgili tüm bilgiye sahip.
} //(Complete type)    
```

##### Incomplete Type Usages

```cpp
struct Nec; // Forward decleration (Incomplete type) 

Kullanım örnekleri;

Nec f1(Nec); // OK
Nec * f2(Nec &); // OK
using NecPtr= Nec *; // OK
using NecRef = Nec &; // OK
extern Nec necx; // OK
struct x { Nec * ptr;} // OK. (Pointer tipinde olduğu için size'ı bellidir.)
inline void foo() // OK (Pointer tipinde olduğu için size'ı bellidir.)
{ Nec * ptr= nullptr; }

class Murat { static Nec snec; } // OK
Nec mynec; // Illegal, compiler'ın Nec'ın içeriğine erişebilmesi lazım. Çünkü mynec'in memory'de kaplayacağı yeri bilmesi gerekiyor.
Nec * foo(); //OK
Nec * p= foo(); //OK
*p -> x // Illegal
*p // Illegal
sizeof(Nec) // Illegal    
```

### ODR'ı Bozmayan Yapılar

#### Classes
Class iki farklı şekilde ifade edilir.
* Forwad Decleration
* Class Definition (ODR'ye göre bir bileşenin sadece 1 definition'ı olabilir.)

```cpp
class Nec {
    // data members; static or non-static data members
    // member functions; static or non-static members functions
    // nested types

    int a; // a variable'ı class scope'tadır.        
};
```

Not: 
```cpp
class Myclass {
    int a,b;
    double m1, m2;
}
// Bu class definition'ı gören compiler, member'lar için memory'de bir yer ayırmaz. Bu sadece bir tanımlamadır.
// Diğer source code'larda bu class'ı kullanmak isterse class definition header'da yapılır ve #include edildikçe bu header class olduğu için ODR ihlal edilmez. #include aslında header dosyası bileşenlerinin include edildiği source code'a yapıştırılmasıdır. 
```
* ODR ihal edilirse code ill formed olur. ODR'nin class'larda çalışması için class definition'ının isminin token by token("birebir")  aynı olması gerekir.
```cpp
"header1.h"
class Myclass {
    int a,b;
}    
```

```cpp
"header2.h"
class Myclass {
    int a;
}    
```

İki class definition birebir aynı değildir. Ill formed bir code vardır. ODR ihlal edilmiştir.

##### Access Control (Public - Private - Protected)
C++'da compiler öncelikle gördüğü isme name lookup yapar. Bu ismin hangi data structure'a ait olduğunu belirlerse access control'e bakıyor.  Access control'den de geçerse erişim hata vermeden devam ediyor.

 1. Name lookup
 2. Context Control
 3. Access Control

public: Sınıfı kullanacak client'lara erişim açık.
private: Client'lara tamamen kapalı. Sadece sınıf için. (Internal)
protected: Sadece bu sınıftan türeyenlerin ve sınıfın kendisinin internal kullanımına izin var. Client'lara kapalı.

Not:
```cpp
class Myclass
{
    int x; // Default olarak private
}

class Myclass
{
    int x; // Default olarak public
}
```

##### Non-Static Data Members

```cpp
class Myclass{
    int x; // Non-static olduğu için her myclass objesinin ayrı bir x'i vardır. 
}

sizeof(Myclass) -> 4;
```
1. Class Definitions

"ahmet.h"

```cpp
class Myclass {
    void func() // Implicitly inline functions
    {
        // code
    }        
}
```
Not: Bir header'da class definition'ı içerisinde func definition yapılırsa bu func için inline expansion uygulanabilir olur.

* Inline Variables (C++17 ile geldi)

```cpp
inline int a = 10;
```
Header dosyasında global scope'ta böyle bir tanımlama yaparsak ODR bozulmaz. Bu header'ı inlclude eden Source'lar için tek bir "a" değişkeni oluşturur.

#### Constexpr

```cpp
int foo();
int main()
{
    const int x= 10; // Sağ tarafın compiler tarafından constant olduğu açıkca görülmekte bu yüzden x constant expression'dır.
    int a[x]{}; //Legal, Diziler boyut olarak sadece constant expression alabilir.
    const int y= foo(); // foo()'nun return değerinin constant olduğu derleme aşamasında belli olmaz.
    
    int b[y]{}; // Syntax error. Çünkü y constant expression değildir.
}    
```

Bu ihtiyaçdan dolayı constexpr ortaya çıkmıştır. constexpr ile tanımlanan değişkenlere ilk verilen değerin sabit olması zorunludur. Uyulmazsa initialization'da compiler hata verir.

```cpp
constexpr int x= 20;
decltype(x) -> "const int"
    
int g{};
constexpr int *p= &g;
decltype(p) -> "int * const"
// Çünkü constexpr değişkenlerin kendisinin const'lığını belirtir. Burada da bu pointer'dır. Yani; const ptr to int
constexpr const int * p= &g;
// const ptr to const int    
```

Not: Compiler constexpr veya const variable'lar için memory'de bir yer oluşturmuyor. Taki memory adresiyle ilgili bir işlem yapana kadar. (& operatörü ile erişim gibi)


2. constexpr Functions

Return value'su compile time'da belli olan fonksiyolardır.
Yani, constexpr ile compiler'a bu fonksiyonun değerini compile time'da hesapla run time'a bırakma diyoruz.

```cpp
constexpr int sumSquare(int a, int b)
{
    return a*a + b*b;
}    
```

Constexpr Function Limitation;
* İçerisinde static local değişken tanımlanamaz.
* Virtual olamaz.
* Return type'ı literal type olmalıdır. (tam sayılar ve reel sayılar uygun, fakat custom class'lar olmaz.)
* constexpr eğer tüm parametrelere const expressionlar gönderilirse çalışır. (compile time'da değerleri belli olan argumanlar). Yoksa runtime'da çalışır ve hata vermez.

```cpp
constexpr auto val= sumSquare(10, 20);
// Return değerinin constant olması garanti altında. Çünkü return value'su constexptr variable'a atanmış. Eğer constexpr bir return üretmezse hata verir.

constexpr int primes []= {2, 3, 5, 7, 11, 19}
constexpr auto val = primes[4]; // 4. index'deki prime sayının const olduğu garanti altında.
```

Not: constexpr fonksiyonların header'da inline func gibi tanımlanması lazım. Çünkü compiler'ın compiler time'da fonksiyon kodunu görmesi lazım.

