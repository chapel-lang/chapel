use DomainSet_def;
use ArraySet_def;
use EnhancedArithmetic;
use Stack_def;

param dimension: int = 2;
const dimensions = [1..dimension];


//|\"""""""""""""""""""""""""""""""|\
//| >    FlaggedDomainSet class    | >
//|/_______________________________|/
class FlaggedDomainSet {
  
  var full_domain: domain(dimension, stridable=true);
  var flags:       [full_domain] bool;
  const min_width: dimension*int;
  
  var domain_set = new DomainSet(dimension,stridable=true);

  //|\'''''''''''''''''''''|\
  //| >    constructors    | >
  //|/.....................|/
  //==== Provide only the flags ====
  def FlaggedDomainSet(flags: [] bool)
    where flags.domain.rank==dimension && flags.domain.stridable==true
  {
    this.full_domain = flags.domain;
    this.flags = flags;
    for d in dimensions do min_width(d) = 2;
  }

  //==== Provide flags and min_width ====
  def FlaggedDomainSet(flags: [] bool, min_width: dimension*int)
    where flags.domain.rank==dimension && flags.domain.stridable==true
  {
    this.full_domain = flags.domain;
    this.flags = flags;
    this.min_width = min_width;
  }
  
  //==== Provide only full_domain ====
  def FlaggedDomainSet(full_domain: domain(dimension,stridable=true))
  {
    this.full_domain = full_domain;
    for d in dimensions do min_width(d) = 2;
  }

  //==== Provide full_domain and min_width ====
  def FlaggedDomainSet(full_domain: domain(dimension,stridable=true), min_width: dimension*int)
    where flags.domain.rank==dimension && flags.domain.stridable==true
  {
    this.full_domain = full_domain;
    this.min_width = min_width;
  }
  // /|'''''''''''''''''''''/|
  //< |    constructors    < |
  // \|.....................\|


  def writeThis(w: Writer) {
    for D in domain_set {
      writeFlags(D);
      writeln("");
    }
  }
    
    
  def writeFlags(D: domain(dimension,stridable=true)) 
  {
    var I: [D] int;
    for idx in D do
      if flags(idx) then I(idx)=1;
      
    writeln("On domain ", D, ":");
    writeln(I);
  }
  

}
// /|"""""""""""""""""""""""""""""""/|
//< |    FlaggedDomainSet class    < |
// \|_______________________________\|



//|\"""""""""""""""""""""""""""""""""""|\
//| >    FlaggedDomainSet.partition    | >
//|/___________________________________|/
def FlaggedDomainSet.partition(target_efficiency: real)
{

  //==== Create stack of unprocessed domains ====
  var unprocessed_domain_stack = new Stack(domain(dimension,stridable=true));
  unprocessed_domain_stack.push(flags.domain);
  
  
  while unprocessed_domain_stack.isEmpty()==false {
    //==== Pop top domain ====
    var D = unprocessed_domain_stack.pop();
    var candidate = new CandidateDomain(D,flags,min_width);
    
    //===> If candidate is inefficient, then split ===>
    if candidate.efficiency() < target_efficiency {
      //==== Generate new domains ====
      var D1, D2: domain(dimension,stridable=true);
      (D1,D2) = candidate.split();
      
      //==== If D2 is empty, split was unsuccessful ====
      if D2.numIndices==0 {
        writeln("Warning: FlaggedDomain.partition\n" +
                "  Could not meet target efficiency on domain ", candidate.D, ".");
        domain_set.add(candidate.D);
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
      domain_set.add(candidate.D);
    
    //==== Clean up ====
    candidate.clear();
    delete candidate;
  }
  
}
// /|"""""""""""""""""""""""""""""""""""/|
//< |    FlaggedDomainSet.partition    < |
// \|___________________________________\|



//|\""""""""""""""""""""""""""""""|\
//| >    CandidateDomain class    | >
//|/______________________________|/
class CandidateDomain {
  var D: domain(dimension,stridable=true);
  var min_width: dimension*int;
  var signatures: dimension*IndependentArray(1,true,int);
  
  //|\''''''''''''''''''''|\
  //| >    constructor    | >
  //|/....................|/
  def CandidateDomain(
    D:         domain(dimension, stridable=true),
    flags:     [] bool,
    min_width: dimension*int)
  {
    this.D         = D;
    this.min_width = min_width;

    //===> Calculate signatures ===>
    for d in dimensions do
      signatures(d) = new IndependentArray(1,true,int,[D.dim(d)]);
      
    for idx in D {
      if flags(idx) then
        for d in dimensions do
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
  def efficiency()
  {
    return +reduce(signatures(1).value):real / D.numIndices:real;
  }
  // /|'''''''''''''''''''/|
  //< |    efficiency    < |
  // \|...................\|

  
  //|\''''''''''''''|\
  //| >    clear    | >
  //|/..............|/
  def clear() {
    for d in dimensions do
      delete signatures(d);
  }
  // /|''''''''''''''/|
  //< |    clear    < |
  // \|..............\|
}




//|\"""""""""""""""""""""""""""""|\
//| >    CandidateDomain.trim    | >
//|/_____________________________|/
def CandidateDomain.trim()
{    
  //===> Find bounds of trimmed domain ===>
  var trimmed_ranges:      dimension*range(stridable=true);
  var trim_low, trim_high: int;
  
  for d in dimensions {
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
  for d in dimensions do
    signatures(d).dom = [D.dim(d)];
}
// /|"""""""""""""""""""""""""""""/|
//< |    CandidateDomain.trim    < |
// \|_____________________________\|






//|\""""""""""""""""""""""""""""""|\
//| >    CandidateDomain.split    | >
//|/______________________________|/
def CandidateDomain.split()
{
  
  var D1, D2: domain(dimension, stridable=true);
  
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
def CandidateDomain.removeHole()
{

  var D1, D2: domain(dimension, stridable=true);
  D1 = D;

  var ranges: dimension*range(stridable=true);
  var hole_low, hole_high: int;
  var low_active, high_active: bool;
  var hole, max_hole: domain(dimension, stridable=true);


  //===> Create stack of holes ===>
  for d in dimensions {
    //==== ranges = D ====
    for d2 in dimensions do ranges(d2) = D.dim(d2);
  
    low_active  = false;
    high_active = false;
  
    //==== Allowable hole bounds ====
    //--------------------------------------------------------
    // Hole bounds outside this range would violate min_width
    // after the split.
    //--------------------------------------------------------
    var stride = D.stride(d);
    var allowable_hole_bounds = D.low(d) + stride*min_width(d) 
                                .. D.high(d) - stride*min_width(d)
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
    var Dset = D - max_hole;
    assert(Dset.indices.numIndices == 2);
    D1 = Dset.domains(1);
    D2 = Dset.domains(2);
    delete Dset;
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
def CandidateDomain.inflectionCut()
{

  var ranges: dimension*range(stridable=true);

  var D1, D2: domain(dimension, stridable=true);
  D1 = D;

  var magnitude: int;
  var diff2_current, diff2_above: int;
  var cut_dimension, cut_magnitude, D1_high: int;


  //===> Generate stack of possible cuts ===>
  for d in dimensions {
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
    for d in dimensions do ranges(d) = D.dim(d);

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











def main {


 
  var D = [-19..19 by 2, -19..19 by 2];
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
  
  

  var flagged_domain_set = new FlaggedDomainSet(F);
  flagged_domain_set.writeFlags(D);

  writeln("");
  flagged_domain_set.partition(.8);
  write(flagged_domain_set);



  
}