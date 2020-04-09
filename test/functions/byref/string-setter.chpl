// Michael Noakes
// 2016/01/13
//
// The new implementation of strings as records relies on the use of
// "copy-constructors" and associated destructors to manage string-buffer
// memory.  In some cases the use of these copy/destroy pairs results in
// observable performance regressions.
//
// One response to this is to optimize the use of functions that return
// strings with a ref-return intent.
//
// This test is designed to demonstrate that we do not introduce
// correctness regressions while doing so.


var getterString = "getter";
var setterString = "setter";


//
// Verify behavior when there is no use of the setter param
//
proc refReturnString() ref
{
  return getterString;
}
proc refReturnString()
{
  return getterString;
}


writeln(refReturnString());
writeln("");
writeln("");



//
// A simple use of the setter param
//

proc refWithSetter() ref
{
  return setterString;
}
proc refWithSetter()
{
  return getterString;
}


writeln(refWithSetter());
writeln(setterString);
refWithSetter1() = "SetterNew";
writeln(setterString);
writeln("");
writeln("");



//
// Ensure that setter is honored through a chain of by-ref calls
//

proc refWithSetter3() ref
{
  return refWithSetter2();
}
proc refWithSetter3()
{
  return refWithSetter2();
}


proc refWithSetter4() ref
{
  return refWithSetter3();
}
proc refWithSetter4()
{
  return refWithSetter3();
}


proc refWithSetter2() ref
{
  return refWithSetter1();
}
proc refWithSetter2()
{
  return refWithSetter1();
}


proc refWithSetter1()   ref
{
  return setterString;
}
proc refWithSetter1()
{
  return getterString;
}


setterString = "setter";

writeln(refWithSetter4());
writeln(setterString);
refWithSetter4() = "SetterNew";
writeln(setterString);
