/// generic record ///

record Rec {
  type t;
}

proc retsRec(type t): Rec {
  return new Rec(t);
}

var xRec = retsRec(int);
var yRec = retsRec(real);
compilerWarning(xRec.type: string);
compilerWarning(yRec.type: string);

/// generic class ///

class Gen {
  type t;
}

var GlobClass = new owned Gen(real);


/* pending #10172

/// owned, explicit ///

proc retsGenOE(type t): owned Gen {
  return new owned Gen(t);
}

var xOE = retsGenOE(int);
var yOE = retsGenOE(real);
compilerWarning(xOE.type: string);
compilerWarning(yOE.type: string);

/// shared, explicit ///

proc retsGenSE(type t): shared Gen {
  return new shared Gen(t);
}

var xSE = retsGenSE(int);
var ySE = retsGenSE(real);
compilerWarning(xSE.type: string);
compilerWarning(ySE.type: string);
*/


/// borrowed, explicit ///

proc retsGenBE(type t): borrowed Gen {
  return GlobClass; // implicit conversion to borrowed
}

var xBE = retsGenBE(int);
compilerWarning(xBE.type: string);


/// unmanaged, explicit ///

proc retsGenUE(type t): unmanaged Gen {
  return new unmanaged Gen(t);
}

var xUE = retsGenUE(int);
var yUE = retsGenUE(real);
compilerWarning(xUE.type: string);
compilerWarning(yUE.type: string);
delete xUE, yUE;


/* pending #11467

/// owned, implicit ///

proc retsGenOI(type t): owned {
  return new owned Gen(t);
}

var xOI = retsGenOI(int);
var yOI = retsGenOI(real);
compilerWarning(xOI.type: string);
compilerWarning(yOI.type: string);


/// shared, implicit ///

proc retsGenSI(type t): shared {
  return new shared Gen(t);
}

var xSI = retsGenSI(int);
var ySI = retsGenSI(real);
compilerWarning(xSI.type: string);
compilerWarning(ySI.type: string);


/// borrowed, implicit ///

proc retsGenBI(type t): borrowed {
  return GlobClass; // implicit conversion to borrowed
}

var xBI = retsGenBI(int);
compilerWarning(xBI.type: string);


/// unmanaged, implicit ///

proc retsGenUI(type t): unmanaged {
  return new unmanaged Gen(t);
}

var xUI = retsGenUI(int);
var yUI = retsGenUI(real);
compilerWarning(xUI.type: string);
compilerWarning(yUI.type: string);
delete xUI, yUI;
*/
