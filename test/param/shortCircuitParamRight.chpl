config param flag = false;

if foo() && flag then
  bar();

writeln("OK!");

proc foo() {
  return true;
}

proc bar() param {
  compilerError("Shouldn't get here");
}
