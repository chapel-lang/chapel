use DomainSet_def;




//|\"""""""""""""""""""""""""""|\
//| >    calculateSignature    | >
//|/___________________________|/
def calculateSignature(F: [] int, sig_dimension: int)
{
  const D = F.domain;
  param rank = D.rank;
  param stridable = D.stridable;
  
  var signature: [D.dim(sig_dimension)] int;

  var ranges: rank*range(stridable=stridable);
  for i in 1..rank do ranges(i) = D.dim(i);

  var orthogonal_plane: D.type;
  

  for i in D.dim(sig_dimension) {
    ranges(sig_dimension) = i..i;
    orthogonal_plane = ranges;
    signature(i) = +reduce(F(orthogonal_plane));
  }
    
  return signature;
  
}
// /|"""""""""""""""""""""""""""/|
//< |    calculateSignature    < |
// \|___________________________\|






//|\"""""""""""""""""""""|\
//| >    zeroSegments    | >
//|/_____________________|/
def zeroSegments(signature: [] int)
  where signature.domain.rank == 1
{
  param stridable = signature.domain.stridable;
  const D = signature.domain;
  const stride = D.stride;
  
  var zero_segments = new DomainSet(1,stridable);
  
  var low_bound_active  = false;
  var high_bound_active = false;
  var low_bound, high_bound: int;
  
  for i in signature.domain {
    //==== Set the low bound ====
    if !low_bound_active && signature(i) == 0 {
      low_bound = i;
      low_bound_active = true;
    }
  
    //==== Set the high bound ====
    if low_bound_active && !high_bound_active {
      if i == signature.domain.high {
        high_bound = i;
        high_bound_active = true;
      }
      if signature(i) != 0 {
        high_bound = i-stride;
        high_bound_active = true;
      } 
    }
  
    //==== Add to the list of zeros ====
    if low_bound_active && high_bound_active {
      assert(low_bound <= high_bound);
      if stridable then
        zero_segments.add( [low_bound..high_bound by stride]);
      else
        zero_segments.add( [low_bound..high_bound]);
      low_bound_active  = false;
      high_bound_active = false;
    }
    
  }
 
  return zero_segments;
  
}
// /|"""""""""""""""""""""/|
//< |    zeroSegments    < |
// \|_____________________\|





//|\""""""""""""""|\
//| >    split    | >
//|/______________|/
def split(F: [] int) where F.domain.rank > 1
{
  const D = F.domain;
  param rank = D.rank;
  param stridable = D.stridable;

  var nz_domains = new DomainSet(rank,stridable);
  nz_domains.add(D);

  var D_ranges: rank*range(stridable=stridable);
  for i in 1..rank do D_ranges(i) = D.dim(i);

  var ranges: rank*range(stridable=stridable);

  for i in 1..rank {
    var signature = calculateSignature(F,i);
    var zero_segments = zeroSegments(signature);

    ranges = D_ranges;
    for segment in zero_segments {
      var segment_range = segment.dim(1);
      ranges(i) = segment_range;

      var zero_domain: D.type = ranges;
      nz_domains -= zero_domain;
    }
  }

  return nz_domains;

}
// /|""""""""""""""/|
//< |    split    < |
// \|______________\|



def main {
  
  var D = [1..10,1..10];
  var F: [D] int;

  F([2..5,3..8]) = 1;
  F([7..9,1..5]) = 1;

  var nz_domains = split(F);

  var finished = false;

  while !finished {
    var nz_domains_new = new DomainSet(nz_domains.rank,nz_domains.stridable);

    for nz_domain in nz_domains do
      nz_domains_new.add( split(F(nz_domain)) );

    if nz_domains == nz_domains_new then
      finished = true;
    else {
      nz_domains <=> nz_domains_new;
      delete nz_domains_new;
    }
  }

  writeln(F);
  writeln("");;
  writeln(nz_domains);

  

/*   for i in D.dim(1) { */
/*     writeln(format("%2i",signature1(i)), "   ", F(i..i,D.dim(2))); */
/*   } */

/*   writeln(""); */
/*   writeln("     ", signature2); */

}
