@sometool.attribute
proc foo() { };

@linter.ignoreField("x")
class C {
  var x : int;
  @chpldoc.nodoc
  proc bar() { }
}

@chpldoc.nodoc
proc foo() { }
