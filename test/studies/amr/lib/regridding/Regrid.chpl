use ArraySet_def;



//|\""""""""""""""""""""""""""""|\
//| >    calculateSignatures    | >
//|/____________________________|/
def calculateSignatures(F: [] int)
{
  const D = F.domain;
  param rank = D.rank;
  param stridable = D.stridable;
  
  var signatures: rank*IndependentArray(1,stridable,int);

  var D_ranges, ranges: rank*range(stridable=stridable);
  for i in 1..rank do D_ranges(i) = D.dim(i);

  var slice_domain: D.type;
  
  for i in 1..rank do {
    signatures(i) = new IndependentArray(1,stridable,int);
    signatures(i).dom = [D.dim(i)];
    ranges = D_ranges;
    
    for j in D.dim(i) {
      ranges(i) = j..j;
      slice_domain = ranges;
      signatures(i).value(j) = +reduce(F(slice_domain));
    }
    
  }
  
  return signatures;
  
}
// /|""""""""""""""""""""""""""""/|
//< |    calculateSignatures    < |
// \|____________________________\|



def main {
 
  var D = [1..10,1..10];
  var F: [D] int;

  F([2..5,3..8]) = 1;
  F([4..9,1..5]) = 1;

  var signatures =  calculateSignatures(F);

  for i in D.dim(1) {
    writeln(format("%2i",signatures(1).value(i)), "   ", F(i..i,D.dim(2)));
  }

  writeln("");
  writeln("     ", signatures(2).value);

}