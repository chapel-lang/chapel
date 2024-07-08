// I wrote this test thinking that we shouldn't ever resolve `bar()`
// since the compiler should be able to figure out that the
// conditional is always 'false' and filed what was initially a bug
// for it in issue #20873.  DIten pointed out that we have to evalute
// the 'foo()' function for side-effects even though we know the end
// result of the conditional will be false (assuming no override of
// the config), and while that seems right, it also seems as though we
// shouldn't need to resolve 'bar()'.  Anyway, I've changed this from
// a future to a test that locks in the current behavior, but I think
// Vass's argument on #20873 that we should evaluate foo() for
// side-effects yet still consider the result of the expression to be
// a 'param' sounds good/preferable to me.

config param flag = false;

if foo() && flag then
  bar();

writeln("OK!");

proc foo() {
  return true;
}

proc bar() param {
  compilerError("Should we have resolved this?");
}
