// This tests the explanation for the unresolved call error
// where the call is paren-less and the function is paren-ful.
//
// This has to be a method. If it is a call to a non-method function
// written without parens (and without args), ex. "myFun;", it is
// currently treated as capturing 'myFun' as a first-class function (FCF),
// and does not result in an error.

class C {
  proc mm() {
    writeln("in mm()");
  }
}

var cc = new C();
cc.mm;
