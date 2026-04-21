# Knowledge check: Move семантики и rvalue референции

>_За всички въпроси приемаме, че са include-нати нужните неща._

### Въпрос 1

Какво ще се отпечата на конзолата след изпълнението на следния код?

```cpp
#include <iostream>
#include <utility>

class A {};

void f(const A&)
{
    std::cout << "f(const A&)\n";
}

void f(A&&)
{
    std::cout << "f(A&&)\n";
}

int main()
{
    A a;
    const A ca;

    f(a);
    f(ca);
    f(A());
    f(std::move(a));
    f(std::move(ca));
}
```

---

### Въпрос 2

Ще се компилира ли? Ако да — какво ще се отпечата? Ако не - защо?

```cpp
#include <iostream>

class A
{
public:
    A() = default;

    A(const A&)
    {
        std::cout << "A(const A&)\n";
    }

    A(A&&)
    {
        std::cout << "A(A&&)\n";
    }
};

void g(A&& obj)
{
    A other = obj;
}

int main()
{
    g(A());
}
```

---

### Въпрос 3

Какво ще се отпечата на конзолата след изпълнението на следния код?

```cpp
#include <iostream>
#include <utility>

class B
{
public:
    B() = default;

    B(const B&)
    {
        std::cout << "B(const B&)\n";
    }

    B& operator=(const B&)
    {
        std::cout << "B::operator=(const B&)\n";
        return *this;
    }

    ~B() = default;
};

int main()
{
    B b1;
    B b2 = std::move(b1);

    B b3;
    b3 = std::move(b2);
}
```

---

### Въпрос 4

Какво ще се отпечата на конзолата след изпълнението на следния код?

```cpp
#include <iostream>

class B
{
public:
    B() = default;

    B(const B&)
    {
        std::cout << "B(const B&)\n";
    }

    B& operator=(const B&)
    {
        std::cout << "B::operator=(const B&)\n";
        return *this;
    }

    B(B&&)
    {
        std::cout << "B(B&&)\n";
    }

    B& operator=(B&&)
    {
        std::cout << "B::operator=(B&&)\n";
        return *this;
    }

    ~B() = default;
};

class C {
    B b;
public:
    C() = default;

    ~C() {
    }
};

int main()
{
    C c1;
    C c2 = std::move(c1);
}
```

---

### Въпрос 5

Какво ще се отпечата на конзолата след изпълнението на следния код?

```cpp
#include <iostream>

struct Part
{
    Part() = default;

    Part(const Part&)
    {
        std::cout << "Part(const Part&)\n";
    }

    Part(Part&&) noexcept
    {
        std::cout << "Part(Part&&)\n";
    }
};

struct Holder
{
    Part part;

    Holder(Part&& part) : part(part) {}
};

int main()
{
    Part p;
    Holder h(std::move(p));
}
```

---

### Въпрос 6

Ще се компилира ли следния код? Ако не - защо?

```cpp
#include <utility>

struct Engine
{
    Engine() = default;
    Engine(const Engine&) = default;
    Engine(Engine&&) = delete;
};

struct Car
{
    Engine engine;
};

int main()
{
    Car c1;
    Car c2 = std::move(c1);
}
```

---

### Въпрос 7

Какво ще се отпечата на конзолата след изпълнението на следния код?

```cpp
#include <iostream>

struct Wheel
{
    Wheel() = default;

    Wheel(const Wheel&)
    {
        std::cout << "Wheel(const Wheel&)\n";
    }

    Wheel(Wheel&&) noexcept
    {
        std::cout << "Wheel(Wheel&&)\n";
    }
};

struct Engine
{
    Engine() = default;

    Engine(const Engine&)
    {
        std::cout << "Engine(const Engine&)\n";
    }

    Engine(Engine&&) noexcept
    {
        std::cout << "Engine(Engine&&)\n";
    }
};

struct Car
{
    Engine engine;
    Wheel wheel;

    Car(Engine&& e, Wheel&& w)
        : engine(std::move(e)), wheel(w) {}
};

int main()
{
    Engine e;
    Wheel w;
    Car c(std::move(e), std::move(w));
}
```

---

### Въпрос 8

Какво ще се изведе на конзолата?

```cpp
struct Buffer
{
    Buffer() = default;

    Buffer(const Buffer&)
    {
        std::cout << "Buffer(const Buffer&)\n";
    }

    Buffer(Buffer&&)
    {
        std::cout << "Buffer(Buffer&&)\n";
    }
};

struct Container
{
    Buffer buffer;

    Container() = default;
    Container(const Container&) = default;
};

int main()
{
    Container c1;
    Container c2 = std::move(c1);
}
```

---

### Въпрос 9

Какво ще се изведе на конзолата?

```cpp
#include <iostream>

class A {
public:
    A() {
        std::cout << "A()" << std::endl;
    }

    A(const A& other) {
        std::cout << "A(const A& other)" << std::endl;
    }

    A& operator=(const A& other) {
        std::cout << "A& operator=(const A& other)" << std::endl;
        return *this;
    }

    A(A&& other) noexcept {
        std::cout << "A(A&& other)" << std::endl;
    }

    A& operator=(A&& other) noexcept {
        std::cout << "operator=(A&& other)" << std::endl;
        return *this;
    }

    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

A&& g() {
    return A();
}

int main()
{
    A obj = g();
}
```

---

### Въпрос 10

Какво ще се изведе на конзолата?

```cpp
#include <iostream>

class A {
public:
    A() {
        std::cout << "A()" << std::endl;
    }

    A(const A& other) {
        std::cout << "A(const A& other)" << std::endl;
    }

    A& operator=(const A& other) {
        std::cout << "A& operator=(const A& other)" << std::endl;
        return *this;
    }

    A(A&& other) noexcept {
        std::cout << "A(A&& other)" << std::endl;
    }

    A& operator=(A&& other) noexcept {
        std::cout << "operator=(A&& other)" << std::endl;
        return *this;
    }

    ~A() {
        std::cout << "~A()" << std::endl;
    }
};

A g() {
    return A();
}

int main()
{
    A obj = g();
}
```

---

## Въпрос 11

Какво ще се изведе на конзолата?

```cpp
class B{};

class A {
private:
    B b;
public:
    A() {
        std::cout << "A()" << std::endl;
    }

    explicit A(B b) {
        std::cout << "A(B)" << std::endl;
    }

    explicit A(const B& b) {
        std::cout << "A(const B&)" << std::endl;
    }
};

int main()
{
    B b;
    A a(b);
}
```

