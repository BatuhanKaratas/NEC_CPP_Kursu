### Class Scope
* Compiler'ın name lookup'ta ilk olarak class scope'da bakacağı notasyonlar;
```cpp
class Myclass{
    int x;  
}

Myclass a;
Myclass b= &a;
a.x; //Dot operator
b->x //Arrow operator
Myclass:: //Scope resolution operator    
```
Not: 
```cpp
class Myclass {
    private:
    void f(double);
    public:
    void f(int);
}
// Burada function overloading vardır.Çünkü private ve public ayrı bir scope oluşturmaz. Tek scope Myclass'dır.

Myclass m;
m.f(1.2); // Syntax error. 3'ten dolayı elendi.
// Namelookup süreçleri;
// 1- Name lookup (Myclass scope'u içinde ara)
// 2- Context Control (Overload var. Function resolution: Exact Match)
// 3- Access Control X (Private func)

class Myclass {
    public:
    void foo(); // Non static member function (Class scope)
    // İlk parametre non static member func olduğu için Myclass *'dır.
    private:
    int mx,my;
};

int foo(int); // Global function, Global namespace scope

Myclass myclass;
myclass.foo(); // Sadece class scope'da aranacak.
```
```cpp
class Myclass {
    public:
    void foo(); // Non-static member function
}

int main()
{
    Myclass::foo(); 
    // Namelookup - Ok
    // Context Error - Fail.
    // Çünkü non-static function'nın bir object ile çağrılması lazımdır. Class ile çağrılamaz.
    
    Myclass m;
    foo(&m); // Name lookup - Fail
    // foo func'ı nerede arayacağını bilemez. Bu sebepten ötürü bulamaz.
    Myclass::foo(&m); 
    // Name lookup - Ok
    // Context control - Fail
}    
```

```cpp
"header.h"
    
class Myclass {
    public: 
    void foo() // Implicitly inline func olur. Çünkü bu func definition header'da tanımlı(compilerın görmesi lazım) bir class definition'ı içerisinde yapılmıştır ne kadar include edilirse edilsin ODR ihlal edilmez. 
    {
        // Code
    }
    
    void bar();
}    

"header.cpp"
void Myclass::bar() // Source file'da bar fonksiyonunun definition'ı yapıldığı için inline function değildir. 
{
    
}

"header"
class Myclass {
    public: 
    int foo();
    int mx;
    int my;
};

"source"
int Myclass::foo()
{
    mx= 10; // Bu func içeriğindeki şeyler class scope'undada aratılır. Çünkü foo Myclass'ın member function'ıdır ve "Myclass *" gizli parametresi vardır. Üye değişkenler ise "this->mx" ile çağrılır. Buda namespace aramasında class scope'unda dahil edilmesine sebep olur. 
}   
```

##### Name Lookup arama sırası;
1. Kullanıldığı blok içersinde
2. 1'i kapsayan bir üst blok içerisinde (enclosing block)
3. Class Scope(class definition içerisinde)
4. Namespace scope 

Not: Name Lookup bir kez yapılır. Name bulunduğunda diğer aşamaya geçmez.

```cpp
"Header"
class Myclass {
    public:
    void foo(Myclass);
    private:
    int x;
}

"Source"
Myclass g;

void Myclass::foo(Myclass m)
{
    auto n= m.x; //Valid
    Myclass y; //Valid
    y.x= 45; //Valid
    g.x= 10; //Valid
}    
// Bu ifadelerin hepsi valid'dir. Çünkü private member'lara class fonksiyonu definition'ı üzerinden ulaşma yetkimiz vardır.

int x= 21;
void Myclass::foo(){
    int x= 10;
    Myclass::x = 15; // Qualifier olduğu için direk Myclass scope'unda arama yapılacaktır.
    ::x= 55; // Qualifier olduğu için direk Global Namespace scope'unda name bulunur.
    x= 10; //Ilk olarak içinde bulunduğu block scope'u içerisinde aramaya yapılır ve local x bulunur.
}

void Myclass::foo()
{
    ++x; 
   // Name lookup sırası;
   // Kullanıldığı block içerisinde - yok
   // Enclosing Block yok
   // Class scope - Bulundu
}    
```

```cpp
class Counter {
    public:
    void foo()
    {
        set(20); // Class scope'da func name'i bulur. (set)
        // Function namelookup'ında sadece func name'e göre arama yapar.
        // Parametreleri dahil etmez.
    }
    
    void set(int val)
    {
        mc= val;
    }    
    private:
    int m{};
};
```

```cpp
class Myclass {
    public:
    void func(int); // Class scope'u içerisinde function redecleration olmaz. Syntax error.
    void func(int);    
};

void foo(int); // Global scope'da foo fonksiyonu function redecleration'dır.
void foo(int);
```

### This

Bu keyword sadece sınıfın static olmayan fonksiyonları içinde kullanılabilir. Global scope'da ki fonksiyonlarda da kullanılamaz.

"this" bir pointer'dır. Çağrıyı yapan nesnenin adresini ifade eder.

"this" bir PR value expression'dır.

```cpp
class Myclass {
    public:
    void foo() {
        int mx=89;
        mx=20; // Local mx Block scope
        Myclass:mx= 20; // Class scope mx
        this->mx = 25; // Class scope mx
        (*this).mx= 30; // Class scope mx. "." prioirity'si > "*"" priority. Bu yüzde *this parantez içine alınmalıdır.
    }
   private:
    int mx;
}
```

This kullanım amaçları;
* Call yapan nesneye erişmek için
```cpp
void foo (Myclass *)
void bar (Myclass )
void baz (Myclass &)

void Myclass::f() {
 foo(this);
 bar(*this);
 baz(*this);
}
```

Not: C++'da public scope sadece class içerisinde tanımlı public alandaki değişken yada fonksiyonlar  değil, aynı zamanda aynı header'da global scope tanımlı değişken ve fonksiyonlardır.

```cpp
class Myclass {
    public:
    Myclass f1() {
        return *this;
    }
    
    Myclass* f2() {
        return this;
    }
    
    Myclass & f3() {
        return *this;
    }
}
```

Not: 
```cpp
int x=10;
double y= 1.0;

cout << x << y;
// Her bir left shift operator return type'ı this*'dır.
```

Const Member Functions

```cpp
void func(T *p) // Mutator
void foo(const T *p) // Accessor    
    
void func(T &p) // Mutator
void foo(const T &p) // Accessor    

class Myclass {
    public:
    void get() const; // Accessor
    // Böylece gizli Myclass * parametresinin const Myclass * olduğunu compiler'a söylemiş olduk.
}    

//Not:
const int x= 10;
int *p= &x; // Syntax error. cont T * to T *'a implicit dönüşüm yoktur. Bu yüzden hata oluşur.

class Myclass {
    public:
    void foo()
    {
        func(); // T *'dan const T*'a implicit dönüşüm vardır. 
    }
    
    void func() const
    {
        foo(); // Bu func'da const func olduğu için gizli parametre const T *'dır. Bu sebepten this (const T *) to (T *) dönüşümü yapılamaz hata verir. 
        mx= 10; // Const function olduğu için mx member'ı üzerinde değişiklik yapamayız. Syntax error.
    }
private:
    int mx;
}

const Myclass m;
m.foo(); // Illegal const t * to t* implicit dönüşümü yoktur.
m.func(); // func const fonksiyon olduğu için legal
Myclass n;
n.foo(); // Legal
n.func(); // Legal T * to Const T * implict dönüşümü vardır.


class Myclass {
    public:
    const Myclass * foo() const {
        return this; // this ->const Myclass *
    }
    
    const Myclass & foo() const {
        return (*this);
    }
}

// Global namespace scope
void foo() const
{
    Myclass m;
    m.mx= 10; // Legal. This ile ilgili herhangi bir işlem yoktur.
}    

void foo();
void foo() const;
// or
void foo(int *);
void foo(const int *);
// or
void foo(int &);
void foo(const int &);

// Yukarıdaki ikililerin her biri bir func overloading oluşturur.

```

Not:
- const fonksiyonlar sadece const objelerle çağrılabilir.
- const fonksiyonlar sadece const fonksiyonları cağırabilir.

```cpp
class vector {
    public:
    int & front();
    const int & front() const; // Const overloading
};

const vector vec;
auto n= vec.front();
vector vec2;
vec2.front()= 56;
```
