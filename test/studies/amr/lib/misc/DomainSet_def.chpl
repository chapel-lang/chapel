
//|\""""""""""""""""""""""""|\
//| >    DomainSet class    | >
//|/________________________|/
//-------------------------------------------------------------
// Stores a list of arithmetic domains.  Allows the subraction
// of one domain from a collection of other domains.  Useful
// for dealing with irregular but rectangular geometries.
//-------------------------------------------------------------
class DomainSet {

  param rank: int;
  param stridable = false;
  var stride: rank*int;

  var indices = [1..0];
  var domains: [indices] domain(rank, stridable=stridable);

  def add(D: domain) 
    where D.type==domains.eltType
  {
    if rank==1 {
      if indices.numIndices==0 then
	stride = tuple(D.stride);
      else assert(D.stride == stride(1),
		  "error: Elements of a DomainSet must have equal stride.");
    }
    else {
      if indices.numIndices==0 then
	stride = D.stride;
      else assert(D.stride == stride,
		  "error: Elements of a DomainSet must have equal stride.");
    }

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


  def this(D_in: domain)
    where D_in.rank==rank && D_in.stridable==stridable
  {
    var new_set = new DomainSet(rank,stridable);

    for D in domains {
      var intersection = D(D_in);
      if intersection.numIndices > 0 then
	new_set.add(intersection);
    }

    return new_set;
  }

  def writeThis(w: Writer){
    write(domains);
  }

}
// /|""""""""""""""""""""""""/|
//< |    DomainSet class    < |
// \|________________________\|




//|\"""""""""""""""""""""""""""""""""|\
//| >    Multiplication overloads    | >
//|/_________________________________|/
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

//==== domain*domain ====
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

//==== range * DomainSet ====
def *(R: range(stridable=?s), set: DomainSet) 
{
  param stridable = s || set.stridable;

  var new_set = new DomainSet(set.rank+1, stridable=stridable);
  for D in set.domains do
    new_set.add(R*D);
  return new_set;
}

//==== domain * DomainSet ====
def *(D: domain, S: DomainSet)
{
  param stridable = D.stridable || S.stridable;

  var S_new = new DomainSet(D.rank+S.rank, stridable=stridable);
  for D2 in S.domains do
    S_new.add(D*D2);

  return S_new;
}

//==== DomainSet * domain ====
def *(S: DomainSet, D: domain)
{
  param stridable = S.stridable || D.stridable;
  var S_new = new DomainSet(S.rank+D.rank, stridable=stridable);
  for D1 in S.domains do
    S_new.add(D1*D);

  return S_new;
}

//==== DomainSet * DomainSet ====
def *(S1: DomainSet, S2: DomainSet)
{
  param stridable = S1.stridable || S2.stridable;

  var S_new = new DomainSet(S1.rank+S2.rank, stridable=stridable);
  for D1 in S1.domains do
    S_new.add(D1*S2);

  return S_new;
}
// /|"""""""""""""""""""""""""""""""""/|
//< |    Multiplication overloads    < |
// \|_________________________________\|









//|\""""""""""""""""""""""""""""""|\
//| >    Subtraction overloads    | >
//|/______________________________|/

//|\''''''''''''''''''''''''''''''''''''|\
//| >    DomainSet = domain - domain    | >
//|/....................................|/
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
// /|''''''''''''''''''''''''''''''''''''/|
//< |    DomainSet = domain - domain    < |
// \|....................................\|


//|\'''''''''''''''''''''''''''''''''''''''|\
//| >    DomainSet = DomainSet - domain    | >
//|/.......................................|/
def -(d_set: DomainSet, E: domain) 
  where d_set.rank==E.rank && d_set.stridable==E.dim(1).stridable
{
  var new_set = new DomainSet(d_set.rank, d_set.stridable);
  for D in d_set.domains do new_set.add(D-E);
  return new_set;

}
// /|'''''''''''''''''''''''''''''''''''''''/|
//< |    DomainSet = DomainSet - domain    < |
// \|.......................................\|



//|\''''''''''''''''''''''''''''''''''''''''''|\
//| >    DomainSet = DomainSet - DomainSet    | >
//|/..........................................|/
def -(S1: DomainSet, S2: DomainSet)
  where S1.rank==S2.rank && S1.stridable==S2.stridable
{
  var S_new = new DomainSet(S1.rank, S1.stridable);

  for D1 in S1 do S_new.add(D1);
  for D2 in S2 do S_new -= D2;

  return S_new;
}
// /|''''''''''''''''''''''''''''''''''''''''''/|
//< |    DomainSet = DomainSet - DomainSet    < |
// \|..........................................\|


// /|""""""""""""""""""""""""""""""/|
//< |    Subtraction overloads    < |
// \|______________________________\|



//|\""""""""""""""""""""""""""|\
//| >    equality overload    | >
//|/__________________________|/
def ==(S1: DomainSet, S2: DomainSet) {
  var S_diff = S1 - S2;

  if S_diff.indices.numIndices==0 then
    return true;
  else
    return false;

}
// /|""""""""""""""""""""""""""/|
//< |    equality overload    < |
// \|__________________________\|


def main {
  
  var full_D = [1..10, 1..10];
 
  var d_set = new DomainSet(2, false);
  d_set.add(full_D);
  d_set -= [3..5, 4..9];
  writeln(d_set);

  var D1 = [1..5 by 2];
  var D2 = [4..8];
  writeln(D1*D2);
  

  writeln(D1*d_set);

  writeln(d_set*D2);

  writeln(d_set*d_set);


}
