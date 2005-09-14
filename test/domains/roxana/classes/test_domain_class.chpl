class Storage {
  type element;
}  

class Domain {
  type IndexType;
  
  var arrays: Storage(Array);
  var subdomains: Storage(Domain);
  
  var first, last: IndexType;
  
  function includes(other: Domain);
  
  function index(i : IndexType): IndexType;
  function range(): seq(IndexType);
  
  function allocate();
  function reallocate();
  
  iterator for(): IndexType;
  iterator forall(): IndexType;
}

class ADomain : Domain {
  parameter rank : integer;
  var stride, alignment: integer;
  
  function set_range(r: seq(IndexType));
}

class IDomain : Domain {
  var indices: Storage(IndexType);
  
  function add(i: seq(IndexType));
  function remove(i: seq(IndexType));
  
  function += (i: seq(IndexType));
  function -= (i: seq(IndexType));
  
  function member?(i : seq(IndexType)): boole;
}

class ODomain : IDomain {
  function new(): IndexType;
}
