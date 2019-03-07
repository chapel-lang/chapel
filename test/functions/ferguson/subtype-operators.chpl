
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
  checkLt(Child, Parent);
  checkLt(int(8), int(16));
  checkNotLt(Parent, Child);
  checkNotLt(int(16), int(8));
  checkNotLt(Child, Child);
  checkNotLt(int(8), int(8));

  checkGt(Parent, Child);
  checkGt(int(16), int(8));
  checkNotGt(Child, Parent);
  checkNotGt(int(8), int(16));
  checkNotGt(Child, Child);
  checkNotGt(int(8), int(8));

  checkLte(Child, Parent);
  checkLte(int(8), int(16));
  checkNotLte(Parent, Child);
  checkNotLte(int(16), int(8));
  checkLte(Child, Child);
  checkLte(int(8), int(8));

  checkGte(Parent, Child);
  checkGte(int(16), int(8));
  checkNotGte(Child, Parent);
  checkNotGte(int(8), int(16));
  checkGte(Child, Child);
  checkGte(int(8), int(8));

  var i8: int(8);
  var i16: int(16);
  checkLte(i8.type, i16.type);

  // try a couple of other patterns
  if i8.type < i16.type {
    // OK
  } else {
    compilerError("error A");
  }
  
  if int(8) < int(16) {
    // OK
  } else {
    compilerError("error B");
  }
}
