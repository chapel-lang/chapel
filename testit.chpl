require 'testit.h';

proc foo() {
  writeln("Hello!");
}

extern proc bar(f);

bar(foo);

//foo();