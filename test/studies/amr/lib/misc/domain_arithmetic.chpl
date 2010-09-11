//==== range*range multiplication ====
def *(R1: range(stridable=?s1), R2: range(stridable=?s2))
{
  var D: domain(2,stridable=(s1||s2)) = (R1, R2);
  return D;
}


//==== range*domain multiplication ====
def *(R: range(stridable=?s), D: domain)
{
  param stridable = s || D.dim(1).stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  ranges(1) = R;
  for i in [1..D.rank] do ranges(i+1) = D.dim(i);

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}


//==== domain*range multiplication ====
def *(D: domain, R: range(stridable=?s))
{
  param stridable = s || D.dim(1).stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  for i in [1..D.rank] do ranges(i) = D.dim(i);
  ranges(D.rank+1) = R;

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}


//==== range exponentiation ====
def **(R: range(stridable=?s), param n: int) {
  var ranges: n*R.type;
  for i in [1..n] do ranges(i) = R;

  var D: domain(n,stridable=s) = ranges;
  return D;
}




/* def main { */

/*   var R1 = 1..5; */
/*   var R2 = 1..10 by 3; */

/*   writeln(R1*R2); */
/*   writeln(R1**3); */
/*   writeln(R2**5); */

/* } */