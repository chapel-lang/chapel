
use MultiDomain_def;



//|\""""""""""""""""""""""""""""""|\
//| >    partitionFlags routine   | >
//|/______________________________|/
//---------------------------------------------------------
// Returns a MultiDomain that partitions the boolean array
// 'flags' with the target efficiency, if possible.
//---------------------------------------------------------
proc partitionFlags(
  flags:             [?full_domain] bool, 
  target_efficiency: real,
  min_width:         full_domain.rank*int)
{
  param rank = full_domain.rank;

  //==== Create stack of unprocessed domains ====
  var unprocessed_domain_stack = new Stack(domain(rank,stridable=true));
  unprocessed_domain_stack.push(full_domain);
  
  //==== MultiDomain for finished domains ====
  var finished_mDomain = new MultiDomain(rank, stridable=true);
  
  
  while unprocessed_domain_stack.isEmpty()==false {
    //==== Pop top domain ====
    var D = unprocessed_domain_stack.pop();
    var candidate = new CandidateDomain(rank,D,flags(D),min_width);
    
    //===> If candidate is inefficient, then split ===>
    if candidate.efficiency() < target_efficiency {
      //==== Generate new domains ====
      var D1, D2: domain(rank,stridable=true);
      (D1,D2) = candidate.split();
      
      //==== If D2 is empty, split was unsuccessful ====
      if D2.numIndices==0 {
        // writeln("Warning: FlaggedDomain.partition\n" +
        //         "  Could not meet target efficiency on domain ", candidate.D, ".");
        finished_mDomain.add(candidate.D);
      }
      //==== Otherwise, push new candidates to stack ====
      else {
        unprocessed_domain_stack.push(D1);
        unprocessed_domain_stack.push(D2);
      }
    }
    //<=== If candidate is inefficient, then split <===
    //==== Otherwise, add to domain_set of finished domains ====
    else
      finished_mDomain.add(candidate.D);
    
    //==== Clean up ====
    candidate.clear();
    delete candidate;
  }
  
  return finished_mDomain;
}
// /|"""""""""""""""""""""""""""""""/|
//< |    partitionFlags routine    < |
// \|_______________________________\|



//|\""""""""""""""""""""""""""""""|\
//| >    CandidateDomain class    | >
//|/______________________________|/
class CandidateDomain {
  param rank:       int;
  const D:          domain(rank,stridable=true);
  const flags:      [D] bool;
  const min_width:  rank*int;
  var   signatures: rank*IndependentArray(1,true,int);
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  //------------------------------------------------------------------
  // Currently forced to use initialize rather than a constructor, as
  // this class is generic.
  //------------------------------------------------------------------
  proc initialize() {
    //===> Calculate signatures ===>
    for d in 1..rank do
      signatures(d) = new IndependentArray(1,true,int,[D.dim(d)]);
      
    for idx in D {
      if flags(idx) then
        for d in 1..rank do
          signatures(d).value(idx(d)) += 1;
    }
    //<=== Calculate signatures <===

    //==== Trim ====
    trim();
  }
  // /|''''''''''''''''''''/|
  //< |    constructor    < |
  // \|....................\|


  //|\'''''''''''''''''''|\
  //| >    efficiency    | >
  //|/...................|/
  proc efficiency()
  {
    return +reduce(signatures(1).value):real / D.numIndices:real;
  }
  // /|'''''''''''''''''''/|
  //< |    efficiency    < |
  // \|...................\|

  
  //|\''''''''''''''|\
  //| >    clear    | >
  //|/..............|/
  proc clear() { 
    for i in 1..rank do delete signatures(i);
  }
  // /|''''''''''''''/|
  //< |    clear    < |
  // \|..............\|
}




//|\"""""""""""""""""""""""""""""|\
//| >    CandidateDomain.trim    | >
//|/_____________________________|/

proc CandidateDomain.trim()
{    
  //===> Find bounds of trimmed domain ===>
  var trimmed_ranges:      rank*range(stridable=true);
  var trim_low, trim_high: int;
  
  for d in 1..rank {
    //==== Low bound ====
    for i in D.dim(d) do
      if signatures(d).value(i)>0 {
        trim_low = i;
        break;
      }
      
    //==== High bound ====
    for i in D.dim(d) by -1 do
      if signatures(d).value(i)>0 {
        trim_high = i;
        break;
      }
    
    //==== Create range ====
    var stride = D.stride(d);
    var R = trim_low .. trim_high by stride;

    //==== Comply with minimum width ====
    if R.length < min_width(d) {
      //==== Approximately center the enlarged range ====
      var n_overflow_low = (min_width(d) - R.length) / 2;
      R = (trim_low - n_overflow_low*stride .. by stride) #min_width(d);

      //==== Enforce low bound of D ====
      if R.low < D.low(d) then
        R += D.low(d) - R.low;

      //==== Enforce high bound of D ====
      if R.high > D.high(d) then 
        R -= R.high - D.high(d);
    }
    
    //==== Assign range ====
    trimmed_ranges(d) = R;
    
  }
  //<=== Find bounds of trimmed domain <===
  

  //==== Resize domain ====
  D = trimmed_ranges;
  
  //==== Resize signatures ====
  for d in 1..rank do
    signatures(d).Domain = [D.dim(d)];
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
  
  if D2.numIndices==0 then
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


  //===> Create stack of holes ===>
  for d in 1..rank {
    //==== ranges = D ====
    for d2 in 1..rank do ranges(d2) = D.dim(d2);
  
    low_active  = false;
    high_active = false;
  
    //==== Allowable hole bounds ====
    //--------------------------------------------------------
    // Hole bounds outside this range would violate min_width
    // after the split.
    //--------------------------------------------------------
    var stride = D.stride(d);
    var allowable_hole_bounds = D.low(d) + stride*min_width(d) 
                                .. D.alignedHigh(d) - stride*min_width(d)
                                by stride;
  
    for i in allowable_hole_bounds {
      if !low_active && signatures(d).value(i)==0 {
        hole_low   = i;
        low_active = true;
      }
      else if low_active && signatures(d).value(i)>0 {
        hole_high   = i-2;
        high_active = true; 
      }
      else if low_active && i==allowable_hole_bounds.high {
        hole_high   = i;
        high_active = true;
      }

      if low_active && high_active {
        ranges(d) = hole_low .. hole_high by 2;
        hole = ranges;
        if hole.numIndices > max_hole.numIndices then
          max_hole = hole;

        low_active  = false;
        high_active = false;
      }
    }
  }
  //<=== Create stack of holes <===

  
  if max_hole.numIndices > 0 {
    var mD = D - max_hole; // Returns MultiDomain
    assert(mD.domains.numElements == 2);
    D1 = mD.domains(1);
    D2 = mD.domains(2);
    delete mD;
  }
  //<=== Split by removing largest hole <===

  return (D1,D2);

}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    CandidateDomain.removeHole    < |
// \|___________________________________\|




//|\""""""""""""""""""""""""""""""""""""""|\
//| >    CandidateDomain.inflectionCut    | >
//|/______________________________________|/

proc CandidateDomain.inflectionCut()
{

  var ranges: rank*range(stridable=true);

  var D1, D2: domain(rank, stridable=true);
  D1 = D;

  var magnitude: int;
  var diff2_current, diff2_above: int;
  var cut_dimension, cut_magnitude, D1_high: int;


  //===> Generate stack of possible cuts ===>
  for d in 1..rank {
    //==== Must be at least 4 cells wide for an inflection cut ====
    if D.dim(d).length >= 4 {

      var sig => signatures(d).value;  // Does this work?
      var stride = D.stride(d);

      //===> Search for cuts ===>
      //==== Allowable D1 high limits ====
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
  //<=== Generate stack of possible cuts <===


  //===> Apply the optimal cut ===>
  if cut_magnitude > 0 {

    //===> Apply cut ===>
    for d in 1..rank do ranges(d) = D.dim(d);

    var stride = D.stride(cut_dimension);
    ranges(cut_dimension) = D.low(cut_dimension) .. D1_high by stride;
    D1 = ranges;

    ranges(cut_dimension) = D1_high+stride .. D.high(cut_dimension) by stride;
    D2 = ranges;
    //<=== Apply cut <===
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
proc writeFlags(flags: [?D] bool) 
{
  var I: [D] int;
  for (i,flag) in (I,flags) do
    if flag then i=1;
    
  writeln("On domain ", flags.domain, ":");
  writeln(I);
}
// /|"""""""""""""""""""""""""""/|
//< |    writeFlags routine    < |
// \|___________________________\|




// proc main {
// 
// 
//  
//   //===> Initialize array of flags ===>
//   var D = [-19..19 by 2, -19..19 by 2];
//   var F: [D] bool = false;
//   
//   var a1: real = 20.0;
//   var b1: real = 7.0;
//   var a2: real = 9.0;
//   var b2: real = 18.0;
//   
//   
//   for (i,j) in D {
//     if (i**2:real/a1**2 + j**2:real/b1**2 < 1.0 || 
//        i**2:real/a2**2 + j**2:real/b2**2 < 1.0) &&
//        abs(j) > 0 then
//       F(i,j) = true;
//   }
//   //<=== Initialize array of flags <===
//   
// 
//   
//   writeFlags(F);
// 
//   writeln("");
// 
//   var partitioned_blocks = partitionFlags(F, .8, (2,2));
// 
//   for block in partitioned_blocks {
//     writeln("");
//     writeFlags(F(block));
//   }
// 
// 
// 
//   
// }