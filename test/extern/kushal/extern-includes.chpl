printInput(1);
printInput(-1);

proc printInput(num: int) {
  require "foo.h";
  extern proc printfoo(num);
  printfoo(num);
}
