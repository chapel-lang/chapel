
in my_module;

v : 0;
f a : v + a;
f 1;
g b : 7 + b;

in my_module2;

v : 1;
f a : v + a + 1;
f 2;

in user;

v : 2;
f a : v + a + 2;
f 3;

my_module::f v;
