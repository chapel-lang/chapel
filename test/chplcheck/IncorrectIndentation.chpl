module IncorrectIndentation {
  proc f1()
  {
    writeln("hi");
    writeln("??");
  }

  proc f2()
  {
  writeln("hi");
  }

  proc f3() {
  writeln("hi");
  }

  proc f4() {
    writeln("hi");
      writeln("hi");
  }

  proc f5() {
    writeln("hi"); writeln("hi");
  }

  proc f6() {
    for 1..10 do
    writeln("hi");
  }

  module M1
  {
    writeln("hi");
    writeln("??");
  }

  module M2
  {
  writeln("hi");
  }

  module M3 {
  writeln("hi");
  }

  module M4 {
    writeln("hi");
      writeln("hi");
  }

  module M5 {
    writeln("hi"); writeln("hi");
  }

  module M6 {
    for 1..10 do
    writeln("hi");
  }

  for 1..10 {
    writeln("hi");
  }

  for 1..10
  {
  writeln("hi");
  }

  for 1..10 {
  writeln("hi");
  }

  module NestedOuter {
    module NestedInner {
      writeln("hi");
       writeln("??");
      writeln("??");
      record nestedRecord {
        proc firstProc() {}
         proc secondProc() {}
        proc thirdProc() {}

        proc nestedProcOuter() {
          proc nestedProcInner(x: int) do return x;
            proc nestedProcInner(x: string) {
              writeln(x);
               writeln(x);
              writeln(x);
              return x;
            }
        }
      }
    }
  }

  on here
  {
    writeln("hi");
    writeln("??");
  }

  on here
  {
  writeln("hi");
  }

  on here {
  writeln("hi");
  }

  on here {
    writeln("hi");
      writeln("hi");
  }

  on here {
    writeln("hi"); writeln("hi");
  }

  on here {
    for 1..10 do
    writeln("hi");
  }

  begin
  {
    writeln("hi");
    writeln("??");
  }

  begin
  {
  writeln("hi");
  }

  begin {
  writeln("hi");
  }

  begin {
    writeln("hi");
      writeln("hi");
  }

  begin {
    writeln("hi"); writeln("hi");
  }

  begin {
    for 1..10 do
    writeln("hi");
  }

  var dummy: int;

  begin with (ref dummy)
  {
    writeln("hi");
    writeln("??");
  }

  begin with (ref dummy)
  {
  writeln("hi");
  }

  begin with (ref dummy) {
  writeln("hi");
  }

  begin with (ref dummy) {
    writeln("hi");
      writeln("hi");
  }

  begin with (ref dummy) {
    writeln("hi"); writeln("hi");
  }

  begin with (ref dummy) {
    for 1..10 do
    writeln("hi");
  }

  // Note: 'cobegins' with one statement throw warning, so all tests here include
  // at least two statements.

  cobegin
  {
    writeln("hi");
    writeln("??");
  }

  cobegin
  {
  writeln("hi");
  writeln("hi");
  }

  cobegin {
  writeln("hi");
  writeln("hi");
  }

  cobegin {
    writeln("hi");
      writeln("hi");
  }

  cobegin {
    writeln("hi"); writeln("hi");
  }

  cobegin {
    writeln("hi");
    for 1..10 do
    writeln("hi");
  }

  cobegin with (ref dummy)
  {
    writeln("hi");
    writeln("??");
  }

  cobegin with (ref dummy)
  {
  writeln("hi");
  writeln("hi");
  }

  cobegin with (ref dummy) {
  writeln("hi");
  writeln("hi");
  }

  cobegin with (ref dummy) {
    writeln("hi");
      writeln("hi");
  }

  cobegin with (ref dummy) {
    writeln("hi"); writeln("hi");
  }

  cobegin with (ref dummy) {
    writeln("hi");
    for 1..10 do
    writeln("hi");
  }

  enum e1
  {
    first,
    second
  }

  enum e2
  {
  first
  }

  enum e3 {
  first
  }

  enum e4 {
    first,
      second
  }

  enum e5 {
    first, second
  }

  union u1
  {
    var element: int;
    proc firstProc() {}
  }

  union u2
  {
    var element: int;

  proc firstProc() {}
  }

  union u3 {
    var element: int;

  proc firstProc() {}
  }

  union u4 {
    var element: int;

    proc firstProc() {}
      proc secondProc() {}
  }

  union u5 {
    var element: int;

    proc firstProc() {} proc secondProc() {}
  }

  enum color { red, green, blue }; // semicolon warning does not issue bad indentation

  // Since locations are incorrectly reported with 'public' and 'private',
  // these shouldn't warn.
  module M7 {
    proc f1 {}
      proc g1 {}
    public proc f2 {}
      public proc g2 {}
    private proc f3 {}
      private proc g3 {}

    use super.M1;
      use super.M1;
    public use super.M2;
      public use super.M2;
    private use super.M2;
      private use super.M2;
  }


  @chplcheck.ignore("IncorrectIndentation")
  module DirectChildrenNotIndented {
  proc f1()
  {
    writeln("hi");
    writeln("??");
  }

  proc f2()
  {
  writeln("hi");
  }

  @chplcheck.ignore("IncorrectIndentation")
  proc f3() {
  writeln("hi");
  }

  proc f4() {
    writeln("hi");
      writeln("hi");
  }

  proc f5() {
    writeln("hi"); writeln("hi");
  }

  proc f6() {
    for 1..10 do
    writeln("hi");
  }
  }


if 1 < 2 {
  writeln("hi");
  if 2 < 3 {
    writeln("hi");
    writeln("??");
  }
}
  // since else statements aren't reported correctly only the misaligned child statements should warn
  if 1 < 2 {
    writeln("hi");
      writeln("??");
  } else if 2 < 3 {
      writeln("hi");
    writeln("??");
 } else {
      writeln("hi");
    writeln("??");
  }
  if 1 < 2 {
  if 3 < 4 {

  }
  }
}
