use DomainSet_def;

class IndependentArray {
  param rank: int;
  param stridable: bool;
  type eltType;
  
  var dom:   domain(rank=rank, stridable=stridable);
  var value: [dom] eltType;
}



class ArraySet {
  
  param rank: int;
  param stridable: bool;
  type  eltType;
  
  var indices: domain(1) = [1..0];
  var arrays: [indices] IndependentArray(rank, stridable, eltType);
  
  def allocate(d_set: DomainSet(rank,stridable)) {
    indices = d_set.indices;
    for i in indices {
      arrays(i) = new IndependentArray(rank, stridable, eltType);
      arrays(i).dom = d_set.domains(i);      
    }
  }
  
  def these() {
    for array in arrays do yield array;
  }
}



def main {
  
  var full_D = [1..10, 1..10];
  var d_set = full_D - [3..5, 4..8];
  
  var array_set: ArraySet(d_set.rank,d_set.stridable, int);
  array_set = new ArraySet(d_set.rank, d_set.stridable, int);
  array_set.allocate(d_set);

  
  for array in array_set {
    writeln(array.dom);
    writeln(array.value);
  }
}