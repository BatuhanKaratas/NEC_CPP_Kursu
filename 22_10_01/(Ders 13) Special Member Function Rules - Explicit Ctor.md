###### Special Member Function Rules

Not: Primitive türlerin default initialize edilmesi garbage value'dur.
Class türlerin default initialize edilmesinde ise default ctor çağrılır. Garbage value oluşmaz.

Not: 12.ders String class'ının implementasyonuna ek;

```cpp
~String()
{
    if(mp)
        std::free(mp);
}

// Move ctor
String (String && other) : mlen{other.mlen}, mp{other.mp}
// Int(Primitive türler) bir member'ın move edilmesi ile copy edilmesi arasında bir fark yoktur.
// Class objectlerinin move ve copy edilmesi arasında fark var. 
{
    other.mlen= 0;
    other.mp= nullptr;
    // Other'ın destructor tetiklenecek şekilde bırakırız.
}

// Move assignment
String & operator=(String && other)
{
    free(mp);
    mlen= other.mlen;
    mp= other.mp;
    other.mlen= 0;
    other.mp= nullptr;
    return this*;
}
```

```cpp
main

int main()
{
    std::string s(10000, 't');
    func(s);
}

void func(const std::String &s) // 1 Function overloading
{
    std::string str{s}; // copy ctor çalışır.
}

void func(std::string && s) // 2 Function overloading
{
    std::string str{std::move(s)}; // string move ctor çalışır.
    // L value to R value conversion
}

Myclass x;
const Myclass y;

func(std::move(x)); //2 R value'lu func overload'u çağrılır.
func(std::move(y)); //1 L value'lu func overload'u çağrılır. Çünkü const nesneleri R value'ya dönüştürdüğümüzde mevcut func overload'ları içerisinde func resolution'a en uygunu const T & olur. Çünkü const qualifier'lığı kaybolmaz. Ayrıca const bir nesneyi R value'ya çevirmek mantıklı değil ve tercih edilmez. Çünkü R value değişim için kullanılır.

void func(const Myclass &&) // 3 nolu overload
{
    //Code
}

Not: Eğer 3' nolu overload'u tanımlarsak const R value call direk 3'e gider. Fakat tercih edilen bir yöntem değildir.

```

###### Kural 1:
Eğer bir class'a hiçbir special member function yazmazsanız, compiler 6 tane special fonksiyonun hepsini kendisi decleared-defined(default) eder.

###### Kural 2:
Eğer sınıfa parametreli bir ctor yazarsak, default ctor not decleared olur, diğer 5 fonksiyon default edilir.

###### Kural 3:
Eğer sadece default ctor user decleared ise, compiler diğer 5 fonksiyonu default eder.

##### Kural 4: (Sınıfı bu durumda bırakmak önerilmez!)
Eğer sadece destructor user decleared ise;

Compiler  default ctor,
                 copy ctor,
                 copy assignment'ı default eder.
                 move ctor not decleared,
                 move assignment not decleared olur.

###### Kural 5: (Sınıfı bu durumda bırakmak önerilmez!)
Sadece copy ctor user decleared ise,
herhangi bir ctor'u user tanımladığı için;

default ctor not decleared,
destructor default
copy assignment default,
move assignment not decleared,
move ctor not decleared olur.

###### Kural 6: (Sınıfı bu durumda bırakmak önerilmez!)
Sadece copy assignment user decleared ise,

default ctor default
destructor default
copy ctor default
move ctor not decleared,
move assignment not decleared olur.

###### Kural 7:
Sadece move ctor user decleared ise,
default ctor not decleared,
destructor defaulted
copy ctor deleted
copy assignment deleted
move assignment not decleared olur.

###### Kural 8:
Sadece move assignment user decleared ise,

default ctor default
destructor default
copy ctor deleted
copy assignment deleted
move ctor not decleared

Not: Bkz; Special Member Functions Table

Notlar:
* Destructor'un "not decleared" olduğu bir durum yoktur.
* Default ctor ya var yada "not decleared"'dir.
* Destructor/copy ctor/copy assignment function'lardan biri bildirilirse move member'lar "not decleared" olacaktır.
* Move ctor veya move assignment function'lardan biri bildirilirse copy member'lar "deleted" olur.


* Bir sınıfı kopyalamaya ve taşımaya nasıl kapatırız?

Copy ctor ve copy assignment delete edilirse move member'lar undecleared olduğu için ayrıca delete yazmaya gerek olmaz. Böylece sınıf kopyalamaya ve taşımaya kapanmış olur.

* Bir sınıfı taşımaya açık, kopyalamaya nasıl kapatırız?
Copy member'lar deleted.
Move member'lar ise user decleared olmalıdır. (Örnek; unique_pointer)

* Bir sınıfı kopyalamaya açık, taşımaya kapalı nasıl yaparız?
Copy member'lar user decleared.
Move member'lar delete edilmemelidir. Çünkü bu durumda R value call'larda kopyalamaya girecekken syntax hatası verir. Zaten copy member'lar decleared olunca move member'lar undecleared olur.

###### Delegating Ctor

```cpp
class Myclass {
  public:
    Myclass(int x);
    Myclass(const char *p);
    Myclass(int, int, int)
  private:
    init();
// Constructor'larda ortak initialization işlerini private init() fonksiyonu yazarak yapabiliriz. Fakat bu durumun bazı dezavantajları vardır.
}
```

* Init fonksiyonu ctor bloğu içinde call edildiğinde aslında bütün class member'ları init edilmiştir. Bir kez daha init olmaz assignment olur. Eficiency ve semantik anlam karmaşası oluşur. Bu yüzden deletagin ctor yapısı ortaya çıkmıştır.

- (1)Myclass(int x): Myclass(x, 0, 0) {}
- (2)Myclass(const char *p): Myclass(std::atoi(p), 0, 0) {}
- (3)Myclass(int, int, int) {}

* 1 ve 2 nolur ctor'lar 3. ctor'a delegate oldu. Bu noktada başka initialize 1. ve 2. ctor'lara eklenemez.

Not: Copy assignment ve move assignment fonksiyonlarını yazarken self assignment kontrollerini yapın.

```cpp
if(this == &other)
    return *this;
```

###### Temporary Objects
* Geçici ömürlü nesnelerdir.

Myclass{} R value category.

const Myclass & r= Myclass{};
Myclass && rr= Myclass{1, 3};

```cpp
void func(const Myclass &); // 1
void func(Myclass &&); // 2

func(Myclass{}) //2 çağrılır.
```

* Geçici nesnelerin bulunduğu ifadelerin yürütülmesinden sonra geçiçi nesne yok edilir.

```cpp
Myclass{}.foo();
// Bu ifadeden sonra geçici nesne yok olmuştur.
```

```cpp
void func(const std::vector<int> & vec)
{
    
}

class Counter {
    public: 
    Counter(int);
};

int main()
{
    using namespace std:
    vector<int> ivec(10, 5);
    func(ivec); // Bu noktada scope leakage vardır.
    func(vector<int>(10, 5)); // Bu ifadede temporary object(R value) kullanarak bunu düzeltebiliriz.
    func(std::string{"alican"}); // temporary object(R value)
    func(Counter{5}); // temporary object(R value)
```

###### Life Extension of Temp. Obj.
Eğer bir temporary object'i R value bir değişkene assign edersek bu işlemden sonra bile temporary object'in hayatı devam eder.

Örnek;
```cpp
{
    Myclass && r= Myclass{3, 5}; // or const Myclass & r'da olabilir.
    r.foo(); // Legal, R scope'dan çıkınca temp object'de yok olur.
}
```

###### Reference Qualifier
Myclass{}.foo(); // Legal

* Bir sınıfın non-static üye fonksiyonlarıİ

a- Value category'si L olan sınıf nesneleri ile çağrılabilir.
b- Value category R olan sınıf nesneleri ile de çağrılabilir.

Fakat bazı durumlarda fonksiyonların sadece L veya R value ile çağrılmasını isteyebiliriz. Bunun için reference qualifiers kullanılır.

```cpp
void func() &; // Sadece L value category nesneler call edebilir.
void foo() &&; // Sadece R value category nesneler call edebilir.
```

```cpp
Myclass m;
m.foo(); // Invalid
std::move(m).func(); // Invalid
```

###### Reference Qualifier Overloads
```cpp
void func() &; // 1 L value ref
void func() &&;// 2 R value ref
void func() const &; // 3 const L value ref// Hem L hem R çağırabilir.
void func const &&; // 4 const R value ref
```

```cpp
Myclass m;
m.func(); // 1

const Myclass cm;
cm.func(); // 3

Myclass{}.func(); // 2
std::move(cm).func(); // 4
```

Not:

```cpp
void func();
void func() &; // Invalid, Bu overload geçersizdir. func'ın tğm overload'ları ya reference qualifier içerecek yada hiç içermeyecek. 
```

###### Conversion Constructors
Sınıf türünden olmayan bir ifade sınıfın conversion constructorlarının kullanılması ile sınıf türüne dönüştürülebilir. 

```cpp
class Myclass {
    public:
    Myclass() = default;
    Myclass(int); // Sınıfın conversion ctor'u
    Myclass & operator=(const Myclass &); // Copy assignment
}

main

Myclass m;
m=12; // Açılımı -> m = Myclass(12); Implicit conversion, Temporary object conversion ctor çağrılır. m'e assign olduktan sonra yok olur. 
```

* Fakat bu özellik bir çok riski beraberinde getiriyor;

```cpp
Myclass(int);
void func(Myclass);
void foo(const Myclass &);
int mi{2};
Myclass m;
func(mi); 
foo(mi); // Func ve Foo call'ları valid'dir. Farkında olmadan mi integer değeri ilie Myclass oluşturulabilir.  
```

Conversion ctor'da kullanılan dönüşüm user defined conversion'dır.

```cpp
Myclass(int x); // Conversion ctor
Myclass(bool); // Conversion ctor

Myclass m;
double dval{};

m= dval;
m= 10 > 5; // m assignment'ların ikiside valid'dir. 
// Açılımı: static_cast<Myclass>(static_cast<int>(dval))
// static_cast<int>: Standard conversion
// static_cast<Myclass>: User defined conversion
```

```cpp
int x;
int * ptr= &x;
m= ptr; // Valid, ptr to bool (Standard conversion)
// bool to Myclass (User defined conversion)
```

```cpp
class A {
    public:
}

class B {
    public:
    B(A); // A'dan B'ye implicit conversion var.
}

class C {
    public:
    C(B); // B'den C'ye implicit conversion var.
}

int main()
{
    A ax;
    C cx = ax; // Invalid. Çünkü dil iki tane implicit user defined conversion'a izin vermez.
    
    C cx= static_cast<B>(ax); // Valid
}

```


###### Explicit (Explicit only implicit banned)
Bu keyword ctor'lar için kullanılır. Amacı implicit conversion'lara izin vermemektedir.

```cpp
explicit Myclass(int);
```

```cpp
Myclass m;
m= 5; // Invalid

Myclass foo()
{
    return 2; // Invalid
}

void func(Myclass);
func(45); // Invalid
```

Örnek;
```cpp
std::vector<int> ivec(100); // Legal
std::vector<int> ivec= 100; // Illegal
Myclass x(10); // Legal Explicit bu initialization için çalışmıyor.
Myclass x= 10; // Illegal explicit çalışır.
std::unique_ptr<int> uptr{new int}; // Legal
std::unique_ptr<int> uptr= new int; // Illegal
```

```cpp
explicit Myclass(int) (1)
{
    
}

explicit Myclass(double)(2)
{
    
}

Myclass x= 12; // Explicit ctor viable func statüsünden çıkarıldı. 2 çağrılır.

Myclass y(15); // 1 çağrılır.

explicit Myclass(int);

int ival{50};
Myclass m= static_cast<Myclass>(ival); // Legal
Myclass m= (Myclass) ival; // Legal. Çünkü explicit conversion vardır.


explicit Myclass(int, int);

Myclass m= {12, 67}; // Illegal
Myclass n{12,67}; // Legal
```
