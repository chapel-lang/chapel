//===> DomainSet class ===>
//-------------------------------------------------------------
// This class stores a list (associative domain) of arithmetic
// domains.
//-------------------------------------------------------------
class DomainSet {

  param rank: int;
  param stridable = false;

  var domain_indices = [1..0];
  var domains: [domain_indices] domain(rank);
  var n_domains = 0;


  def add(D) where D.rank == rank {
    var new_high_idx = domain_indices.high + 1;
    domain_indices = [1..new_high_idx];
    domains(new_high_idx) = D;
  }

  def add(set: DomainSet) where set.rank == rank {
    for D in set.domains do
      add(D);
  }

}
//<=== DomainSet class <===



//===> Overload range*domain ===>
def *(R, D){
  var D_new: domain(D.rank+1);
  var ranges: (D.rank+1)*range;
  ranges(1) = R;
  for i in [1..D.rank] do
    ranges(i+1) = D.dim(i);
  D_new = ranges;
  return D_new;
}
//<=== Overload range*domain <===



//===> Overload range*DomainSet ===>
def *(R, set: DomainSet) {
  var new_set = new DomainSet(set.rank+1);
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
  var a, b, aE, bE, slice_low, slice_high: int;
  var set = new DomainSet(D.rank);

  a = D.dim(1).low;
  b = D.dim(1).high;
  aE = E.dim(1).low;
  bE = E.dim(1).high;

  if aE>b || bE<a then
    set.add(D);
  else {
    slice_low  = max(a,aE);
    slice_high = min(b,bE);

    //===> Rank 1: Terminal case ===>
    if D.rank == 1 {

      //==== Add the low section, if it exists ====
      if a < slice_low then
	set.add( [a..slice_low-1] );

      //==== Add the high section, if it exists ====
      if b >slice_high then
	set.add( [slice_high+1..b] );
    }
    //<=== Rank 1: Terminal case <===


    //===> Higher rank case ===>
    else {
      //==== Declarations ====
      var D_projected:  domain(D.rank-1);
      var E_projected:  domain(D.rank-1);
      var interior_set: DomainSet(D.rank);
      var ranges:       (D.rank-1) * range;

      //==== Calculate projections of D and E onto upper dimensions ====
      for i in [1..D.rank-1] do ranges(i) = D.dim(i+1);
      D_projected = ranges;
      for i in [1..D.rank-1] do ranges(i) = E.dim(i+1);
      E_projected = ranges;


      //==== Add the low section, if it exists ====
      if a < slice_low then
	set.add( (a..slice_low-1) * D_projected );

      //==== Compute the interior DomainSet, and add ====
      interior_set = (slice_low..slice_high) * (D_projected - E_projected);
      set.add(interior_set);

      //==== Add the high section, if it exists ====
      if b > slice_high then
	set.add( (slice_high+1..b) * D_projected );

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
def -(set: DomainSet, E: domain) where set.rank == E.rank
{
  var new_set = new DomainSet(set.rank);

  for D in set.domains do new_set.add(D-E);

  return new_set;

}
// /~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
//<=== Overload subtraction: DomainSet = DomainSet - domain <===|
// \____________________________________________________________|




var set = new DomainSet(2);
set.add([1..10,1..10]);
set.add([11..15,1..6]);

var new_set = set - [6..12,-3..3];
writeln(new_set.domains);


