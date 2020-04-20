proc main {
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





//|\""""""""""""""""""""""""""""""""|\
//| >    Scalar-tuple arithmetic    | >
//|/________________________________|/

proc =(ref T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  for i in 0..#T.size do
    T(i) = a;
}

proc +(a,T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = a+T(i);
  return U;
}

proc +(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = T(i)+a;
  return U;
}

proc -(a,T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = a-T(i);
  return U;
}

proc -(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = T(i)-a;
  return U;
}

proc -(T)
  where isTuple(T) && isHomogeneousTuple(T)
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = -T(i);
  return U;
}

proc *(a, T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = a*T(i);
  return U;;
}

proc *(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = T(i)*a;
  return U;;
}

proc /(a, T)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = a/T(i);
  return U;;
}

proc /(T,a)
  where isTuple(T) && isHomogeneousTuple(T) && a.type==T(0).type
{
  var U: T.type;
  for i in 0..#T.size do
    U(i) = T(i)/a;
  return U;;
}

proc abs(T)
  where isTuple(T) && isHomogeneousTuple(T) 
    && (T(0).type==int || T(0).type==real)
{
  var U: T.type;
  for i in 0..#T.size do
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
proc **(R: range(stridable=?s), param n: int) {
  var ranges: n*R.type;
  for i in 0..#n do ranges(i) = R;

  var D: domain(n,idxType=R.idxType,stridable=s) = ranges;
  return D;
}

//==== range * domain ====
proc *(R: range(stridable=?s), D: domain)
{
  param stridable = s || D.stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  ranges(0) = R;
  for i in 1..D.rank do ranges(i) = D.dim(i);

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}

//==== domain * range ====
proc *(D: domain, R: range(stridable=?s))
{
  param stridable = s || D.stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  for i in 0..#D.rank-1 do ranges(i) = D.dim(i);
  ranges(D.rank) = R;

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}

//==== domain * domain ====
proc *(D1: domain, D2: domain)
{
  param stridable = D1.stridable || D2.stridable;
  param rank = D1.rank + D2.rank;

  var ranges: rank*range(stridable=stridable);
  for i in 0..#D1.rank do
    ranges(i) = D1.dim(i);
  for i in 0..#D2.rank do
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
proc tuplify(obj) {
  if isTuple(obj) then return obj;
  else return (obj,);
}
// /|""""""""""""""""""""""""/|
//< |    tuplify routine    < |
// \|________________________\|





