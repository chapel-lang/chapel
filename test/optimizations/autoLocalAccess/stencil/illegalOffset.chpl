use StencilDist;

const n = 10;

const Dom = stencilDist.createDomain({0..<n}, fluff=(1,));
const DomInner = Dom.expand(-2);

var InArr: [Dom] int = 1;
var OutArr: [Dom] int;

InArr.updateFluff();

forall i in DomInner {
  OutArr[i] = InArr[i-"1"];
}

writeln(OutArr);
