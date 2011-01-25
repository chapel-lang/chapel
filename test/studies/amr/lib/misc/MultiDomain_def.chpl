
use LanguageExtensions;


//|\"""""""""""""""""""""""""""|\
//| >    class: MultiDomain    | >
//|/___________________________|/

//-------------------------------------------------------------
// Stores a list of arithmetic domains.  Allows the subraction
// of one domain from a collection of other domains.  Useful
// for dealing with irregular but rectangular geometries.
//-------------------------------------------------------------

class MultiDomain {

  param rank:      int;
  param stridable: bool = false;
  var   stride:    rank*int;

  var subindices: domain(1) = [1..0];
  var domains: [subindices] domain(rank, stridable=stridable);




  //|\'''''''''''''''''''''''''''''|\
  //| >    special method: this    | >
  //|/.............................|/
  
  proc this (subindex: int)
  {
    return domains(subindex);
  }
  // /|'''''''''''''''''''''''''''''/|
  //< |    special method: this    < |
  // \|.............................\|
  
  

  //|\''''''''''''''''''''''''''''''|\
  //| >    special method: these    | >
  //|/..............................|/

  iter these () 
  {
    for D in domains do yield D;
  }
  // /|''''''''''''''''''''''''''''''/|
  //< |    speical method: these    < |
  // \|..............................\|


  proc length { return subindices.high; }



  //|\''''''''''''''''''''''''''''''''''|\
  //| >    special method: writeThis    | >
  //|/..................................|/
  
  //-----------------------------------------------------------
  // Defines the output of the intrinsic 'write' and 'writeln' 
  // procedures, so that write(MultiDomain) will produce
  // something sensible.  Mainly for testing and debugging.
  //-----------------------------------------------------------

  proc writeThis (w: Writer)
  {
    write(domains);
  }
  // /|''''''''''''''''''''''''''''''''''/|
  //< |    special method: writeThis    < |
  // \|..................................\|
  

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
proc MultiDomain.intersect(D_in)
  where D_in.type == domains.eltType
{
  
  var new_mD = new MultiDomain(rank,stridable=stridable);
  
  for D in domains {
    var intersection = D(D_in);
    if intersection.numIndices > 0 then
      new_mD.add(intersection);
  }
  
  subindices = new_mD.subindices;
  domains = new_mD.domains;
  
  delete new_mD;

}
// /|""""""""""""""""""""""""""""""/|
//< |    MultiDomain.intersect    < |
// \|______________________________\|





//|\""""""""""""""""""""""""""""""""|\
//| >    MultiDomain.add methods    | >
//|/________________________________|/
proc MultiDomain.add(D: domain) 
  where D.type==domains.eltType
{
  if D.numIndices>0 {
    if this.length==0 then
      stride = tuplify(D.stride);
    else assert(tuplify(D.stride) == stride,
                "error: Elements of a MultiDomain must have equal stride.");

    subindices = [1..subindices.high+1];
    domains(subindices.high) = D;
  }
}


proc MultiDomain.add(mD: MultiDomain) 
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
    
  
  const prev_high = subindices.high;
  subindices = [1..prev_high + mD.subindices.high];
  domains(prev_high+1..subindices.high) = mD.domains;
}
// /|""""""""""""""""""""""""""""""""/|
//< |    MultiDomain.add methods    < |
// \|________________________________\|









//|\"""""""""""""""""""""""""""""""""""|\
//| >    MultiDomain multiplication    | >
//|/___________________________________|/
//==== range * MultiDomain ====
proc *(R: range(stridable=?s), mD: MultiDomain) 
{
  param stridable = s || mD.stridable;

  var mD_new = new MultiDomain(mD.rank+1, stridable=stridable);
  for D in mD.domains do mD_new.add(R*D);
  return mD_new;
}

//==== domain * MultiDomain ====
proc *(D: domain, mD: MultiDomain)
{
  param stridable = D.stridable || mD.stridable;

  var mD_new = new MultiDomain(D.rank+mD.rank, stridable=stridable);
  for E in mD.domains do
    mD_new.add(D*E);

  return mD_new;
}

//==== MultiDomain * domain ====
proc *(mD: MultiDomain, E: domain)
{
  param stridable = mD.stridable || E.stridable;
  var mD_new = new MultiDomain(mD.rank+E.rank, stridable=stridable);
  for D in mD do mD_new.add(D*E);

  return mD_new;
}

//==== MultiDomain * MultiDomain ====
proc *(mD1: MultiDomain, mD2: MultiDomain)
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
//---------------------------------------------------------------------------
// This performs a "set minus" operation D-E between two domains of equal 
// rank and stride, returning a MultiDomain.  This procedure is the backbone 
// of MultiDomain calculations.
//
// The algorithm is recursive.  If the input domains are of rank 1, the 
// result can be calculated explicitly.  If they are of rank > 1, then
// the result is split into three segments -- low, interior, and high --
// based on the structure of the difference in the leading dimension.
//
// The 'interior' section is determined by the range in the leading dimension
// on which D and E overlap.  It is formed by the tensor product of this
// range with the difference of D and E projected onto all non-leading
// dimensions; this projected difference is computed by a lower-rank domain
// subtraction.
//
// The 'low' and 'high' sections are determined by the portion of the
// leading dimension that is either lower or higher than the 'interior'
// section.  By definition, nothing is removed from D in these areas, and
// they are directly calculated as full (but possibly empty) domains.
//--------------------------------------------------------------------------
proc -(D: domain, E: domain) where D.rank == E.rank
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
  
  
  //==== Check whether D and E are disjoint ====
  //--------------------------------------------------------------------
  // Checking for this right away can provide a significant performance
  // improvement.  Otherwise, disjointness may not be discovered until
  // iterating down to a lower dimension.
  //--------------------------------------------------------------------
  var disjoint = false;
  for i in 1..rank do
    if D.high(i) < E.low(i) || D.low(i) > E.high(i) {
      disjoint = true;
      break;
    }
  
  if disjoint {
    mD = new MultiDomain(D.rank, stridable=stridable);
    mD.add(D);
    return mD;
  }
  else {
    var domain_stack = nontrivialSubtraction(D,E);
    mD.subindices = [1..domain_stack.size];
    for i in 1..domain_stack.size do
      mD.domains(i) = domain_stack.pop();
    return mD;
  }
}


proc nontrivialSubtraction(D: domain, E: domain) where D.rank == E.rank
{
  
  param stridable = D.stridable || E.stridable;
  param rank = D.rank;
  
  var domain_stack = new Stack(domain(rank, stridable=stridable));
  

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

  var interior1_low  = max(D1low,  E1low);
  var interior1_high = min(D1high, E1high);


  //===> Rank 1: Terminal case ===>
  if rank == 1 {

    //==== Add the low section, if it exists ====
    if D1low < interior1_low {
      if stridable then
        domain_stack.push( [D1low .. interior1_low-stride1 by stride1] );
      else
        domain_stack.push( [D1low .. interior1_low-1] );
    }

    //==== Add the high section, if it exists ====
    if D1high > interior1_high {
      if stridable then
        domain_stack.push( [interior1_high+stride1 .. D1high by stride1] );
      else
        domain_stack.push( [interior1_high+1 .. D1high] );
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
        domain_stack.push( (D1low .. interior1_low-stride1 by stride1) * D_projected );
      else
        domain_stack.push( (D1low .. interior1_low-1) * D_projected );
    }

    //==== Compute the interior MultiDomain, and add ====
    var projected_diff: Stack( domain(rank-1,stridable=stridable) ) 
                          = nontrivialSubtraction(D_projected,E_projected);

    var interior1_range: range(stridable=stridable);
    if stridable then
      interior1_range = interior1_low .. interior1_high by stride1;
    else
      interior1_range = interior1_low .. interior1_high;
      

    for i in 1..projected_diff.size do
      domain_stack.push( interior1_range * projected_diff.pop() );


    //==== Add the high section, if it exists ====
    if D1high > interior1_high {
      if stridable then
        domain_stack.push( (interior1_high + stride1 .. D1high by stride1) * D_projected );
      else
        domain_stack.push( (interior1_high+1 .. D1high) * D_projected );
    }
  }
  //<=== Rank > 1 <===

  return domain_stack;

}
// /|''''''''''''''''''''''''''''''''''''''/|
//< |    MultiDomain = domain - domain    < |
// \|......................................\|


//|\'''''''''''''''''''''''''''''''''''''''''''|\
//| >    MultiDomain = MultiDomain - domain    | >
//|/...........................................|/
proc -(mD: MultiDomain, E: domain) 
  where mD.rank==E.rank && mD.stridable==E.dim(1).stridable
{
  
  var domain_stack = new Stack(domain(mD.rank, stridable = mD.stridable));
  var domain_count: int;
  
  for D in mD {
    var difference = D-E;

    for D_diff in difference { 
      domain_stack.push(D_diff);
      domain_count += 1;
    }
    
    delete difference;
  }


  var mD_new = new MultiDomain(mD.rank, mD.stridable);
  mD_new.subindices = [1..domain_count];
  for i in mD_new.subindices do
    mD_new.domains(i) = domain_stack.pop();

  return mD_new;

}
// /|'''''''''''''''''''''''''''''''''''''''''''/|
//< |    MultiDomain = MultiDomain - domain    < |
// \|...........................................\|



//|\''''''''''''''''''''''''''''''''''''''''''''''''|\
//| >    MultiDomain = MultiDomain - MultiDomain    | >
//|/................................................|/
proc -(mD1: MultiDomain, mD2: MultiDomain)
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
proc MultiDomain.subtract(E: domain)
  where E.type == domains.eltType
{
  if domains.numElements>0 then
    assert(tuplify(E.stride) == stride,
	         "error: Elements of a MultiDomain must have equal stride.");

  var difference = this - E;
  this.subindices = difference.subindices;
  this.domains = difference.domains;
  delete difference;
}
// /|"""""""""""""""""""""""""""""/|
//< |    MultiDomain.subtract    < |
// \|_____________________________\|





//|\""""""""""""""""""""""""""|\
//| >    class: MultiArray    | >
//|/__________________________|/

//-----------------------------------------------------
// A collection of arrays, designed to store values on
// all domains in a MultiDomain.  Enforcement of this
// is fairly weak.
//-----------------------------------------------------

class MultiArray {
  
  //|\'''''''''''''''|\
  //| >    fields    | >
  //|/...............|/

  param rank: int;
  param stridable: bool;
  type  eltType;
  
  var subindices:         domain(1);
  var independent_arrays: [subindices] IndependentArray(rank, stridable, eltType);

  // /|'''''''''''''''/|
  //< |    fields    < |
  // \|...............\|
  
  
  proc length return subindices.high;


  
  
  //|\'''''''''''''''''''''''''''''|\
  //| >    special method: this    | >
  //|/.............................|/
  
  proc this (subindex: int) var
  {
    return independent_arrays(subindex).value;
  }
  // /|'''''''''''''''''''''''''''''/|
  //< |    special method: this    < |
  // \|.............................\|
  
  
  
  //|\''''''''''''''''''''''''''''''|\
  //| >    special method: these    | >
  //|/..............................|/
  
  //------------------------------------------------------
  // Iterates over the IndependentArrays that make up the
  // MultiArray.
  //------------------------------------------------------

  iter these () var 
  {
    for independent_array in independent_arrays do 
      yield independent_array.value;
  }
  // /|''''''''''''''''''''''''''''''/|
  //< |    special method: these    < |
  // \|..............................\|



  //|\'''''''''''''''''''''''''|\
  //| >    method: allocate    | >
  //|/.........................|/
  
  //----------------------------------------------------------
  // Allocates storage in the MultiArray corresponding to the
  // domains in a MultiDomain.
  //----------------------------------------------------------

  proc allocate ( mD: MultiDomain(rank,stridable) ) 
  {
    subindices = mD.subindices;
    
    for i in subindices {
      independent_arrays(i) = new IndependentArray(rank, stridable, eltType);
      independent_arrays(i).Domain = mD.domains(i);      
    }
  }
  // /|'''''''''''''''''''''''''/|
  //< |    method: allocate    < |
  // \|.........................\|
 
 
 
  //|\''''''''''''''''''''''|\
  //| >    method: clear    | >
  //|/......................|/
  
  proc clear () 
  {
    for subindex in subindices {
      independent_arrays(subindex).clear();
      delete independent_arrays(subindex);
    }
  }
  // /|''''''''''''''''''''''/|
  //< |    method: clear    < |
  // \|......................\|
  
}
// /|""""""""""""""""""""""""""/|
//< |    class: MultiArray    < |
// \|__________________________\|






// //|\""""""""""""""""""""""""""|\
// //| >    equality overload    | >
// //|/__________________________|/
// proc ==(S1: MultiDomain, S2: MultiDomain) {
//   var S_diff = S1 - S2;
// 
//   if S_diff.subindices.numIndices==0 then
//     return true;
//   else
//     return false;
// 
// }
// // /|""""""""""""""""""""""""""/|
// //< |    equality overload    < |
// // \|__________________________\|


proc main {
  
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
