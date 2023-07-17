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
 var ownC0 = new owned C();
 var c0: borrowed C = ownC0.borrow();
 c0.checkMe();
 var ownC1 = new owned C("hi", "hi", blah.c_str());
 var c1 = ownC1.borrow();
 c1.checkMe();
 var ownC2 = new owned C(blah, blah, blah.c_str());
 var c2 = ownC2.borrow();
 c2.checkMe();
}
