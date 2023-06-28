###### Special Member Function Status

* Not Decleared: Fonksiyon yok
* User Decleared
    * User defined && decleared
    * User defined && default
    * User defined && deleted
 * Implicitly Decleared
     * Compiler defined && decleared
     * Compiler defined && deleted 
 
 Not: Compiler tarafından default edilen "default ctor" sınıfın non-static, public, inline fonksiyonudur.
 
 * Derleyici sınıfın tüm non-static veri elemanlarını default-init edecek kodu üretir.
 
 ```cpp
class Nec 
{
    private:
    A ax;
    B bx;
    C cx;
};

// Bu durumda class Nec için compiler'ın ürettiği default ctor şu şekilde olur;

Nec() : ax{}, bx{}, cx{}
{
    // A, B ve C class'larının default ctor'u çağrılır. Primitive typelar garbage value alırlar.
}
```

Eğer derleyici bir special member function'o default ettiğinde dilin kurallarını çiğneyen bir durum oluşursa derleyici Syntax hatası vermek yerine default etmesi gerektiği fonksiyonu delete eder.

Neden syntax hatası oluşacak bir durum olsun ki?

* Default init edilemiyor olabilir. (Class member'ları const veya reference ise)
* Olmayan bir fonksiyona çağrı(Bir member'ın default ctor'u delete olmuş ve call edilmeye çalışılıyor)
* Private bir fonksiyon çağrılmaya çalışıldığında

Örnekler;

```cpp
class Nec
{
    public:
    Nec(int); // Nec'in default ctor'u yoktur. Başka bir ifadeyle implicitly decleared && deleted'dır.
}

class Myclass
{
    public:
    private:
    Nec mnec; //Nec'in default ctor'u olmadığı için default ctor call eden bir initialization syntax error'dur.
    const int a; // Const değişkenler bir değer ile default initialize olmak zorundalar. Compiler bunu implicitly yapamaz. Syntax error'dur.
}
```

##### In-Class Initializer(Default Member Initialization(DMI))

```cpp
class Myclass 
{
    private:
    int mx{0}; // OK
    double dual= 3.4; // OK
    int my(23); // Default Member Initialization'a uygun değil
    int mz{10};
}
// Bu durumda Myclass için default ctor implicitly decleared olur. Fakat DMI kullanıldığı için DMI'da initialize edilen değerler member'ların ilk değeri olur.  
```

```cpp
class Myclass 
{
 public:
    Myclass() : my{30}
    {
     // User decleared && defined default ctor mx değeri ctor'da assign edilmemesine rağmen son durumda;
    //    mx: 10
    //    my: 30    
    }
 private:
    int mx{10};
    int my{15};
}
```

##### RAII Idiom

* Class resource'ları constructor'da alınmalı destructor'da ise bırakılmalıdır.


##### Copy Constructor

* Bir sınıf nesnesi hayata değerini aynı türden bir başka sınıf nesnesinden alarak geldiğinde copy ctor çağrılır.

Copy Ctor kullanım alanları;
```cpp
Nec x;
Nec y= x;
Nec y(x);
Nec y{x};
// 3 statement'ta da copy ctor çağrılır ve y nesnesi oluşturulur.
```

* Call by value Function Call
```cpp
void foo(Nec nec);

Nec x;
foo(x); // Call by value call- Parametreye kopyalama yapılırken copy ctor çağrılır.

Nec foo()
{
    return necObject; // Return value'yu dönerken copy ctor çağrılır.
}
```
Compiler tarafından yazılan copy ctor sınıfın non-static, public, inline special member fonksiyonudur.

```cpp
Nec(const Nec &); // Copy Ctor Decleration

private:
A ax;
B bx;
C cx;

Nec(const Nec & other): ax{other.ax}, bx{other.bx}, cx{other.cx}  
// Copy Ctor Definition
{
    // -----
}

Nec x;
Nec y= x;
```
Örnek;

```cpp
class Date{
    public:
    Date(int d, int m, int y); md{d}, mm{m}, my{y}
    {}    
    Date()= default; // Default ctor'u belirtmeseydik.(User decleared && defaulted) Default ctor bu durumda olmayacaktı. Çünkü başka bir ctor tanımlanmış.
    
    private:
    int md{1}, mm{1}, my{1900}; 
}

Date dx{12, 8, 1999};
Date dy= dx; // Birbirinden tamamen bağımsız iki obje. Copy ctor çağrılır ve bu copy ctor compiler tarafından implicitly oluşturulmuştur.
Date dz; // Default ctor çağrılır.
```

###### Rule of Zero
Special member functionları bir zorunluluk olmadığı sürece bırak compiler yazsın.

```cpp
class String {
    public:
    String() = default;
    String(const char * p) : mlen{std::strlen(p)},
                             mp{static_cast<char *>(malloc(mlen+1))}
    {
        if(!mp)
        {
            throw std::runtime_error("Can not allocate memory");
        }
        
        std::strcpy(mp, p);
    }
    
    ~String()
    {
        std::free(mp);
    }
    
    private:
    std::size_t mlen{};
    char * mp{}; // Handle (Pointer or Reference)
}

void func(String s);

//main
String str{"Deneme"};
func(str); // Bu noktada shallow copy yapılır. Bu sakıncalıdır çünkü String class'ı handle içermektedir. Func fonksiyonunun parametresi scope'dan çıkınca destruct olduğunda pointee'de destruct olur ve str değişkeni dangling pointer'a dönüşür. Deep copy gereklidir. Pointee'lerde kopyalanmalıdır. Sadece pointer'lar değil.
```

Deep Copy Ctor
```cpp
String(const String & other) : mlen{other.mlen}, mp{static_cast<char *>(std::malloc(mlen+1))}
{
    if(!mp)
    {
        throw std::runtime_error("Cannot allocate memory")
    }
    
    std::strcpy{mp, other.mp}; // Pointee(Resource) kopyalandı.
}
```

Not: Bir sınıfın objelerinin kopyalanmamasını istiyorsak copy ctor'unun decleration'ının sonuna "=delete" eklemeliyiz.

Not: Bir class'a destructor yazıyorsanız;
Copy ctor ve copy assignment special member functionlarıda yazmalısınız. Bu kuralada BigThree (Rule of Three)
Yeni C++ ile;
Move ctor ve move assignment special member functionlarıda yazılmalıdır. Big Five(Rule of Five)

###### Copy Assignment Function
Eğer her iki nesnede initialize edilmişse yani hayattaysa ve kopyalamaya çalışılırsa bu function çağrılır.

x = y;

Aynı zamanda bir operator overloading'dir. Dilin kurallarına göre operator overloading "operator =" şeklindedir.

Eğer kullanıcı defined etmemişse compiler implicitly olarak non-static, public, inline bir assign operatör fonksiyonu yazar.

```cpp
class Nec {
    public:
    Nec & operator=(const Nec & other)
    {
        ax = other.ax;
        bx= other.bx;
        cx= other.cx;
        return *this;
    }
    private:
    A ax;
    B bx;
    C cx;
};

necx= necy; (necx.operator=(necy))
```

Not: C++'da operator= return value'su türü L value'dur. (T &)

x= y= z= t= foo();

x, y, z ve t foo() return value'suna eşitlendi.

"=" operatörünün çalışma yönü sağdan sola doğrudur.

Örnek;

```cpp
Date dx{12, 5, 2012};
Date dy;

dy = dx; // Copy assignment call edilir.
// Bir başka açılımı
dy.operator=(dx);

(dx = dy).print();
// Bir başka açılımı
dx.operator(dy).print();

//String class'ını kullanırsak 

main()
{
    String s1{"Deneme1"};
    if(true)
    {
        String s2{"Deneme2"};
        // s1.mp -> Deneme1
        // s2.mp -> Deneme2
        
        s2= s1;
        // Compiler'ın yazdığı copy assignment çağrılır ve shallow copy 
        // yapılır.
        
        // Sonuç
        // s1.mp -> Deneme1
        // s2.mp -> Deneme1
        // Scope'dan çıkıldığında Deneme1 free edilir.
        // s1.mp ve s2.mp pointer'ı Dangling pointer durumuna düşer.
        // Deneme2 alanı memory leak olur.
    }
}
```

Eğer user copy assignment'ı yazarsa;

```cpp
// Deep Copy
String & operator=(const String & other)
{
    if(this == &other)
    {
        return *this;
    }
    
    mlen = other.mlen;
    std::free(mp);
    mp= static_cast<char *>(malloc(mlen+1));
    std::strcpy(mp, other.mp);
    return *this;
}

// Eğer bir self assignment olursa 
// int x= 10;
// x=x;
// kendi kaynağını free ettiği için undefined behaviour olur. Buna önlem // olarak; iki operand'ın aynı olup olmadığı kıyaslanır.(this == &other)
// Aynıysa resource management işlemleri yapılmaz. Sol operand direk return edilir.
```

###### Move Semantics

```cpp
T x= other;
x= other;

// Eğer other değişkenin resource bir daha kullanılmayacaksa boşuna copy masrafı çıkarmayalım. Direk move semantic kullanabiliriz. Move ile other'ın resource'unu çalarız.

// Compiler compiler time'da other'ın resource'unun gözden çıkarılabilir olduğuna kanaat getirir. (Yani non-persisten or R value expression) ve move semantiğini kullanmayacı tercih eder.
```

```cpp
void func(const Myclass &) 1 Overloading
{
    // ....
}

void func(Myclass &&) 2 Overloading
{
    // ....
}

void main(int)
{
    Myclass x; // Persistent
    func(x);// 1
    func(Myclass{}); // 2 Non-persistent (R value)
}
```

###### Move Ctor

```cpp
Myclass (Myclass && r); 1 // Move ctor
Myclass (const Myclass &); 2 // Copy ctor
Myclass(); 3 // Default ctor
Myclass foo();

main
Myclass x;3
Myclass y= x; 2 // L value
Myclass z= foo(); 1 // R value
```

###### Move Assignment

```cpp
Myclass & operator(const Myclass &); 1 // Copy assignment
Myclass & operator(Myclass &&);2 // Move assignment
Myclass foo();
Myclass x,y;

x= y; // 1 L value
x= foo(); // 2 R value
y= Myclass{}; // 2 R value. Bu nesneyi başka bir kodun kullanma ihtimali yok o yüzden move assignment çağrılır.
```

```cpp
Myclass x;
Myclass y= x; // Copy ctor call edilir.
Myclass z= std::move(x); // Bu move fonksiyonu sayesinde L value - expression R value'ya dönüştürülür. (Value category conversion) 
```

###### Move Ctor Default Compiler Implementasyonu
```cpp
class Nec {
    public:
    // Implicitly implemented compiler move ctor
    Nec(Nec && other): ax{std::move(other.ax)}, bx{std::move(other.bx)},     cx{std::move(other.cx)}
    {
        // Some code
    }
    
// Not: Eğer member'ları assign ederken move fonksiyonunu kullanmazsak bu member'ların copy ctor'ları çağrılır. Çünkü other.ax ifadesi L value'durç A, B ve C için move ctor çağrılmasını istiyorsak std::move(other.ex) ifadesini kullanabiliriz. 
};
```
