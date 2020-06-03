
public use BasicDataStructures;



//|\""""""""""""""""""""""""""""|\
//| >    clusterFlags routine   | >
//|/____________________________|/

//-----------------------------------------------------------
// Returns a List of domains that clusters flagged points
// in the boolean array 'flags' into domains with the target 
// efficiency, if possible.  Clustering is performed by the 
// Berger-Rigoutsos algorithm.
//-----------------------------------------------------------

proc clusterFlags (
  flags:             [?full_domain] bool, 
  target_efficiency: real,
  min_width:         full_domain.rank*int )
{
    
  param rank = full_domain.rank;

  //---- Create stack of unprocessed domains ----
  
  var unprocessed_domain_stack = new unmanaged Stack( domain(rank,stridable=true) );
  unprocessed_domain_stack.push(full_domain);
  
  
  //---- List of finished domains ----
  
  var finished_domain_list = new unmanaged List( domain(rank, stridable=true) );
  
  
  while !unprocessed_domain_stack.isEmpty()
  {

    //---- Make the top domain a candidate ----
    
    var D = unprocessed_domain_stack.pop();
    var candidate = new unmanaged CandidateDomain(rank,D,flags(D),min_width);
    
    
    //===> If candidate is inefficient, then split ===>

    if candidate.efficiency() < target_efficiency
    {

      //---- Generate new domains ----
      
      var D1, D2: domain(rank,stridable=true);
      (D1,D2) = candidate.split();

      
      //---- If D2 is empty, split was unsuccessful ----
      
      if D2.size==0 then finished_domain_list.add(candidate.D);


      //---- Otherwise, push new candidates to stack ----
      
      else {
        
        unprocessed_domain_stack.push(D1);
        unprocessed_domain_stack.push(D2);
      }
      
    }
    //<=== If candidate is inefficient, then split <===

    //---- Otherwise, add to domain_set of finished domains ----
    
    else finished_domain_list.add(candidate.D);
    
    
  
    //---- Clean up ----

    delete candidate;
  }
  
  
  
  delete unprocessed_domain_stack;
  
  return finished_domain_list;
}
// /|"""""""""""""""""""""""""""""/|
//< |    clusterFlags routine    < |
// \|_____________________________\|



//|\""""""""""""""""""""""""""""""|\
//| >    CandidateDomain class    | >
//|/______________________________|/

// helper class
class ArrayWrapper
{
  var Domain: domain(1,stridable=true);
  var array: [Domain] int;
}

class CandidateDomain {
  
  param rank:       int;
  const D:          domain(rank,stridable=true);
  const flags:      [D] bool;
  const min_width:  rank*int;
  var   signatures: rank*unmanaged ArrayWrapper;

  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/

  //
  // TODO: At present, the compiler-generated constructor for classes
  // with array fields like this requires the actual that's passed to
  // it to be a default rectangular array.  This is stricter than if
  // the user wrote the equivalent initializer in which the actual to
  // have the same formal type because we treat formal array arguments
  // as being flexible if they don't name an explicit domain map.
  // This causes issues once array operations like slices are not
  // stored in closed form.  See issue #5289 for details.
  //
  // Once this issue is resolved and the compiler is generating
  // similarly generic initializers, the changes that introduced this
  // comment can/should be reverted (the introduction of the
  // CandidateDomain() constructor below).
  //
  proc init(param rank: int,
                       initD,
                       initFlags,
                       initMin_width) {
    this.rank = rank;
    D = initD;
    flags = initFlags;
    min_width = initMin_width;

    proc calculate_signatures(param d) {
      if d == rank-1 then return ( new unmanaged ArrayWrapper( {D.dim(d)} ) ,);
      else                return ( new unmanaged ArrayWrapper( {D.dim(d)} ) ,
                                 ( ...calculate_signatures( d + 1 ) )      );
    }
    signatures = calculate_signatures(0);

    this.complete();
      
    for idx in D 
    {
      if flags(idx) then
        for d in 0..rank-1 do signatures(d).array(idx(d)) += 1;
    }


    //---- Trim ----
    trim();
    
  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|



  //|\'''''''''''''''''''|\
  //| >  deinitializer   | >
  //|/...................|/

  proc deinit ()
  { 
    for i in 0..rank-1 do delete signatures(i);
  }
  // /|'''''''''''''''''''/|
  //< |  deinitializer   < |
  // \|...................\|



  //|\'''''''''''''''''''|\
  //| >    efficiency    | >
  //|/...................|/
  proc efficiency()
  {
    return +reduce(signatures(1).array):real / D.size:real;
  }
  // /|'''''''''''''''''''/|
  //< |    efficiency    < |
  // \|...................\|


}
// /|"""""""""""""""""""""""""""""""/|
//< |    class: CandidateDomain    < |
// \|_______________________________\|




//|\"""""""""""""""""""""""""""""|\
//| >    CandidateDomain.trim    | >
//|/_____________________________|/

proc CandidateDomain.trim()
{    
  //===> Find bounds of trimmed domain ===>
  var trimmed_ranges:      rank*range(stridable=true);
  var trim_low, trim_high: int;
  
  for d in 0..rank-1 {
    
    //---- Low bound ----
    
    for i in D.dim(d) do
      if signatures(d).array(i)>0 {
        trim_low = i;
        break;
      }

      
    //---- High bound ----
    
    for i in D.dim(d) by -1 do
      if signatures(d).array(i)>0 {
        trim_high = i;
        break;
      }
    
    
    //---- Create range ----

    var stride = D.stride(d);
    var R = trim_low .. trim_high by stride;


    //---- Comply with minimum width ----

    if R.size < min_width(d) 
    {
      
      //---- Approximately center the enlarged range ----
      
      var n_overflow_low = (min_width(d) - R.size) / 2;
      var tmp = ((trim_low - n_overflow_low*stride .. by stride) #min_width(d));
      R = tmp.alignHigh();


      //---- Enforce bounds of D ----
      
      if R.low  < D.low(d)  then R += D.low(d) - R.low;
      if R.high > D.high(d) then R -= R.high - D.high(d);
    }
    
    
    //---- Assign range ----
    
    trimmed_ranges(d) = R;
    
  }
  //<=== Find bounds of trimmed domain <===
  

  //---- Resize domain and signatures ----
  
  D = trimmed_ranges;
  for d in 0..rank-1 do signatures(d).Domain = {D.dim(d)};

}
// /|"""""""""""""""""""""""""""""/|
//< |    CandidateDomain.trim    < |
// \|_____________________________\|






//|\""""""""""""""""""""""""""""""|\
//| >    CandidateDomain.split    | >
//|/______________________________|/

proc CandidateDomain.split()
{
  
  var D1, D2: domain(rank, stridable=true);
  
  (D1,D2) = removeHole();
  
  if D2.size==0 then
    (D1,D2) = inflectionCut();
  
  return (D1,D2);

}
// /|""""""""""""""""""""""""""""""/|
//< |    CandidateDomain.split    < |
// \|______________________________\|




//|\"""""""""""""""""""""""""""""""""""|\
//| >    CandidateDomain.removeHole    | >
//|/___________________________________|/

proc CandidateDomain.removeHole()
{

  var D1, D2: domain(rank, stridable=true);
  D1 = D;

  var ranges:      rank*range(stridable=true);
  var hole_low:    int;
  var hole_high:   int;
  var low_active:  bool;
  var high_active: bool;
  var hole:        domain(rank,stridable=true);
  
  var max_hole:    domain(rank, stridable=true);
  var d_cut:       int;
  var stride:      int;

  //===> Locate largest hole ===>

  for d in 0..rank-1 {
    
    //---- ranges = D ----
    for d2 in 0..d-1      do ranges(d2) = D.dim(d2);
    for d2 in d+1..rank-1 do ranges(d2) = D.dim(d2);
  
  
    //---- Allowable hole bounds ----
    //--------------------------------------------------------
    // Hole bounds outside this range would violate min_width
    // after the split.
    //--------------------------------------------------------
    
    stride = D.stride(d);

    var allowable_hole_bounds = D.low(d) + stride*min_width(d) 
                                .. D.high(d) - stride*min_width(d)
                                by stride;

    low_active  = false;
    high_active = false;

    for i in allowable_hole_bounds {
      if !low_active && signatures(d).array(i)==0 {
        hole_low   = i;
        low_active = true;
      }
      else if low_active && signatures(d).array(i)>0 {
        hole_high   = i-stride;
        high_active = true; 
      }
      else if low_active && i==allowable_hole_bounds.high {
        hole_high   = i;
        high_active = true;
      }

      if low_active && high_active {
        ranges(d) = hole_low .. hole_high by stride;
        hole = ranges;
        
        if hole.size > max_hole.size
        {
          max_hole = hole;
          d_cut    = d;
        }

        low_active  = false;
        high_active = false;
      }
    }
  }
  
  //<=== Locate largest hole <===

  
  
  //---- Split by removing largest hole ----
  
  if max_hole.size > 0
  {
    
    stride = D.stride(d_cut);
    
    for d in 0..d_cut-1      do ranges(d) = D.dim(d);
    for d in d_cut+1..rank-1 do ranges(d) = D.dim(d);
    
    ranges(d_cut) = D.low(d_cut) .. max_hole.low(d_cut)-stride by stride;
    D1 = ranges;
    
    ranges(d_cut) = max_hole.high(d_cut)+stride .. D.high(d_cut) by stride;
    D2 = ranges;
    
  }


  return (D1,D2);

}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    CandidateDomain.removeHole    < |
// \|___________________________________\|




//|\""""""""""""""""""""""""""""""""""""""|\
//| >    CandidateDomain.inflectionCut    | >
//|/______________________________________|/

proc CandidateDomain.inflectionCut ()
{

  var ranges: rank*range(stridable=true);

  var D1, D2: domain(rank, stridable=true);
  D1 = D;

  var magnitude: int;
  var diff2_current, diff2_above: int;
  var cut_dimension, cut_magnitude, D1_high: int;


  //===> Locate optimal cut ===>
  
  for d in 0..rank-1 {
    
    //---- Must be at least 4 cells wide for an inflection cut ----
    
    if D.dim(d).size >= 4 {

      ref sig = signatures(d).array;
      var stride = D.stride(d);

      //===> Search for cuts ===>
      
      //---- Allowable D1 high limits ----
      var allowable_D1_highs = D.low(d) + stride*(min_width(d)-1) 
                               .. D.high(d) - stride*min_width(d)
                               by stride;
      
      var i = allowable_D1_highs.low - stride;
      diff2_above = sig(i) - 2*sig(i+stride) + sig(i+2*stride);

      for i in allowable_D1_highs {
        diff2_current = diff2_above;
        diff2_above = sig(i) - 2*sig(i+stride) + sig(i+2*stride);

        if diff2_current*diff2_above <= 0 {
        
          magnitude = abs(diff2_above - diff2_current);
        
          if magnitude > cut_magnitude {
            cut_dimension = d;
            // cut_location = i;
            cut_magnitude = magnitude;
            D1_high = i;
          }
        }

      }
      //<=== Search for cuts <===
    }
  }
  
  //<=== Locate optimal cut <===


  //===> Apply the optimal cut ===>
  
  if cut_magnitude > 0 {

    for d in 0..rank-1 do ranges(d) = D.dim(d);

    var stride = D.stride(cut_dimension);
    ranges(cut_dimension) = D.low(cut_dimension) .. D1_high by stride;
    D1 = ranges;

    ranges(cut_dimension) = D1_high+stride .. D.high(cut_dimension) by stride;
    D2 = ranges;

  }
  
  //<=== Apply the optimal cut <===


  return (D1,D2);

}
// /|""""""""""""""""""""""""""""""""""""""/|
//< |    CandidateDomain.inflectionCut    < |
// \|______________________________________\|



//|\"""""""""""""""""""""""""""|\
//| >    writeFlags routine    | >
//|/___________________________|/

//--------------------------------------------------------
// For testing purposes.  Writes a boolean array of flags
// in a more readable form (0s and 1s).
//--------------------------------------------------------

proc writeFlags( flags: [?D] bool )
{

  var I: [D] int;
  for (i,flag) in zip(I,flags) do
    if flag then i=1;
    
  writeln("On domain ", flags.domain, ":");
  writeln(I);

}
// /|"""""""""""""""""""""""""""/|
//< |    writeFlags routine    < |
// \|___________________________\|




proc main {


 
  //===> Initialize array of flags ===>
  var D = {-19..19 by 2, -19..19 by 2};
  var F: [D] bool = false;
  
  var a1: real = 20.0;
  var b1: real = 7.0;
  var a2: real = 9.0;
  var b2: real = 18.0;
  
  
  for (i,j) in D {
    if (i**2:real/a1**2 + j**2:real/b1**2 < 1.0 || 
       i**2:real/a2**2 + j**2:real/b2**2 < 1.0) &&
       abs(j) > 0 then
      F(i,j) = true;
  }
  //<=== Initialize array of flags <===
  

  
  writeFlags(F);

  writeln("");

  var cluster_blocks = clusterFlags(F, .8, (2,2));

  for block in cluster_blocks {
    writeln("");
    writeFlags(F(block));
  }



  
}
