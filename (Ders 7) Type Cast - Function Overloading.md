# Type Cast Operator
Not: Bazen tür dönüşümü yaparak kodu legal bir hale getirsek bile undefined-unspecified behaviour'a sebep olabiliriz.

##### C Dönüşüm Türleri
* 
```cpp
C++ static cast örneği;
i1 / i2; // Gerçek sayılı bir sonuç elde etmek için (double)i1 / i2 explicit type cast yapabiliriz.

ival = dval; // Burada bir implicit dönüşüm gerçekleşir ve son derece sakıncalıdır. 
// Çünkü narrowing conversion oluşur. (Double to Int)
// Ayrıca dval'in tam sayı değeri ival'e sığmayacak kadar büyükse undefined behaviour oluşur.
```
* 
```cpp
C++ const cast örneği;
int main()
{
    int x= 10;
    const int * ptr= &x;
    int * p= (int *) ptr;
    *p = 56;
// Bu örnekte ptr'nin point ettiği obj const değil. Fakat ptr ptr to const int tipinde. Dolayısıyla ptr'yi kullanarak x'i değiştiremeyiz. Bunu başka bir pointerla(p) yapmak istediğimizde "const T *"'ı "T *"'a implicitly dönüştüremeyiz. Bunu yapmak için C-style const cast (int *) yaparak const'lıktan kurtulabiliriz.      
}
```

* 
```cpp
C++ reinterpret cast örneği;
struct Data {
    int a,b,c;
}

int main()
{
    Data mydata {1, 4, 6};
    char *p = (char *) &mydata;
// Farklı türler arası dönüşüm çoğu zaman sakıncalı da olsa bazı durumlarda uygulanması gerekir. Örneğin bir array'in adresini char * ile tutmak gibi. Eğer array'in elemanlarının boyutu 1 byte'ı aşmıyorsa sorun olmaz.        
}
```

C++ tür dönüşüm operatörleri;
* Static Cast
* Const Cast
* ReInterpret Cast
* Dynamic Cast

C++'da ki bu tür dönüşüm operatörleri ile birlikte;
* Code readability artıyor.
* Hata yapma olasılığı düşüyor.
* Code maintability artıyor.
Not: Bu avantajlardan dolayı C++'da C style cast kullanılmamalıdır.'

1. Static Cast
* Enum to Int
* Tam sayı to Reel sayı
* Enum to Enum (Farklı enum türleri arasındaki dönüşümlerde)

2. Const Cast
* const T * to T * işlemlerinde kullanılır.

3. Reinterpret Cast
* Farklı türlerin birbirine dönüştürülmesinde kullanılır.

4. Dynamic Cast
* Inheritance dönüşümlerinde kullanılır.

```cpp
C++ Static Cast Örneği;
enum class Pos {off, on, hold};
int x= 10;
int y= 3;
double dval= static_cast<double>(x) / y;
int ival= static_cast<int>(dval);
Pos mypos= Pos::on;
ival= static_cast<int>(myPos); 
```

```cpp
C++ Const Cast Örneği;
char * mystrchr(const char * p, int c)
{
    while(*p) {
        if(*p == c) {
            return const_cast<char *>(p);
        }
        ++p;    
    }
}    
```

```cpp
C++ Reinterpret Cast Örneği;
double dval= 456.432;
char *p= reinterpret_cast<char *>(&dval);
// Burada 8 byte'lık memory'de tutulan double'ı(dval) bir char diziye tutturup elemanlarına erişmeyi amaçlıyoruz.
```

```cpp
C++ Reinterpret Cast Örneği;
const unsigned int uval {436u};
int *p1= reinterpret_cast<int *>(&uval); //Syntax error. const unsigned int * to int * bu şekilde dönüştürülemez.

int *p1= const_cast<int *>(reinterpret_cast<const int *>(&uval)); // Ok 
// unsigned int'i int *'a dönüştürmek için farklı türlerin dönüşümü için kullanılan (int * vs unsigned int) reinterpret cast kullandık.
// Const'lığı düşürmek için const cast kullandık.
```

# Function Overloading