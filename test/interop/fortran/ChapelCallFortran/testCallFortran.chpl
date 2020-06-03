pragma "generate signature" extern proc getInt(): int;
pragma "generate signature" extern proc getReal(): real;
pragma "generate signature" extern proc setInt(ref i: int);
pragma "generate signature" extern proc setReal(ref r: real);

pragma "generate signature" extern var fortInt: int;
pragma "generate signature" extern var fortReal: real;

proc main {
  var answer = 42,
      e  = 2.718281828,
      pi = 3.141592653;
  setInt(answer);
  setReal(e);

  assert(fortInt == 42, "fortInt == " + answer:string);
  assert(fortReal == e, fortReal:string + " == " + e:string);

  fortInt = -1;
  fortReal = pi;

  assert(getInt() == -1, "getInt() == -1");
  assert(getReal() == pi, "getReal() == " + pi:string);

  writeln("Success!");
}
