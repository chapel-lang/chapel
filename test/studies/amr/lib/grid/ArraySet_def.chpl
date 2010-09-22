use DomainSet_def;

class IsolatedArray {
  param rank: int;
  param stridable: bool;
  type eltType;
  
  var dom:   domain(rank=rank, stridable=stridable);
  var value: [dom] eltType;
}



class ArraySet {
  
  const domain_set;
  type eltType;
  
  var arrays: [domain_set.indices] IsolatedArray(domain_set.rank, domain_set.stridable, eltType);
  
  def initialize() {
    for i in domain_set.indices {
      arrays(i) = new IsolatedArray(domain_set.rank, domain_set.stridable, eltType);
      arrays(i).dom = domain_set.domains(i);      
    }
  }
  
  def these() {
    for array in arrays do yield array;
  }
}



def main {
  
  var full_D = [1..10, 1..10];
  var d_set = full_D - [3..5, 4..8];
  
  var array_set = new ArraySet(d_set, int);
  for array in array_set {
    writeln(array.dom);
    writeln(array.value);
  }
}