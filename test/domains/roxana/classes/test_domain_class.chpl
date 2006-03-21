class Storage {
  type element;
}  

class Domain {
  type IndexType;
  
  var arrays: Storage(Array);
  var subdomains: Storage(Domain);
  
  var first, last: IndexType;
  
  fun includes(other: Domain);
  
  fun index(i : IndexType): IndexType;
  fun range(): seq(IndexType);
  
  fun allocate();
  fun reallocate();
  
  iterator for(): IndexType;
  iterator forall(): IndexType;
}

class ADomain : Domain {
  parameter rank : int;
  var stride, alignment: int;
  
  fun set_range(r: seq(IndexType));
}

class IDomain : Domain {
  var indices: Storage(IndexType);
  
  fun add(i: seq(IndexType));
  fun remove(i: seq(IndexType));
  
  fun += (i: seq(IndexType));
  fun -= (i: seq(IndexType));
  
  fun member?(i : seq(IndexType)): boole;
}

class ODomain : IDomain {
  fun new(): IndexType;
}
