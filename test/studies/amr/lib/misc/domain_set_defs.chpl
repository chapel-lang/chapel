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

  def add(D: domain) where D.type==domains.eltType {
    var new_high_idx = indices.high + 1;
    indices = [1..new_high_idx];
    domains(new_high_idx) = D;
  }

  def add(set: DomainSet) 
  where set.rank==rank && set.stridable==stridable
  {
    for D in set.domains do
      add(D);
  }

}
//<=== DomainSet class <===



//===> Overload range*domain and domain*range ===>
def *(R: range(stridable=?s), D: domain)
{
  param stridable = s || D.dim(1).stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  ranges(1) = R;
  for i in [1..D.rank] do ranges(i+1) = D.dim(i);

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}

def *(D: domain, R: range(stridable=?s))
{
  param stridable = s || D.dim(1).stridable;

  var ranges: (D.rank+1)*range(stridable=stridable);
  for i in [1..D.rank] do ranges(i) = D.dim(i);
  ranges(D.rank+1) = R;

  var D_new: domain(D.rank+1,stridable=stridable) = ranges;
  return D_new;
}
//<=== Overload range*domain and domain*range <===





//===> Overload range*DomainSet ===>
def *(R: range(stridable=?s), set: DomainSet) 
{
  param stridable = s || set.stridable;

  var new_set = new DomainSet(set.rank+1, stridable=stridable);
  for D in set.domains do
    new_set.add(R*D);
  return new_set;
}
//<=== Overload range*DomainSet <===




//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> Overload subtraction: DomainSet = domain - domain ===>
//|_________________________________________________________/
def -(D: domain, E: domain) where D.rank == E.rank
{
  param stridable = D.dim(1).stridable || E.dim(1).stridable;

  for i in [1..D.rank] do
    assert(D.dim(i).stride == E.dim(i).stride);

  var stride1 = D.dim(1).stride;

  var set = new DomainSet(D.rank, stridable=stridable);
  var D1low, D1high, E1low, E1high, slice_low, slice_high: int;


  D1low  = D.dim(1).low;
  D1high = D.dim(1).high;
  E1low  = E.dim(1).low;
  E1high = E.dim(1).high;

  if E1low > D1high  ||  E1high < D1low then  // D and E are disjoint
    set.add(D);
  else {
    slice_low  = max(D1low,  E1low);
    slice_high = min(D1high, E1high);

    //===> Rank 1: Terminal case ===>
    if D.rank == 1 {

      //==== Add the low section, if it exists ====
      if D1low < slice_low then
	set.add( [D1low .. slice_low-stride1 by stride1] );

      //==== Add the high section, if it exists ====
      if D1high >slice_high then
	set.add( [slice_high+stride1 .. D1high by stride1] );
    }
    //<=== Rank 1: Terminal case <===


    //===> Higher rank case ===>
    else {
      //==== Declarations ====
      var D_projected:  domain(D.rank-1, stridable=stridable);
      var E_projected:  domain(D.rank-1, stridable=stridable);
      var interior_set: DomainSet(D.rank, stridable=stridable);
      var ranges:       (D.rank-1) * range(stridable=stridable);

      //==== Calculate projections of D and E onto upper dimensions ====
      for i in [1..D.rank-1] do ranges(i) = D.dim(i+1);
      D_projected = ranges;
      for i in [1..D.rank-1] do ranges(i) = E.dim(i+1);
      E_projected = ranges;


      //==== Add the low section, if it exists ====
      if D1low < slice_low then
	set.add( (D1low .. slice_low-stride1) * D_projected );

      //==== Compute the interior DomainSet, and add ====
      interior_set = (slice_low..slice_high by stride1) 
	             * (D_projected - E_projected);
      set.add(interior_set);

      //==== Add the high section, if it exists ====
      if D1high > slice_high then
	set.add( (slice_high+stride1 .. D1high by stride1) * D_projected );

    }
    //<=== Higher rank case <===
  }

  return set;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== Overload subtraction: DomainSet = domain - domain <===|
// \_________________________________________________________|








//|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
//|===> Overload subtraction: DomainSet = DomainSet - domain ===>
//|____________________________________________________________/
def -(set: DomainSet, E: domain) 
  where set.rank==E.rank && set.stridable==E.dim(1).stridable
{
  var new_set = new DomainSet(set.rank, set.stridable);
  for D in set.domains do new_set.add(D-E);
  return new_set;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== Overload subtraction: DomainSet = DomainSet - domain <===|
// \____________________________________________________________|




var set = new DomainSet(2, stridable=true);
set.add([1..9 by 2, 1..9 by 2]);
set.add([11..15 by 2,1..7 by 2]);

var new_set = set - [5..11 by 2,-3..3 by 2];
writeln(new_set.domains);


