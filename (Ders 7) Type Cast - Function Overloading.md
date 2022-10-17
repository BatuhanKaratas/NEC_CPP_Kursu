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
C'de function overloading yoktur. Bunun sebebi C'nin küçük ve yalın bir dil olması ekstra kontrollerde bulunmak istemiyor. Yükü programcıya yüklüyor.

Function overloading compile time'a ekstra bir yük getirir. Run time'a herhangi bir eksta yük getirmez.

* Function overloading (Static Binding - Compile Time)
* Function overriding (Dynamic Binding - Run Time)

Hangi durumlarda overloading oluşur?

* Aynı isimli
* Aynı scope'da
    * Namespace scope
    * Class scope
    * Block scope
    * Function scope
    * Function prototype olacak

* Function signature'ın farklı olması 
Not: Function signature: Parametre değişkenlerinin sayısı ve her birinin türü'dür. Return type'ı dahil değildir.

Örnek:
* int foo(int)
    int foo(int, int= 0) // Function overloading

* int foo(int);
    int foo(const int) // Function redecleration. "const int" ve "int" farklı türler olarak değerlendirilmez. 
    
  * int foo(int *)
      int foo(const int *) // Funtion overloading. "const int *" ve "int *" farklı türler kabul edilir. Çünkü burada point edilen değerin status'ü değişmektedir.
      
   * void f (double * const p);
       void f(double * p); // Function redecleration. "double * const" ve "double *" farklı bir tür anlamına gelmez. Çünkü burada pointer const'dur.
       
   * void func(int &)
       void func(const int &) // Function overloading. "const int &" ve "int &" farklı türlerdir. Çünkü referans oldukları değerin status'ü farklıdır.
       
   * typedef double flt_type;
       void f(double);
       void f(flt_type); // Function redecleration. Syntax hatası yok.
       
   * void f(char) // Bu parametre türünün işareti (+ or -) compiler'a göre değişiklik gösterebilir.
       void f(signed char)
       void f(unsigned char) // 3 adet overloading vardır. Hepsi farklı tür olarak değerlendirilir.
       
   * void f(int *);
      void f(int * *);
      void f(int * * *); // Herbiri farklı bir tür olarak değerlendirilir. 3 adet overloading vardır.
      
  * void f (int &); // Left reference
      void f(int &&);  // Right reference, Farklı türlerdir. Function overloading vardır.
      
  * void f (int &);
      void f(const int &);
      void f(int &&); // 3 adet overloading vardır.
      
    * void f(std::int32_t);
        void f(int); // Compiler definition'a bağlı.  int 4 byte ise overload yok.
        // 4 byten farklı ise overload vardır.
        
      * void f(int);
          void f(int *);
          void f(int &); // Function overloading.
          
       * void f(int p[ ]); // Array decay gerçekleşir.
           void f(int p [20]);
           void f(int *p); // Aslında hepsi aynı türdür. Çünkü C++'da fonksiyonların parametreleri  ve return type'ı aslında array ve fonksiyon olamaz. Bunları temsil eden pointer'lardır. (Array decay - Function decay)
           Bu yüzden burada funtion redecleration vardır. 
         
         * void foo (int (int)) // Function pointer'a conversion yapılır.
             void foo (int (*) (int));  Aynı türlerdir. Bu yüzden function redecleration vardır.
             
         * void foo(int (*) [5]); // 5 elemanlı array pointer
             void foo(int (*) [6]); // 6 elemanlı array pointer
             // İkisi farklı tür kabul edilir. Çünkü burada array decay yoktur direk pointer'ın kendisi ifade edilmiştir. Bu yüzden burada overloading vardır.
             int a5[5];
             int(*p1) [5]= &a5; 
             
## Function Overload Resolution
Compiler'ın uygun function bulma aşamaları;

1- Candidate function bulma. Sadece scope'daki functionların sadece isimleri uyuşanları buluyor. (Parametrelere bakılmıyor)

2- Viable function bulma. Candidate functionlar arasında callable(compiler error vermeyen) olan fonksiyon bulunur. (Parametrelere bakılır.)

Örnekler: 
```cpp
void func(bool);

int x{};
func(&x); // Pointer türü. Pointer türünün bool'a implicit dönüşüm var. // nullptr= false, others= true; Bu yüzden func viable func'tır.
//------------------------------------------------------------------
void func(int *);

int x{};
double f{};
void * up{};

func(&x); // Viable
func(&f); // Compile time error. Not viable. double *'dan int *'a implicit dönüşüm yoktur. Aynı primitive typler arasında ise implicit dönüşüm var.
func(up); // Compile time error. Not viable. int * to void * dönüşümü yok.
    
//--------------------------------------------------------------------
void func(const int *);

int x{};
func(&x); // Viable. int * to const int * implicit conversion var. 

//--------------------------------------------------------------------
void func(int *);

const int x{};
func(&x); // Not viable. const int * to int * implicit dönüşümü yok.

//--------------------------------------------------------------------
void func(char *);

func("Mursit Koc"); // Not viable. const char * to char * implicit dönüşümü yok.

//--------------------------------------------------------------------
enum pos {on, off, hold};
enum class color{yellow, blue};

void f(pos);
void g(color);

f(1); // int to enum dönüşümü yoktur. Not viable.
g(1); // int to enum dönüşümü yoktur. Not viable.
f(color::yellow); // farklı enum türleri arasında dönüşüm yoktur.
g(off); // farklı enum türleri arasında dönüşüm yoktur.

//--------------------------------------------------------------------
enum pos {on, off, hold};
enum class color{yellow, blue};

void f(int);

f(color::yellow); // enum class to int dönüşümü yoktur. Not viable
f(off); // enum to int dönüşümü yoktur. Not viable
//--------------------------------------------------------------------

void f(int *&); // L value reference (int * refer eden bir referans)

int x{10};
f(&x); // PR value expression. Not viable.
```

3-  2. aşama sonucunda en az 2 viable function ihtimali oluşacak ve compiler bunlar arasında en uygununu seçmelidir. En uygunu seçemezse ambiguity for function overload hatasını alırız.

Örnek;
```cpp
void f(int);
void f(double);

f(2u); // Bu durumda 2 adet viable function vardır. Fakat compiler hangi overload fonksiyonu seçeceğine karar veremez. Ambiguity error. 
```

Best match seçim kıstasları;

Not: "…" : Elipsis
* void func(int, int , ...); // Variadic function

Eğer ihtimaller içerisinde variadic function varsa best match olma ihtimali priority olarak **en düşük** varyasyondur.

* User defined conversion

 Best match olma ihtimali priority olarak ikinci en düşük priority'li match'dir.
 
```cpp
class Myclass {
    public:
    Myclass(int); // Conversion ctor
};

void func(Myclass);

func(12); // int to Myclass user-defined implicit dönüşümü gerçekleşir. // Dolasıyla func fonksiyonu user-defined conversion priority'sine sahiptir.
```

* Standart conversion

Best match olma ihtimali priority olarak üçüncü en düşük priority'li match'dir.

```cpp
int to double
int * to void *
nullptr to int *
long double to char
```
Compile time error vermeyen tüm implicit conversion'lar bu dönüşüme dahildir.

* Promotion conversion
Best match olma ihtimali priority olarak dördüncü en düşük priority'li match'dir.

 * Integral Promotion:
     Integer altı türlerin int'e promote edilmesidir. 
     Örneğin: short-char-bool gibi türler int'e promote olur.
 * Float to Double Promotion.

* Exact Match
Best match olma ihtimali priority olarak en yüksek priority'li match'dir.

Argumanla function parametre türü bire bir aynıysa exact match'dir. 

Ayrıca bunun dışında şu conversion'larda exact match'dir:
* T * to const T* dönüşümü
* Array decay to T * dönüşümü (&int[0] to int * )
* Function decay to Func * dönüşümü (func to int (fp *) (int))

```cpp
void f(int (*) (int))
int bar(int);
f(bar); // Function decay to func * dönüşümü gerçekleşir. 
```

Örnek;
```cpp
1 void f(int);
2 void f(double);
3 void f(long);

f(3.4); // 2 exact match
f(12); // 1 exact match
f(12u); // Ambiguity error. 3 f'de de standart conversion gerçekleşir. Bir seçim yapılamadığından error oluşur.

f(true); // Integral promotion, bool to int.
f(2.3L); // Long double. Ambiguity error. 3 f'de de standat conversion gerçekleşir. Bir seçim yapılamadığında error oluşur.

f(2.3f); // Double promotion. 2 promotion match
f('A'); // char to int promotion. 1 integral promotion match.

İstisna kurallar;
*     
1 void func(int *);
2 void func(const int *);

// Bunun adı const overloading'dir. 

int x= 5;
func(&x); // İkiside exact match olmasına rağmen. 1 nolu func seçilir.

* 
1 void fuc(int &);
2 void func(const int &);

int x=5;
func(x); // İkiside exact match olmasına rağmen. 1 nolu func seçilir.
```




