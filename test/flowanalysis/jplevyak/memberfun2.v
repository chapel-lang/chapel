class A { c:1; d:2;};
class B : A { e:3.0; };
A::f() : c + d;
b : new B;
B::g() : e + f();
print(b.g());
