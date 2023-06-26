### Special Member Functions
```cpp
class Nec{
public:
    void set(int); // Hidden parameter Nec *
    int get() const; // Hidden parameter const Nec * 
    // get() fonksiyonu nesnenin değerini değiştiremeyecek. (Observable State)
};
```
Örnek: 
```cpp
int globalInt1{10};
int globalInt2{20};

class Bat {
public:
    void setMem() const
    {
        *mem= 15; // Legal pointer'ın point ettiği değer değiştirilebilir.
        mem= &globalInt2; // Illegal pointer değiştirilmeye çalışıldı.
        m_Debug_Count= 10; // Legal
    }    
    
    int * mem{&globalInt};
    mutable int m_Debug_Count{}; // Nesnenin observale state dışındaki değişkenlerde mutable kullanabiliriz.
    
    mutable int m_Crashed_Value; // Nesnenin observable-state'i dışında
    // Sınıfın const fonksiyonları içerisinde bu değişkenin değişmesi legal'dir.
    // Const nesnelere ait bu değişkenin değişmesi legaldir.
}

//Örnek;
class Nec {
    public:
    mutable int mx{};
};

int main()
{
    const Nec mynec;
    mynec.mx= 5; // Legal
}    
```
##### Constructor ve Destructor Special Member Functionları
##### Constructor
* Bir sınıfın nesnesinin non-static data member'larının initialize edilmesi anlamına gelir.
* C++'da her sınıfn nesnesi bir ctor ile oluşturulur.
##### Destructor
* Bir nesnenin lifespan'ini bitiren fonksiyondur.

##### Constructor (ctor)
* Sınıfın non-static üye fonksiyonu. 
* Static member function olamaz.
* Geri dönüş değeri olmaz.
* İsmi sınıf ismiyle aynı
* Global function olamaz
* Const member fonksiyon olamaz.
* Overload edilebilir.
* Sınıfın public üye fonksiyonu olmak zorunda değil.
* . veya -> operatörleri ile bir sınıf nesnesi tarafından çağrılamaz.

##### Destructor(dtor)
* Sınıfın non-static üye fonksiyonu
* Global fonksiyon olamaz.
* Static member function olamaz.
* Geri dönüş değeri olamaz.
* Const üye fonksiyon olamaz.
* İsmi sınıf ismi ile aynı olmak zorunda. (~ClassName())
* Parametre değişkeni olamaz.
* Overload edilemez.
* Sınıfın public üye fonksiyonu olmak zorunda değil. (private-protected olabilir.)
* . veya -> operatörleri ile bir sınıf nesnesi tarafından çağrılabilir.

```cpp
Myclass m;
m.~MyClass(); // Legal fakat kullanılmaması lazım.
```

##### Default Constructor
* Parametre değişkeni olmayan yada tüm parametre değişkenleri varsayılan argüman olmalı

```cpp
Myclass();
MyClass(int = 0);
```
Not: Argüman gönderilmeden çağrılabilen ctor'dur.

##### Special Member Function

* Bu fonksiyonların kodları (definition)'ları belirli koşullar sağlandığında derleyici tarafından yazılabiliyor. (Compiler tarafından default edilebilir.)

Compiler tarafından default edilmek nedir?
* Dilin kurallarına göre(implicitly) bu fonksiyonları bildirilebilir ve bizim için bu fonksiyonların kodları yazılabilir.
* Programcı compiler'dan bu fonksiyonların kodu yazmasını talep edebilir.

6 tane special member function vardır.

* Default ctor
* Destructor
* Copy Ctor
* Move Ctor (C++11'den itibaren geldi)
* Copy Assignment
* Move Assignment (C++11'den itibaren geldi)

Not: Global sınıf nesneleri için ctor main başlamadan önce dtor is main bittikten sonra çağrılır.

###### Constructor - Destructor Sırası

```cpp
// x.cpp'de bulunan global myclass objeleri(aynı source file'da bulunurlar)
Myclass g1;
Myclass g2;
Myglass g3;

// Construct sırası; g1-g2-g3
// Destruct sırası; g3-g2-g1
```

Not: Dikkat!!! farklı kaynak dosyalarda tanımlanmış global sınıf nesnelerinin hayata gelme sırası belli değil. (Static initialization fiasco)

##### Static Local Object
```cpp
void foo()
{
    static Myclass c;
}

int main()
{
    foo();
    foo();
}

// Static c objesi foo() çağrılırsa ctor'u call eder ve main'den çıktıktan sonra dtor'u call edilir. foo() çağrılmazsa ctor-dtor çalışmaz.
```

##### Local Object(Automatic Scope Object)
```cpp
for(int i{0}; i < 10; i++)
{
    Myclass m;
}

Index: 0 ctor - dtor
Index: 1 ctor - dtor
Index: 2 ctor - dtor
.
.
.
```
* Statik ömürlü nesneler (Data segment section'ınında tutulur)
* Otomatik ömürlü nesneler (Stack segment'inde tutulur)
    * Stack pointer stack'in cursor'ıdır. Yeni local nesne geldikçe ilerletilir. Destruct oldukça geriye çekilir. Linear çalışır. 
 
* Dinamik ömürlü nesneler (Heap segment'inde tutulur.)

Örnek; void * operator new(std::size_t)

* Dinamik Ömürlü Nesnenin Oluşumu
```cpp
auto new Fighter -> ((Fighter *) operator new(sizeof(Fighter)))->ctor
```
* Dinamik Ömürlü Nesnenin Hayatının Bitmesi
```cpp
delete p; -> p->~Fighter();
```
     * Allocate edilen alan bellek alanına geri verilir. 

Ara not:

C++'da pointer-like class'lar;

- Smart Pointer: Dinamik ömürlü nesnelerin hayatını kontrol etmek
- Iterator: Array'ler üzerinde iterate etmek. (Data Structures)

Exercises:

```cpp
{
    Nec x;
    Nec &r1= x;
    Nec &r2= x;
    Nec &r3= x;
}
// Bu noktada 1 ctor ve 1 dtor call edilir. (x için)

{
    Nec x;
    Nec *p= &x;
}
// Bu noktada 1 ctor ve 1 dtor call edilir. (x için)
```

###### Default Initialization
```cpp
Nec x;
Nec y{};

// Primitive type'larda olduğu gibi garbage value oluşmaz. Default ctor value initialization her ikisinde de çağrılır.

Nec x(); 
// Burada ise Scoot Meyer'ın must vexing parse'ı çalışır ve compiler bunu bir function decleration olarak kabul eder.
```

Dikkat:
* Bir sınıfın default constructor'ı olmak zorunda değil. (Not decleared olabilir)

```cpp
class Nec {
    public:
    Nec(int); // Bu durumda compiler implicitly olarak default ctor'u decleared etmez.
}
```

###### Special Member Function Status
* Not decleared
* User decleared
    * User declared-defined - Nec()
    * User decleared- defaulted - Nec() = default
    * User decleared - deleted  - Nec() = delete
* Implicitly Decleared
    * Implicitly decleared-defaulted
    * Implicitly decleared-deleted
    
Ara Not:
Defaulted functions:
Sadece special functionlar defaul edilebilir.
Deleted functions:
Fonksiyon var ama bu fonksiyona çağrı illegal. Tüm fonksiyonlar delete edilebilir.

```cpp
void func(int) = delete; 
// Delete edilen fonksiyonun sadece decleration'ı olur. Definition'ı olmaz.

int main()
{
    func(12); // Syntax error
}

// Delete edilmiş bir fonksiyon function overload resolution'da viable bir fonksiyondur. Fakat seçilirse ve call edilmeye çalışılırsa syntax error oluşur.

void func(int) = delete;
void func(float);
void func(double); 
// func'ın 3 overload'ı vardır.
```

```cpp
class Myclass 
{
    public:
        Myclass(int x)
        {
            // ....
        }
        
        Myclass(int x, int y)
        {
            // ....
        }
    // Bu durumda default ctor not decleared olur.
};
```

###### Initialization

```cpp
int main()
{
    Myclass x1(10); // Direct Init
    Myclass x2{20}; // Direct List Init
    Myclass x3= 92; // Copy initialization
    Myclass x4(4, 5);
    Myclass x5{6, 7};
    Myclass x6{9, 12};
}
```

###### Ctor Initializer List
a-) Yalnızca ctor'lar için kullanılabilir.

```cpp
class Myclass{
    public:
    Myclass() : m_x(10), m_y(20), m_z(9.8)
                 or        or        or
                m_x{10}, m_y{20}, m_z{9.8}
    {
        // Code
    }
    
    int m_x, m_y;
    double m_z;
}

// Bu class için primitive member'lar Initializer list ile initialize etmezsek garbage value olur.
```
b) Hayata getireceği sınıf nesnesinin m_static veri elemanlarını initialize eder.
Not: 1. tercihimiz her zaman ctor init list olmalıdır.

Eğer ctor initializer list kullanmazsakİ

```cpp
class Myclass {
    public:
    Myclass()
    {
        m_address = "Ankara"; (Assignment)
    }
    private:
    std::string m_address;
};

// Myclass nesnesi default ctor ile oluşturulurken;
//- String class'ının default ctor'u çağrılacak.
//- String class'ının copy assignment'ı çalışacak.
//- Verim düşecek, Bu yüzden ctor initializer list kullanmak mantıklıdır.
```

Önemli Not:

* Her zaman veri elemanlarının hayata gelme sırası class definition sırasıdır. Bu yüzden definition sırasını koruyarak initialize etmeliyiz.

```cpp
class Myclass {
    public:
    Myclass() : m_y{20}, m_x{10} // Legal Fakat önce mx sonra my initialize olur. Esas sıra class definition sırasıdır.
    {
        
    }
    private:
    int m_x;
    int m_y;
};
```

Direct List Init Example;

Date.h

```cpp
class Date{
    public:
    Data(int day, int mon, int year);
    private:
    int mday;
    int mmon;
    int myear;
}
```

Date.cpp

```cpp
Date::Date(int day, int mon, int year) :
mday{day}, mmon{mon}, myear{year}
{
    // Code
}

int main()
{
    Date today{24, 9, 2022};
}
```

```cpp
int g{};
class Myclass 
{
   public:
    Myclass(): r{&g} // Legal, reference initialize oldu. 
              ,c{5} // Legal, constant var. Initialize oldu.   
    {
        // Code
    };
    private:
    int &r; // Default initialization şart
    const int c; // Default initialization yapılması şart
}
```
