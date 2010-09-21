//===> DomainSet class ===>
//-------------------------------------------------------------
// This class stores a list (associative domain) of arithmetic
// domains.
//-------------------------------------------------------------
class DomainSet {

  param rank: int;
  param stridable = false;

  var indices = [1..0];
  var domains: [indices] domain(rank, stridable=stridable);

  def add(D: domain) 
    where D.type==domains.eltType
  {
    var new_high_idx = indices.high + 1;
    indices = [1..new_high_idx];
    domains(new_high_idx) = D;
  }

  def add(d_set: DomainSet) 
    where d_set.rank==rank && d_set.stridable==stridable
  {
    for D in d_set.domains do
      add(D);
  }

  def these() {
    for D in domains do yield D;
  }

}
//<=== DomainSet class <===



//|""""""""""""""""""""""""""""""""\
//|===> Multiplication overloads ===>
//|________________________________/
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

//==== range * DomainSet ====
def *(R: range(stridable=?s), set: DomainSet) 
{
  param stridable = s || set.stridable;

  var new_set = new DomainSet(set.rank+1, stridable=stridable);
  for D in set.domains do
    new_set.add(R*D);
  return new_set;
}
// /""""""""""""""""""""""""""""""""/
//<=== Multiplication overloads ===<
// \________________________________\









//|"""""""""""""""""""""""""""""\
//|===> Subtraction overloads ===>
//|_____________________________/

//|:::::::::::::::::::::::::::::::::::\
//|===> DomainSet = domain - domain ===>
//|:::::::::::::::::::::::::::::::::::/
def -(D: domain, E: domain) where D.rank == E.rank
{
  param stridable = D.stridable || E.stridable;
  param rank = D.rank;

  //==== Require strides to be the same ====
  //------------------------------------------------------------
  // It might be possible to make sense out of this for unequal
  // strides, but that's not a functionality I need.
  //------------------------------------------------------------
  for i in 1..D.rank do
    assert(D.dim(i).stride == E.dim(i).stride);


  var d_set = new DomainSet(D.rank, stridable=stridable);

  //==== Extract data from first dimension ====
  var stride1 = D.dim(1).stride;
  var D1low  = D.dim(1).low;
  var D1high = D.dim(1).high;
  var E1low  = E.dim(1).low;
  var E1high = E.dim(1).high;

  if E1low > D1high  ||  E1high < D1low then  // D and E are disjoint
    d_set.add(D);
  else {
    var interior1_low  = max(D1low,  E1low);
    var interior1_high = min(D1high, E1high);

    //===> Rank 1: Terminal case ===>
    if rank == 1 {

      //==== Add the low section, if it exists ====
      if D1low < interior1_low {
        if stridable then
          d_set.add( [D1low .. interior1_low-stride1 by stride1] );
        else
          d_set.add( [D1low .. interior1_low-1] );
      }

      //==== Add the high section, if it exists ====
      if D1high > interior1_high {
        if stridable then
          d_set.add( [interior1_high+stride1 .. D1high by stride1] );
        else
          d_set.add( [interior1_high+1 .. D1high] );
      }
    }
    //<=== Rank 1: Terminal case <===


    //===> Higher rank case ===>
    else {
      //==== Declarations ====
      var D_projected:  domain(rank-1, stridable=stridable);
      var E_projected:  domain(rank-1, stridable=stridable);
      var interior_set: DomainSet(rank, stridable=stridable);
      var ranges:       (rank-1) * range(stridable=stridable);

      //==== Calculate projections of D and E onto upper dimensions ====
      for i in 1..rank-1 do ranges(i) = D.dim(i+1);
      D_projected = ranges;
      for i in 1..rank-1 do ranges(i) = E.dim(i+1);
      E_projected = ranges;


      //==== Add the low section, if it exists ====
      if D1low < interior1_low {
        if stridable then
          d_set.add( (D1low .. interior1_low-stride1 by stride1) * D_projected );
        else
          d_set.add( (D1low .. interior1_low-1) * D_projected );
      }

      //==== Compute the interior DomainSet, and add ====
      if stridable then
        d_set.add( (interior1_low ..interior1_high by stride1) * (D_projected - E_projected) );
      else
        d_set.add( (interior1_low .. interior1_high) * (D_projected - E_projected) );

      //==== Add the high section, if it exists ====
      if D1high > interior1_high {
        if stridable then
          d_set.add( (interior1_high + stride1 .. D1high by stride1) * D_projected );
        else
          d_set.add( (interior1_high+1 .. D1high) * D_projected);
      }
    }
    //<=== Higher rank case <===
  }

  return d_set;

}
// /:::::::::::::::::::::::::::::::::::/
//<=== DomainSet = domain - domain ===<
// \:::::::::::::::::::::::::::::::::::\


//|::::::::::::::::::::::::::::::::::::::\
//|===> DomainSet = DomainSet - domain ===>
//|::::::::::::::::::::::::::::::::::::::/
def -(d_set: DomainSet, E: domain) 
  where d_set.rank==E.rank && d_set.stridable==E.dim(1).stridable
{
  var new_set = new DomainSet(d_set.rank, d_set.stridable);
  for D in d_set.domains do new_set.add(D-E);
  return new_set;

}
// /::::::::::::::::::::::::::::::::::::::/
//<=== DomainSet = DomainSet - domain ===<
// \::::::::::::::::::::::::::::::::::::::\

// /"""""""""""""""""""""""""""""/
//<=== Subtraction overloads ===<
// \_____________________________\



def main {
  
  var full_D = [1..10, 1..10];
 
  var d_set = new DomainSet(2, stridable=false);
  d_set.add(full_D);
  d_set -= [3..5, 4..9];
  
  var A: [full_D] int;
  for D in d_set do A(D) = 1;
  
  writeln(A);

}
