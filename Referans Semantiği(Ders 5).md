# Reference Semantics
Referans, pointer'lara sadece dil katmanında bir alternatiftir.
Assembly code'da pointer ve referans aynıdır.

**Call by value**: Fonksiyona gönderilen nesnenin bir kopyası alınır.
**Call by reference**: Fonksiyona gönderilen nesnenin kopyalanmadan kendisi üzerinde işlem yapılıyor.

C'de call by reference:

```cpp
void func(int *p);
int a= 5;
func(&a);
```

C++'da call by reference:
```cpp
void func(int &p);
int a= 5;
func(a);   
```

**Referans kategorileri**
* L value reference
* R value reference (Move semantics - Perfect forwarding)
* Forwarding(Universal) reference

**L value reference**
* L value referans ile bir nesnenin yerine geçecek bir yapıyı tanımlamış oluyoruz.

```cpp
int x= 10;
// Pointer semantiği ile;
int * const p= &x;
(*p)= 20;
++(*p);
// Referans semantiği ile;
int &r = x; // Referans bind işlemi (L value ref, L value expression ile initialize edilmek zorundadır.)
r= 20;
++r;
```
* Referans deklatörlü ifade default initialize edilemez. (Compile tim error)
* Referans'lar sadece bir defa bind edilebilir yani her referans sadece bir defa(initialization'da) bir nesneye refer edilebilir.
```cpp
int x{3};
int &r{x}; 
int *p = &r; // r x demektir.
```

Not: Pointer bellekte bir yer tutmaktadır. Fakat reference initialize edilen şeyin yerine geçmektedir. Bellekte ayrıca bir yer tutmaz.

Örnek kodlar;

```cpp
int x{20};
int &r1= x;
int &r2= r1;
int &r3 = r2;
int *p= &x;
int * &rp= p; // P'nin türü "int *" olduğu için referans'ın türüde int * olur.  
*rp= 99;

//---------------------------------------------------------------------
int a[] {1, 3, 5, 7, 9};
int *p = a; // Array decay p a'nın 0. index'ine point ediyor.
++p; //a'nın 1. index'ine point ediyor.
int (*p) [5]= &a; // p array'in kendisine point ediyor.
int (&ra) [5]= a; // Array decay bu noktada çalışmaz. Çünkü bir referans değerine ataması yapıldı. Bu yüzden direk a array'inin kendisini refer eden bir l value ra referansı oluşturuldu. 

//---------------------------------------------------------------------
struct Data {
    int a,b,c;
}
Data mydata{2, 4 , 5};
Data & rd= mydata;
cout << rd.a;
```

**Pointer Sem**
```cpp
int x= 45;
void func(int *p)
{
    *p= 55;
}
func(&x);
```

**Ref Sem**
```cpp
int x= 45;
void func(int &p)
{
    p= 55;
}
func(x);
```

```cpp
int *p{};
int &r = *(p); // Dereferencing
int a [10][20][30]{0};
int (&r) [30]= a[10][20];
```

**Pointer Sem**
```cpp
int g= 5;

int * func()
{
    return &g;
}

int * ptr= func();

*ptr = 100; // Global scope'daki g'in değeri 100 olarak değiştirildi.
```
**Reference Sem**
```cpp
int g= 5;

int & func()
{
    return g;
}

int & r= func();
r= 100;

func()= 99; // valid g=99;
++func(); // valid g=100;
```
Önemli Not: Dangling pointer olmaması için statik ömürlü nesnelerin adresi veya referansı döndürülmemelidir.

Önemli Not: 
```cpp
int a= 10;
int * const ptr= &a; // const ptr to int
const int * ptr= &a; // ptr to const int
```
##### Referans Semantiğinde Const kullanımı
```cpp
int x= 45;
const int & r = x; // const R'ın değerini non-mutable yapar.             
//Aynı zamanda r başka bir yere referans verilemez. Bu kural zaten tüm referans semantiği için geçerli bir durumdur. 

void func(T &); // setter (mutator)
void func(const T &); // getter parametrenin değerinin değişmeyeceği anlamına gelir.
//-------------------------------------------------------
const int x= 10;
const int & r= x; // Dilin kuralı gereği r reference'ı const olmak zorundadır. 

//-------------------------------------------------------
const int & foo();
foo()= 45; // foo() return'ünü başka bir değere eşitleyemeyiz.


//--------------------- Önemli ------------------------------
int x= 10;
double & dr = x; // Illegal
const double & dr= x; // const olduğu için Legal'dir.
// Kod açılımı;
double temp= x; // Conversion için geçici bir double nesne oluşturur.
const double &dr= temp; // Geçici nesneyi const ref'e eşitler.

int &r= 10; // Illegal. Çünkü referans'lar sadece L value ile assign edilebilir. Burada R value ile initialize edilmeye çalışılmış.
const int &r= 10; // const olduğu için Legal'dir.
// Kod açılımı;
int temp= 10; // Geçici nesne kullanılarak değer L value'ya çevrilir.
const int & r = temp;
```

** Pointers vs References **

** Pointers **
* Default initialization var.
* Pointer bellekte yer tutar ve adresi alınabilir.
* Function pointer vardır.
```cpp
int foo(int x){return x;}
int (*fp) (int)= &foo; // Function pointer
```
* Elemanları pointer olan dizi olur.
```cpp
int *pa[]= {&x, &y, &z};
```
* Pointer'lar null değerine sahip olabilir. Bu değer işlemlerin başarılı olup olmadığı kontrolünde kullanılır.
```cpp
int *p{nullptr};

// malloc -> return void *
// fopen -> return file *
```

Not: 
    Bir pointer;
    * int * p{}; ile p nullptr'ye initialize edilir.
    * Statik ömürlü değişkenler initialize edilmezse nullptr atanır.
    * Otomatik ömürlü değişkenler initialize edilmezse garbage value atanır. (Undefined behaviour)

* Const ptr değilse pointer'lar başka bir nesneye point edebilir.

**References**
* Default initializing yoktur.
* Reference bellekte yer tutmaz ve adresi yoktur.
* Function reference vardır.
```cpp
int foo(int x){return x;}
int (&fp) (int)= foo; // Function reference
```
* Elemanları referans tipinde olan dizi yoktur.
* Null reference yoktur. Bunun yerine C++17 ile gelen optional kullanılabilir.
* Referanslar başka bir nesneye refer edilemez. (Tek bind olur)