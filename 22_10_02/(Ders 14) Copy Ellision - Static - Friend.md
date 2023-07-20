#### Copy Ellision

Kopyalamadan kaçınma anlamına gelir.

C++17 ile copy ellision bazı durumlarda mandatory oldu. Bu durumlarda sınıfın copy ctor'unun deleted veya private olması hata oluşturmaz. Çünkü zaten kullanılmıyor.

###### Mandatory Case 1

```cpp
class Myclass {};

void func(Myclass m) {};

main

func(Myclass{});

// C++17 ile;
// 1- Default ctor çağrılır.
// 2- Destructor çağrılır.
// Copy ctor çağrılmadı compiler bu optimizasyonu mandatory olarak gerçekleştirdi.
```

###### Mandatory Case 2 RVO(Return Value Optimization)
```cpp
Myclass foo()
{
    return Myclass{};
}

main

Myclass m= foo();
// C++17 ile;

//1- Default ctor
//2- Destructor çağrılır.
```

* Named RVO(Mandatory optimization değil, bu yüzden sınıfın copy ctor'u delete edilemez.)

```cpp
Myclass foo()
{
    Myclass m; // Automatic scope variable
    
    return m;
}

main
{
    Myclass m= foo();
    // 1- Ctor
    // 2- Dtor
    // Compiler bu kodu normail bir initialization yapılıyomuş gibi çevirir.
    
    // Mandatory optimization olmadığı için en kötü durumda;
    // Compiler m'i x value(PR value)'ya çevirir ve m'e assign ederken move ctor çağrılır. 
}
```

###### Static Class Members
```cpp
class Nec {
    public:
    static int sval; // Sınıfa ait global değişken, sınıf objesinden bağımsızdır.
    int a{}, b{}, c{}; // Sınıf objesine bağlı değişkenler
};

int g;
```

###### Global variable vs Static class variable

Static class variable (static int x)
* Class scope'da
* Access control var (public-private-protected)
* Statik ömürlü (Main çağrılmadan program çalışmaya başladığında hayatta olur, program bitince destruct olur.)
* Static Init Fiasco (Farklı source dosyalarda statik değişkenlerin oluşma sırası belirsiz)
* Inline variable olarak tanımlanabilir.
* Ancak class ismiyle nitelenerek  erişim sağlanabilir. (Name lookup)

```cpp
Myclass::x // Recommed usage

Myclass m1,m2;
m1.x= 10; // Legal fakat önerilmiyor. Code readability açısından olumsuz.
m2.x= 15; // Aynı x değişti. Değer m1 ve m2 içinde 15 olur.
```

Not:
 Myclass.h

```cpp
class Myclass {
    public:
    static int x; // Incomplete type, non-defining decleration
}

//main
{
    Myclass::x= 10; // Compile time'da hata yok. Fakat link time'da hata verir. Çünkü X storage'da allocate edilmedi.
}
```

 Myclass.cpp
 
 ```cpp
#include "myclass.h"

int Myclass::x; //Zero Initialization
int Myclass::x= 10; //Copy Initialization
int Myclass::x(); //Direct Initialization
int Myclass::x{}; //Value Initialization
int Myclass::x{20}; //Initialization List
```

Not: Static member variable bir incomplete type'dır.

```cpp
class Nec; // Forward Decleration

class Myclass {
    private:
    static Nec sne; // Legal, static incomplete type'dır. Source file'da definition'ının yapılması şart.
    static Myclass m; // Legal, static incomplete type'dır.Source ile'da definition'ının yapılması şart.
    Myclass n; // Illegal
}
```
Not: Static member variable'lar sadece source file'da değil class definition'da da(header) define edilebilir. (Herhangi bir fonksiyon içinde olmaması lazım.)

Sadece;
-Const
-Integral Type ise bu işlem yapılabilir.

C++ 17 ile;
* Static member'ın başına Inline keyword'u eklenerek  class definition'da da initialize edilebilir.

Hatırlatma;

Inline variable'ların kullanım amacı;

Header'da global scope'da tanımı yapılan variable'a inline keyword'ü eklemezsek include edildiği her source'da bir kopyası oluşur ve one definition rule ihlal edilmiş olur.

Çünkü include demek header'ı al cpp'ye yapıştır demektir.

```cpp
Header.h
inline static int x=5;

Ahmet.cpp
#include "header.h"

inline static int x= 5;


//Mahmut.cpp
#include "header.h"
inline static int x= 5;

// One definition rule ihlali olmuyor. Tek bir x değişkeni var.
```

```cpp
Header.h
class Myclass 
{
public:
    static int x;
};

int Myclass::x{}; // One Definition Rule ihlal edilir çünkü inline değil.
```

Not: Class static member'lar ctor initializer list'te initialize edilemezler.  Bu member'lar zaten program başlamadan initialize edilmiş olurlar.

###### Static Member Functions
```cpp
class Myclass {
    public:
    void foo(); // Non-static member function. Gizli parametre var.  (Myclass *)
    static void bar(); // Static member function. Gizli parametre yok.
}
```

###### Static Member Function
* Class Scope'da ancak this pointer yok yani bir sınıf nesnesinin adresi ile çağrılmayan sınıfın üye fonksiyonları

Myclass::bar();

* Erişim kontrolüne tabi
* Sınıfın private ögelerine erişebilir.


Not:
**Static ve Non-static fonksiyon definition'larını source file'da ayırt etmek için;**

```cpp
#define STATIC

STATIC void foo();
// Compiler bu define'ı siler. Static function definition ayrımı için kullanılabilir.
```

**Önemli**
```cpp
static void bar(Myclass p)
{
    Myclass x; // Legal
    p.mx= 34; // Legal
    x.mx= 10; // Legal
    mx= 15;  // Ilegal
    // Çünkü static class member function'lar içerisinde hidden parameter(this) yok. Non-static veri elemanlarına bu yüzden erişemez    
}    
```

```cpp
Static void bar() const; // Illegal hidden parametre olmadığı için const static function olmaz.  
```
Not:
```cpp
class Nec {
    public:
    static void foo(int);
}

void bar(int);

// Bar ve Foo fonksiyonlarının function pointer olarak türü tamamen aynı.foo'nun class içerisinde olması fark bir fark yaratmadı. int (fp*) (int)

class Nec
{
    private:
    
    static int foo()
    {
        return 777;
    }
    
    static int x;
};

int Nec::x= foo();
// Class'ın static x'i intialize edilirken class scope aktive olur ve call edilen foo fonksiyonu private olmasına rağmen class scope'da olduğu için erişim sağlanabilir.
```

###### Static Idiom's

* Object Live Count

```cpp
class Myclass {
    public:
    Myclass(const Myclass &)= delete;
    Myclass & operator=(const Myclass &)= delete;
    
    Myclass()
    {
        ++ms_live_count;
    }    
    
    ~Myclass()
    {
        --ms_live_count;
    }
    
    static int getLiveCount()
    {
        return ms_live_count;
    }        
    
    Myclass(int)
    {
        ++ms_live_count;
    }
    
    private:
    static inline int ms_live_count{};
}

main
{
    Myclass m1,m2,m3; // +3
    auto p1= new Myclass{3}; // +1
    auto p2= new Myclass{5}; // +1
    delete p1; // -1
    std::cout << Myclass::getLiveCount();
    
    // Output: 4
}
```
###### Named Constructor Idiom
(Client kodu kısıtlamak amacıyla kullanılır)

- Dinamik ömürlü nesne kullanmaya zorlamak (Otomatik-Static ömürlü nesne oluşumu yasak)

```cpp
class Myclass{
    private:
    Myclass();
    Myclass(int);
    
    public:
    static Myclass * createObject()
    {
        return new Myclass;
    }
    
    void foo();
}

main

Myclass *p= Myclass::createObject();
p->foo();
```

** Mono State Pattern **
```cpp
class Clock 
{
    public:
    static std::time_t now();
    static void adjust();
    static int getMinute();
}; // non-static hiçbir member bulunmuyor.

main

Clock c1;
Clock c2;
Clock c3; 
// C1, C2 ve C3'ün state'leri aynıdır. Çünkü non-static objeye özel hiçbir ifade yoktur.
```
Ekstra Not:

**Design Pattern**

GOF(Gang of Four) Pattern
* Creational Patterns 
    * Builder
    * Singleton
    * Factory vb.
 * Structural Patterns
 * Behavioral Patterns
 
###### Signleton Design Pattern
* Bir sınıf türünden tek bir nesne olacak. O nesneye global erişim sağlanacak.
* Program sonlanmadan önce nesnenin destroy edilmesi gerekiyor mu?
* Thread-safe olması gerekiyor mu?
* Inheritance olacak mı?
* Lazy Init. gerekiyor mu? (Main başlamadan initialization mı? Func çağrıldığında yapılan initialisation mı?)

Singleton Örneği;
```cpp
class Myclass (copy ctor delete edilmeli) 
{
 private:
    inline static Myclass * p_Instance{};
    Myclass();
 public:
    static Myclass * getInstance(){
        if(!mp_Instance)
        {
            mp_Instance= new Myclass;
        }    
        
        return mp_Instance;
        
        // Thread safe değil
        // Delete edilmiyor.
    }    
}

main

Myclass *p= Myclass::get_instance();
```

###### Meyer's Singleton ######
```cpp
class Myclass {
    private:
    Myclass();
    
    public:
    
    static Myclass & getInstance()
    {
        static Myclass m;
        return m;
        // Thread safe
        // Program sonunda delete olacak
    }
    
    void foo();
    void bar();
    void baz();
}
```

###### Named Constructor Idiom-3
```cpp
class Complex {
    public:
    static Complex createCartesian(double r, double n)
    {
        return Complex(r, n);
    }
    
    static Complex createPolar(double angle, double distance)
    {
        return Complex(angle, distance, 0);
    }        
    private:
    Complex(double r, double n);
        Complex(double angle, double distance, int);
}

main
auto c1= Complex::createCortesian(2.3, 5.6);
auto c2= Complex::createPolar(2.3, 5.6);
```

** Ninja Example **
```cpp
class Ninja
{
    private:
    inline static std::vector<Ninja *> ninjaList{};
    std::string m_Name;
    
    public:
    Ninja(const std::string &s): mName{s}
    {
        ninjaList.push_back(this);
    }
    
    Ninja()= default;
    Ninja(const Ninja &) = delete;
    Ninja & operator=(const Ninja &) = delete;
    // copy ctor ve assignment delete olursa move fonksiyonları da not decleared olur.
   ~Ninja()
   {
       if(auto itr= std::find(ninjalist.begin(), ninjalist.end(), *this); itr != ninjaList.end() )
       {
           ninjaList.erase(iter);
       }
   }
    
    void callFriends()
    {
        for (auto e: ninjaList)
        {
            if(e != this)
            {
                cout <<e.mName;
            }    
        }            
    }
}

main

Ninja n1{"n1"};
Ninja n1{"n2"};

Ninja * n3 {new Ninja{"n3"}};
Ninja * n4 {new Ninja{"n4"}};

delete n3;
n4->callFriends(); // n1,n2
delete n4;
n1.callFriends(); // n2
```


###### Friend Declerations
Bu decleration ile;

* Namespace scope(global)'daki bir fonksiyona
* Bir sınıfın üye fonksiyonuna
* Bir sınıfın tüm üye fonksiyonlarına (Sınıfın tüm private member'larına erişim yetkisi verilir. Bir nevi bu yapılar için private kaldırılır.)

```cpp
class Myclass{
    public:
    friend void func(Myclass);
    private:
    void foo();
    int mx;
}

void func(Myclas px)
{
    Myclass cx;
    cx.mx= 10;// Legal    
    px.foo();// Legal, çünkü bu fonksiyona sınıfın private member'larına erişim yetkisi verildi.
}    
```

* Ayrıca istenirse global'de decleare func fonksiyonu Myclass içerisinde define edilebilir. Buna hidden friend function denir.

```cpp
friend void func(Myclass pm)
{
    // Code
}    
//Bu fonksiyon sınıfın member fonk'u değildir. Sanki namespace scope'daki bir fonk. gibi düşünülmelidir. 
```

* Friend bildirimlerinin sınıfın public, private veya protected alanlarında yapılmasının bir farkı yoktur.

Örnek;
```cpp
class Nec {
    public:
    void foo(); // Visible olması lazım. Myclass için forward decleration olursa friend tanımı geçersiz olur.
}

class Myclass {
    private:
    friend void Nec::foo();
    void func();
}

void Nec::foo()
{
    Myclass x;
    x.func(); // Legal
}    
```

Örnek;
```cpp
class Myclass {
  private:
    friend class Nec; // Nec class'ın bütün fonksiyonları benim private member'larıma erişebilir.
    
    void f1();
    void f2();
};

class Nec {
    void foo(){
        Myclass m;
        m.f1(); // Legal
        m.f2(); // Legal
    }
    
    void bar()
    {
        Myclass m;
        m.f1(); // Legal
        m.f2(); // Legal
    }    
};
```

