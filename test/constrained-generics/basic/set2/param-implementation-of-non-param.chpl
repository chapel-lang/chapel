/*
This test checks the situation where
- a required function is non-param
- its implementation is a param function
*/

interface MyArray {
  proc Self.rank: int;
}

implements MyArray(Locales.type);
// array.rank param fulfills the Self.rank requirement

proc cgFun(arg: MyArray) {
  writeln(arg.rank);
}

cgFun(Locales);
