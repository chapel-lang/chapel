proc h(arg, otherArg) {
  arg = 11;
}

proc g(arg, otherArg: int) {
  h(arg, otherArg);
}

proc f(arg:string, otherArg:int) {
  g(arg, otherArg);
}

f("hi", 1);
