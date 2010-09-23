//|----------------------------*
//|==== range exponentiation ===>
//|----------------------------*
def **(R: range(stridable=?s), param n: int) {
  var ranges: n*R.type;
  for i in [1..n] do ranges(i) = R;

  var D: domain(n,stridable=s) = ranges;
  return D;
}
// *----------------------------|
//<=== range exponentiation <===|
// *----------------------------|




//|-------------------------------*
//|===> Scalar/tuple arithmetic ===>
//|-------------------------------*
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
// *-------------------------------|
//<=== Scalar/tuple arithmetic <===|
// *-------------------------------|



def main {
  var T = (1,5,-2);
  writeln("T   = ", T);
  writeln("T+1 = ", T+1);
  writeln("1+T = ", 1+T);
  writeln("T-1 = ", T-1);
  writeln("1-T = ", 1-T);
  writeln("-T  = ", -T);
}