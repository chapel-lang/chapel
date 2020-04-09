/* 2018-06-08 This test exposes a bug where the type 'Empty' is pruned.

Bug notes:

* The bug goes away if we remove the argument from conjGrad(),
i.e. make it proc conjGrad() {...}

* I did not want to define proc init() for Empty. However,
if I don't, then I get an unrelated error, which counts as an
"unimplemented feature": "could not generate default initializer
for type 'Empty', please define one".

* conjGrad1 is the simpler reproducer. conjGrad2 adds a non-trivial method.

* BTW this test checks that we can define like-named
types inside different functions. (Yes we can.)

* This is motivated by cg-sparse-partred.chpl as of later today.
The workaround there is to move the definition of the records
to the global scope. The counterpart of hi() is still defined
in the function scope. (Yes that works.)
Once this bug is fixed, we can move the records back to the
function scope in cg-sparse-partred.chpl .
*/

proc plusPR1(FEXPR) {}

proc conjGrad1(A) {
  record Empty { proc init(){} }
  plusPR1(new Empty());
}

proc plusPR2(FEXPR) { writeln(FEXPR.hi()); }

proc conjGrad2(A) {
  var B = [333, 555, 777];
  record Empty { proc init(){} proc hi() return B[2]; }
  plusPR2(new Empty());
}

proc main {
  conjGrad1(Locales);
  conjGrad2(Locales);
}
