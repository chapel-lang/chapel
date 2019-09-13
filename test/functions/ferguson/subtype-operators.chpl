
class Parent {
}
class Child : Parent {
}

proc checkLt(type a, type b) {
  if (a < b) == false then
    compilerError("unexpected: " + a:string + " not < " + b:string);
}

proc checkNotLt(type a, type b) {
  if (a < b) then
    compilerError("unexpected: " + a:string + " < " + b:string);
}

proc checkLte(type a, type b) {
  if (a <= b) == false then
    compilerError("unexpected: " + a:string + " not <= " + b:string);
}

proc checkNotLte(type a, type b) {
  if (a <= b) then
    compilerError("unexpected: " + a:string + " <= " + b:string);
}

proc checkGt(type a, type b) {
  if (a > b) == false then
    compilerError("unexpected: " + a:string + " not > " + b:string);
}

proc checkNotGt(type a, type b) {
  if (a > b) then
    compilerError("unexpected: " + a:string + " > " + b:string);
}

proc checkGte(type a, type b) {
  if (a >= b) == false then
    compilerError("unexpected: " + a:string + " not >= " + b:string);
}

proc checkNotGte(type a, type b) {
  if (a >= b) then
    compilerError("unexpected: " + a:string + " >= " + b:string);
}


proc main() {
  checkLt(borrowed Child, borrowed Parent);
  checkNotLt(int(8), int(16));
  checkNotLt(borrowed Parent, borrowed Child);
  checkNotLt(int(16), int(8));
  checkNotLt(borrowed Child, borrowed Child);
  checkNotLt(int(8), int(8));

  checkGt(borrowed Parent, borrowed Child);
  checkNotGt(int(16), int(8));
  checkNotGt(borrowed Child, borrowed Parent);
  checkNotGt(int(8), int(16));
  checkNotGt(borrowed Child, borrowed Child);
  checkNotGt(int(8), int(8));

  checkLte(borrowed Child, borrowed Parent);
  checkNotLte(int(8), int(16));
  checkNotLte(borrowed Parent, borrowed Child);
  checkNotLte(int(16), int(8));
  checkLte(borrowed Child, borrowed Child);
  checkLte(int(8), int(8));

  checkGte(borrowed Parent, borrowed Child);
  checkNotGte(int(16), int(8));
  checkNotGte(borrowed Child, borrowed Parent);
  checkNotGte(int(8), int(16));
  checkGte(borrowed Child, borrowed Child);
  checkGte(int(8), int(8));

  var i8: int(8);
  var i16: int(16);
  checkNotLte(i8.type, i16.type);

  // try a couple of other patterns
  if i8.type < i16.type {
    compilerError("error A");
  } else {
    // OK
  }
  
  if int(8) < int(16) {
    compilerError("error B");
  } else {
    // OK
  }
}
