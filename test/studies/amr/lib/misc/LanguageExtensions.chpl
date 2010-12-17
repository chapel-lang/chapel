//|\""""""""""""""""""""""""""""""|\
//| >    Basic data structures    | >
//|/______________________________|/
class OneWayNode {
  type data_type;
  var  data: data_type;
  var  next: OneWayNode(data_type);
}

record Stack {
  type data_type;
  var top: OneWayNode(data_type);
  var size: int=0;
  
  def push(data: data_type) {
    top = new OneWayNode(data_type, data, top);
    size += 1;
  }
  
  def pop() {
    if isEmpty() then 
      halt("Attempting to pop off an empty stack.");
    var old_top = top;
    top = old_top.next;
    size -= 1;
    return old_top.data;
  }
  
  def isEmpty() {
    return top==nil;
  }
}
// /|""""""""""""""""""""""""""""""/|
//< |    Basic data structures    < |
// \|______________________________\|





//|\""""""""""""""""""""""""""""""""|\
//| >    Scalar-tuple arithmetic    | >
//|/________________________________|/
def =(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  for i in 1..T.size do
    T(i) = a;
}

def +(a,T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = a+T(i);
  return U;
}

def +(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = T(i)+a;
  return U;
}

def -(a,T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = a-T(i);
  return U;
}

def -(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = T(i)-a;
  return U;
}

def -(T)
  where isTuple(T) && isHomogeneousTuple(T)
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = -T(i);
  return U;
}

def *(a, T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = a*T(i);
  return U;;
}

def *(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = T(i)*a;
  return U;;
}

def /(a, T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = a/T(i);
  return U;;
}

def /(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(1).type
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = T(i)/a;
  return U;;
}

def abs(T)
  where isTuple(T) && isHomogeneousTuple(T) 
    && (T(1).type==int || T(1).type==real)
{
  var U: T.type;
  for i in 1..T.size do
    U(i) = abs(T(i));
  return U;
}
// /|""""""""""""""""""""""""""""""""/|
//< |    Scalar/tuple arithmetic    < |
// \|________________________________\|




//|\""""""""""""""""""""""""""""""""|\
//| >    Range/domain arithmetic    | >
//|/________________________________|/
//==== Range exponentiation ====
def **(R: range(stridable=?s), param n: int) {
  var ranges: n*R.type;
  for i in [1..n] do ranges(i) = R;

  var D: domain(n,stridable=s) = ranges;
  return D;
}

//==== range * domain ====
def *(R: range(stridable=?s), D: domain)
{
  param stridable = s || D.stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  ranges(1) = R;
  for i in [1..D.rank] do ranges(i+1) = D.dim(i);

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}

//==== domain * range ====
def *(D: domain, R: range(stridable=?s))
{
  param stridable = s || D.stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  for i in [1..D.rank] do ranges(i) = D.dim(i);
  ranges(D.rank+1) = R;

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}

//==== domain * domain ====
def *(D1: domain, D2: domain)
{
  param stridable = D1.stridable || D2.stridable;
  param rank = D1.rank + D2.rank;

  var ranges: rank*range(stridable=stridable);
  for i in 1..D1.rank do
    ranges(i) = D1.dim(i);
  for i in 1..D2.rank do
    ranges(D1.rank+i) = D2.dim(i);

  var D_new: domain(rank,stridable=stridable) = ranges;
  return D_new;
}
// /|""""""""""""""""""""""""""""""""/|
//< |    Range/domain arithmetic    < |
// \|________________________________\|




//|\""""""""""""""""""""""""|\
//| >    tuplify routine    | >
//|/________________________|/
//-------------------------------------------------------------
// If object is a tuple, returns the object itself; otherwise,
// encloses the object in a tuple.  Useful for handling the
// 1D/tuple problem.
//-------------------------------------------------------------
def tuplify(obj) {
  if isTuple(obj) then return obj;
  else return tuple(obj);
}
// /|""""""""""""""""""""""""/|
//< |    tuplify routine    < |
// \|________________________\|



//|\"""""""""""""""""""""""""""""""|\
//| >    IndependentArray class    | >
//|/_______________________________|/
//--------------------------------------------------------------
// This class is designed to enable an "array of arrays".  The
// array is "independent" in that its domain is tethered to it.
// This ends up working like a Fortran allocatable array; after
// it is declared, the IndependentArray may be "allocated" by
// setting its Domain.
//--------------------------------------------------------------
class IndependentArray {
  param rank: int;
  param stridable: bool;
  type  eltType;
  
  var Domain: domain(rank=rank, stridable=stridable);
  var value: [Domain] eltType;
}
// /|"""""""""""""""""""""""""""""""/|
//< |    IndependentArray class    < |
// \|_______________________________\|



def main {
  var T = (1,5,-2);
  writeln("T   = ", T);
  writeln("T+1 = ", T+1);
  writeln("1+T = ", 1+T);
  writeln("T-1 = ", T-1);
  writeln("1-T = ", 1-T);
  writeln("-T  = ", -T);
  
  T = 3;
  writeln(T);
}