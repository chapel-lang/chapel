proc foo() param {
  return false;
  return true;
}

param p = foo();

writeln(p);
