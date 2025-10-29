proc foo(type arg1, param arg2: arg1 = "hello") where isIntType(arg2.type) {
  compilerWarning("arg1 constrains arg2's type to be: " + arg1:string);
  compilerWarning("arg2's final value ends up " + arg2:string);
}
foo(int);
