module superKeyword {
  // 'super' should be reserved as a keyword so all of the following should fail:
  var super = 1;
  proc super { return 42; }
  iter super { yield 42; }
  module super { }
  proc foo(type super) { return 42 : super; }
}
