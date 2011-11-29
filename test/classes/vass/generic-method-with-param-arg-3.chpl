
proc xxx(aa) { return aa.getAllocationDomainForDefaultRectangularArray(3); }

proc printAllocationDomain(A:[])
{
  A._value.printAllocationDomain();
}

  proc BaseArr.getAllocationDomainForDefaultRectangularArray(param rank): rank * range(int(64), stridable=true)
  {
    halt("getAllocationDomainForDefaultRectangularArray:non-DefaultRectangular array encountered");
    var dummy: rank * range(int(64), stridable=true);
    return dummy;
  }

proc DefaultRectangularArr.printAllocationDomain(){
  writeln("In DefaultRectangularArr.printAllocationDomain");
  writeln(getAllocationDomainForDefaultRectangularArray(dom.rank));
}

proc DefaultRectangularArr.getAllocationDomainForDefaultRectangularArray(param rank): rank * range(int(64), stridable=true)
{
  if _arrAlias {
    return xxx(_arrAlias);
  } else {
    var p:rank * range(int(64), stridable=true);
    for i in 1..rank do
      p(i)=dom.ranges(i);
    return p;
  }
}

/////////////////////////////////////////////////////////////////////////////

config const m = 4;
config const n = 4;
config const o = 4;

config const printOutput = false;

var Dom = [1..m,1..n,1..o];

var Ref: [Dom] real;
var A: [Dom] real;
var B: [Dom] real;
for (a,i) in (A,[1..m*n*o]) do a = i;
for (b,i) in (B,[1..m*n*o]) do b = i+500;

var Dom2=[1..1,2..4,1..1];
var Dom1=[2..2,1..3,4..4];

printAllocationDomain(A[Dom2]);
