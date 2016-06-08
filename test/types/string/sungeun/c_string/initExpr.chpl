use checkType;

// scalars
var blah = "blah";
checkType(blah.type);

var blah1: string = "blah";
checkType(blah1.type);

var blah2: c_string = "blah";
checkType(c_string, blah2.type);

// domains
var D = { "blah" };
checkType(D.idxType);

var D1: domain(string) = { "blah", "blah2" };
checkType(D1.idxType);

var D2: domain(string) = { "blah":string, "blah2" };
checkType(D2.idxType);

// arrays
var A = [ "hi", "bye", "why" ];
checkType(A.eltType);

var A1: [3..5] string = [ "hi", "bye", "why" ];
checkType(A1.eltType);

// classes with scalars
{
  class C {
    var blah = "blah";
    var blah1: string = "blah";
    var blah2: c_string = "blah";
    proc checkMe() {
      checkType(blah.type);
      checkType(blah1.type);
      checkType(c_string, blah2.type);
    }
  }

 const blah = "blah";
 var c0: C;
 c0.checkMe();
 var c1 = new C("hi", "hi", blah.c_str());
 c1.checkMe();
 var c2 = new C(blah, blah, blah.c_str());
 c2.checkMe();
}
