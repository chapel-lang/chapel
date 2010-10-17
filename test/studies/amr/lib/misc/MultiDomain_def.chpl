
use LanguageExtensions;


//|\""""""""""""""""""""""""""|\
//| >    MultiDomain class    | >
//|/__________________________|/
//-------------------------------------------------------------
// Stores a list of arithmetic domains.  Allows the subraction
// of one domain from a collection of other domains.  Useful
// for dealing with irregular but rectangular geometries.
//-------------------------------------------------------------
class MultiDomain {

  param rank:      int;
  param stridable: bool = false;
  var   stride:    rank*int;

  var indices: domain(1) = [1..0];
  var domains: [indices] domain(rank, stridable=stridable);


  //|\'''''''''''''''''''''''|\
  //| >    these iterator    | >
  //|/.......................|/
  //-----------------------------------
  // Iterate over the list of domains.
  //-----------------------------------
  def these() {
    for D in domains do yield D;
  }
  // /|'''''''''''''''''''''''/|
  //< |    these iterator    < |
  // \|.......................\|


  def length { return indices.high; }


  def writeThis(w: Writer){
    write(domains);
  }

}
// /|""""""""""""""""""""""""""/|
//< |    MultiDomain class    < |
// \|__________________________\|




//|\""""""""""""""""""""""""""""""|\
//| >    MultiDomain.intersect    | >
//|/______________________________|/
//--------------------------------------------------
// Replace each element of the domain list with its 
// intersection with the input domain.
//--------------------------------------------------
def MultiDomain.intersect(D_in)
  where D_in.type == domains.eltType
{
  
  var new_mD = new MultiDomain(rank,stridable=stridable);
  
  for D in domains {
    var intersection = D(D_in);
    if intersection.numIndices > 0 then
      new_mD.add(intersection);
  }
  
  indices = new_mD.indices;
  domains = new_mD.domains;
  
  delete new_mD;

}
// /|""""""""""""""""""""""""""""""/|
//< |    MultiDomain.intersect    < |
// \|______________________________\|





//|\""""""""""""""""""""""""""""""""|\
//| >    MultiDomain.add methods    | >
//|/________________________________|/
def MultiDomain.add(D: domain) 
  where D.type==domains.eltType
{
  if D.numIndices>0 {
    if this.length==0 then
      stride = tuplify(D.stride);
    else assert(tuplify(D.stride) == stride,
                "error: Elements of a MultiDomain must have equal stride.");

    indices = [1..indices.high+1];
    domains(indices.high) = D;
  }
}


def MultiDomain.add(mD: MultiDomain) 
  where mD.rank==rank && mD.stridable==stridable
{
  // writeln("this.length = ", this.length);
  // writeln("this.stride = ", this.stride);
  // writeln("mD.stride = ", mD.stride);
  // writeln("mD.length = ", mD.length);
  
  if this.length>0 && mD.length>0 then
    assert(mD.stride == stride);
  else if this.length==0 && mD.length>0 then
    stride = mD.stride;
    
  
  const prev_high = indices.high;
  indices = [1..prev_high + mD.indices.high];
  domains(prev_high+1..indices.high) = mD.domains;
}
// /|""""""""""""""""""""""""""""""""/|
//< |    MultiDomain.add methods    < |
// \|________________________________\|









//|\"""""""""""""""""""""""""""""""""""|\
//| >    MultiDomain multiplication    | >
//|/___________________________________|/
//==== range * MultiDomain ====
def *(R: range(stridable=?s), mD: MultiDomain) 
{
  param stridable = s || mD.stridable;

  var mD_new = new MultiDomain(mD.rank+1, stridable=stridable);
  for D in mD.domains do mD_new.add(R*D);
  return mD_new;
}

//==== domain * MultiDomain ====
def *(D: domain, mD: MultiDomain)
{
  param stridable = D.stridable || mD.stridable;

  var mD_new = new MultiDomain(D.rank+mD.rank, stridable=stridable);
  for E in mD.domains do
    mD_new.add(D*E);

  return mD_new;
}

//==== MultiDomain * domain ====
def *(mD: MultiDomain, E: domain)
{
  param stridable = mD.stridable || E.stridable;
  var mD_new = new MultiDomain(mD.rank+E.rank, stridable=stridable);
  for D in mD do mD_new.add(D*E);

  return mD_new;
}

//==== MultiDomain * MultiDomain ====
def *(mD1: MultiDomain, mD2: MultiDomain)
{
  param stridable = mD1.stridable || mD2.stridable;

  var mD_new = new MultiDomain(mD1.rank+mD2.rank, stridable=stridable);
  for (D1,D2) in [mD1,mD2] do
    mD_new.add(D1*D2);

  return mD_new;
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    MultiDomain multiplication    < |
// \|___________________________________\|









//|\""""""""""""""""""""""""""""""|\
//| >    Subtraction overloads    | >
//|/______________________________|/

//|\''''''''''''''''''''''''''''''''''''''|\
//| >    MultiDomain = domain - domain    | >
//|/......................................|/
def -(D: domain, E: domain) where D.rank == E.rank
{
  param stridable = D.stridable || E.stridable;
  param rank = D.rank;

  //==== Require strides to be the same ====
  //------------------------------------------------------------
  // It might be possible to make sense out of this for unequal
  // strides, but that's not a functionality I need.
  //------------------------------------------------------------
  assert(D.stride == E.stride,
         "error: -(D: domain, E: domain)\n" +
         "Strides of domains must be equal.");


  var mD = new MultiDomain(D.rank, stridable=stridable);

  //==== Extract data from first dimension ====
  //-----------------------------------------------------------
  // The syntax here seems a little bulky, but it ensures that
  // the results are always integers, regardless of D.rank.
  //-----------------------------------------------------------
  var stride1 = D.dim(1).stride;
  var D1low  =  D.dim(1).low;
  var D1high =  D.dim(1).high;
  var E1low  =  E.dim(1).low;
  var E1high =  E.dim(1).high;

  if E1low > D1high  ||  E1high < D1low then  // D and E are disjoint
    mD.add(D);
  else {
    var interior1_low  = max(D1low,  E1low);
    var interior1_high = min(D1high, E1high);

    //===> Rank 1: Terminal case ===>
    if rank == 1 {

      //==== Add the low section, if it exists ====
      if D1low < interior1_low {
        if stridable then
          mD.add( [D1low .. interior1_low-stride1 by stride1] );
        else
          mD.add( [D1low .. interior1_low-1] );
      }

      //==== Add the high section, if it exists ====
      if D1high > interior1_high {
        if stridable then
          mD.add( [interior1_high+stride1 .. D1high by stride1] );
        else
          mD.add( [interior1_high+1 .. D1high] );
      }
    }
    //<=== Rank 1: Terminal case <===


    //===> Rank > 1 ===>
    else {
      //==== Declarations ====
      var D_projected: domain(rank-1, stridable=stridable);
      var E_projected: domain(rank-1, stridable=stridable);
      var ranges:      (rank-1) * range(stridable=stridable);

      //==== Calculate projections of D and E onto upper dimensions ====
      for i in 1..rank-1 do ranges(i) = D.dim(i+1);
      D_projected = ranges;
      for i in 1..rank-1 do ranges(i) = E.dim(i+1);
      E_projected = ranges;


      //==== Add the low section, if it exists ====
      if D1low < interior1_low {
        if stridable then
          mD.add( (D1low .. interior1_low-stride1 by stride1) * D_projected );
        else
          mD.add( (D1low .. interior1_low-1) * D_projected );
      }

      //==== Compute the interior MultiDomain, and add ====
      var projected_diff: MultiDomain(rank-1,stridable=stridable) = D_projected - E_projected;
      var mD_interior:    MultiDomain(rank,stridable=stridable);
      if stridable then
        mD_interior = (interior1_low .. interior1_high by stride1) * projected_diff;
      else
        mD_interior = (interior1_low .. interior1_high) * projected_diff;
      mD.add(mD_interior);
      delete projected_diff;
      delete mD_interior;

      //==== Add the high section, if it exists ====
      if D1high > interior1_high {
        if stridable then
          mD.add( (interior1_high + stride1 .. D1high by stride1) * D_projected );
        else
          mD.add( (interior1_high+1 .. D1high) * D_projected );
      }
    }
    //<=== Rank > 1 <===
  }

  return mD;

}
// /|''''''''''''''''''''''''''''''''''''''/|
//< |    MultiDomain = domain - domain    < |
// \|......................................\|


//|\'''''''''''''''''''''''''''''''''''''''''''|\
//| >    MultiDomain = MultiDomain - domain    | >
//|/...........................................|/
def -(mD: MultiDomain, E: domain) 
  where mD.rank==E.rank && mD.stridable==E.dim(1).stridable
{
  var mD_new = new MultiDomain(mD.rank, mD.stridable);
  
  for D in mD {
    var difference = D-E;
    mD_new.add(difference);
    delete difference;
  }
  
  return mD_new;

}
// /|'''''''''''''''''''''''''''''''''''''''''''/|
//< |    MultiDomain = MultiDomain - domain    < |
// \|...........................................\|



//|\''''''''''''''''''''''''''''''''''''''''''''''''|\
//| >    MultiDomain = MultiDomain - MultiDomain    | >
//|/................................................|/
def -(mD1: MultiDomain, mD2: MultiDomain)
  where mD1.rank==mD2.rank && mD1.stridable==mD2.stridable
{
  var mD_new = new MultiDomain(mD1.rank, mD1.stridable);

  mD_new.add(mD1);

  for E in mD2 {
    var mD_temp = mD_new - E;
    delete mD_new;
    mD_new = mD_temp;
  }
  
  return mD_new;
}
// /|''''''''''''''''''''''''''''''''''''''''''''''''/|
//< |    MultiDomain = MultiDomain - MultiDomain    < |
// \|................................................\|


// /|""""""""""""""""""""""""""""""/|
//< |    Subtraction overloads    < |
// \|______________________________\|





//|\"""""""""""""""""""""""""""""|\
//| >    MultiDomain.subtract    | >
//|/_____________________________|/
def MultiDomain.subtract(E: domain)
  where E.type == domains.eltType
{
  if domains.numElements>0 then
    assert(tuplify(E.stride) == stride,
	         "error: Elements of a MultiDomain must have equal stride.");

  var difference = this - E;
  this.indices = difference.indices;
  this.domains = difference.domains;
  delete difference;
}
// /|"""""""""""""""""""""""""""""/|
//< |    MultiDomain.subtract    < |
// \|_____________________________\|





//|\"""""""""""""""""""""""""|\
//| >    MultiArray class    | >
//|/_________________________|/
//-----------------------------------------------------
// A collection of arrays, designed to store values on
// all domains in a MultiDomain.  Enforcement of this
// is fairly weak.
//-----------------------------------------------------
class MultiArray {
  
  param rank: int;
  param stridable: bool;
  type  eltType;
  
  var indices: domain(1) = [1..0];
  var arrays: [indices] IndependentArray(rank, stridable, eltType);
  
  def length return indices.high;
  
  //|\'''''''''''''''''''''''|\
  //| >    these iterator    | >
  //|/.......................|/
  //------------------------------------------------------
  // Iterates over the IndependentArrays that make up the
  // MultiArray.
  //------------------------------------------------------
  def these() {
    for array in arrays do yield array;
  }
  // /|'''''''''''''''''''''''/|
  //< |    these iterator    < |
  // \|.......................\|


  //|\''''''''''''''''''''''''|\
  //| >    allocate method    | >
  //|/........................|/
  //----------------------------------------------------------
  // Allocates storage in the MultiArray corresponding to the
  // domains in a MultiDomain.
  //----------------------------------------------------------
  def allocate(mD: MultiDomain(rank,stridable)) {
    indices = mD.indices;
    for i in indices {
      arrays(i) = new IndependentArray(rank, stridable, eltType);
      arrays(i).Domain = mD.domains(i);      
    }
  }
  // /|''''''''''''''''''''''''/|
  //< |    allocate method    < |
  // \|........................\|
  
}
// /|"""""""""""""""""""""""""/|
//< |    MultiArray class    < |
// \|_________________________\|






// //|\""""""""""""""""""""""""""|\
// //| >    equality overload    | >
// //|/__________________________|/
// def ==(S1: MultiDomain, S2: MultiDomain) {
//   var S_diff = S1 - S2;
// 
//   if S_diff.indices.numIndices==0 then
//     return true;
//   else
//     return false;
// 
// }
// // /|""""""""""""""""""""""""""/|
// //< |    equality overload    < |
// // \|__________________________\|


def main {
  
  var full_D = [1..10, 1..10];
 
  var mD = new MultiDomain(2, false);
  mD.add(full_D);
  mD.subtract([3..5, 4..9]);
  mD.subtract([4..8, 1..5]);

  var I: [full_D] int;
  for D in mD do I(D)=1;

  writeln("");
  writeln("Full domain: ", full_D);
  writeln("MultiDomain: ", mD);  
  writeln(I);

  var mArray = new MultiArray(2, false, int);
  mArray.allocate(mD);
  for A in mArray do A.value = 2;
  
  for (D,A) in (mD,mArray) do I(D) = A.value;
  writeln("");
  writeln(I);




  var full_D_strided = [1..21 by 2, -5..31 by 3];
  var mD_strided = new MultiDomain(2,true);
  mD_strided.add(full_D_strided);
  mD_strided.subtract([5..13 by 2, -5..1 by 3]);
  mD_strided.subtract([9..15 by 2, 1..38 by 3]);

  var I_strided: [full_D_strided] int;
  for D in mD_strided do I_strided(D)=1;

  writeln("");
  writeln("Full domain: ", full_D_strided);
  writeln("MultiDomain: ", mD_strided);  
  writeln(I_strided );
  
  var mArray_strided = new MultiArray(2,true,int);
  mArray_strided.allocate(mD_strided);
  for A in mArray_strided do A.value = 2;
  
  for (D,A) in (mD_strided,mArray_strided) do I_strided(D) = A.value;
  writeln("");
  writeln(I_strided);

}
