# Default Arguments
Not: Fonksiyonların çok fazla parametreye sahip olması iyi bir pratik değildir. Hatalara sebebiyet verir ve okunabilirliği düşürür.

Not: Yeni bir yapı entegre ettiğimizde ilk bakmamız gereken run time maliyetidir.

* void func ( int, int, int = 10);
* 
```cpp
int x{5};
void f(int = x); // Legal Global default value

int f1(int x=0);
void f2(int y= f1()); // Legal

int x= 10;
void func(int * ptr= &x) // Legal

void func(int x, int y=x) // Illegal    
```

Not: Default argument ya function signature'a (recommended) yazılmalıdır veya function definition'da yazılmalıdır. İkisinde de kullanılırsa syntax error verir.

* 
```cpp
int g{20};
func(int = g++); // Unevaluated değildir. Her default argument call'da global g değeri artar.
```

"Emrah.h"

func(int, int, int);

"Emrah.cpp"
```cpp
#include "emrah.h"

void func(int, int, int= 10)
// Valid, Çünkü emrah.h'dan alınan func decleration ile source file'da bulunan func decleration kümülatif birleşir. Yani başka bir header'dan çektiğimiz func fonksiyonunu source dosyasında default argument'li bir şekilde tekrar decleration yaparsak son hali geçerli olur. (Redecleration)
    
main()
{
    func(5, 10); // Valid    
}    

// Eğer redecleration yöntemi uygun olmazsa;
// Wrapper function yazılarak bu sorun çözülebilir.

void wrapperFunc(int x, int z, int y=10)
{
    func(x, y, z); // 2. parametre wrapper func ile defaultlandı. 
}
```

```cpp
void func(int x, int * ptr= nullptr)
{
    if(!ptr) // Default argument kullanılmışsa bu if'e girecek.
    { }    
}    
```

Not: Best practise tavsiyesi: CppCoreGuidelines (Bjarne Strostrup ve Herb Sutter)

# Scoped Enumarations
Traditional enum'ların dezavantajları nelerdir?

* Underlying type'ını(enum bileşenlerin type'ı) compiler belirler.
    * Default type  int'dir. Örnek; Eğer int'e sığmayan bir bileşen varsa type double'a çekilir.

* 
```cpp
enum Nec {on, off};
enum Color {white, black};
Nec necx;
Color myColor;
necx = black; // Illegal Farklı enum türleri arasında dönüşüm yok.
necx= 2; // Illegal Aritmetic to enum dönüşümü yok.
int ival= black; // Legal Enum to aritmetik dönüşüm var. Bu dünüşümün olması sorundur. 
```

* Enum bileşenlerinin geçerli olduğu heryerde kullanılabilir olması.
Örneğin;
```cpp
// traffic.h
enum traficlight { red, yellow};

// screen.h
enum screencolor { red, white};

// Iki header'da include edildiğinde scope'ları ortaklaşır. 
// Red'ler için syntax error oluşur.
``` 

Modern Enum 

```cpp
enum Color : unsigned long long { gray, black};
// Underlying type developer tarafından belirlenebilir.
```

Scoped Enum
Scoped enum traditional enum'un dezavantajlarını yok eder.

```cpp
enum class Color : unsigned int {white, gray, black};
// Underlying type belirtilmezse default int'dir.

Color myColor= Color::white; 
// Enum class adı belirtelerek ancak bileşenlere ulaşılır.
// Eğer enum class ismini namespace olarak uzun uzun yazmak istemezsek, // C++ 20 ile gelen "using enum ScreenColor" ifadesini kullanabiliriz. 
using enum ScreenColor
    
auto c1= ScreenColor::red; // yerine (gerekli değil)
auto c1= red; // kullanılabilir. 

int x;
ScreenColor sc{ScreenColor::black};
x = sc; // Illegal, Enum to arithmetic type dönüşümü yoktur. 
```
