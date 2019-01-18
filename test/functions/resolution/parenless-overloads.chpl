// Checknig resolution of paren-less functions overloaded via where-clause.
//
// The three versions differ whether a where-clause is present:
//  1 - in both overloads
//  2 - only in the desired overload
//  3 - only in the other overload
//
// The a/b versions switch the order of the overloads.
//
// The different returned types avoid confusion in compiler error reporting.
//

record R { param s:string; }

type shapeType = void;

// standinType() with parens

proc testParen1a() {

  proc standinType() type  where shapeType == void  return R("p1a-yes");
  proc standinType() type  where shapeType != void  return R("p1a-no");

  type ttt = standinType();
  compilerWarning(ttt:string, 0);
}

proc testParen1b() {

  proc standinType() type  where shapeType != void  return R("p1b-no");
  proc standinType() type  where shapeType == void  return R("p1b-yes");

  type ttt = standinType();
  compilerWarning(ttt:string, 0);
}

proc testParen2a() {

  proc standinType() type  where shapeType == void  return R("p2a-yes");
  proc standinType() type                           return R("p2a-no");

  type ttt = standinType();
  compilerWarning(ttt:string, 0);
}

proc testParen2b() {

  proc standinType() type                           return R("p2b-no");
  proc standinType() type  where shapeType == void  return R("p2b-yes");

  type ttt = standinType();
  compilerWarning(ttt:string, 0);
}

proc testParen3a() {

  proc standinType() type                           return R("p3a-yes");
  proc standinType() type  where shapeType != void  return R("p3a-no");

  type ttt = standinType();
  compilerWarning(ttt:string, 0);
}

proc testParen3b() {

  proc standinType() type  where shapeType != void  return R("p3b-no");
  proc standinType() type                           return R("p3b-yes");

  type ttt = standinType();
  compilerWarning(ttt:string, 0);
}

// paren-less standinType

proc testNops1a {

  proc standinType type  where shapeType == void  return R("n1a-yes");
  proc standinType type  where shapeType != void  return R("n1a-no");

  type ttt = standinType;
  compilerWarning(ttt:string, 0);
}

proc testNops1b {

  proc standinType type  where shapeType != void  return R("n1b-no");
  proc standinType type  where shapeType == void  return R("n1b-yes");

  type ttt = standinType;
  compilerWarning(ttt:string, 0);
}

proc testNops2a {

  proc standinType type  where shapeType == void  return R("n2a-yes");
  proc standinType type                           return R("n2a-no");

  type ttt = standinType;
  compilerWarning(ttt:string, 0);
}

proc testNops2b {

  proc standinType type                           return R("n2b-no");
  proc standinType type  where shapeType == void  return R("n2b-yes");

  type ttt = standinType;
  compilerWarning(ttt:string, 0);
}

proc testNops3a {

  proc standinType type                           return R("n3a-yes");
  proc standinType type  where shapeType != void  return R("n3a-no");

  type ttt = standinType;
  compilerWarning(ttt:string, 0);
}

proc testNops3b {

  proc standinType type  where shapeType != void  return R("n3b-no");
  proc standinType type                           return R("n3b-yes");

  type ttt = standinType;
  compilerWarning(ttt:string, 0);
}

// call the above

testParen1a();
testParen1b();
testParen2a();
testParen2b();
testParen3a();
testParen3b();

testNops1b;
testNops2b;
testNops3b;
// as of this writing, the following do not work (correctly)
testNops2a;
testNops1a;
testNops3a;

compilerError("done");
