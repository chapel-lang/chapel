/* 
use this file to query for the subdomain the current locale owns.
Ex) const Space = {1..10};
   const SpaceMap = Space dmapped Block(Space);
   var A : [SpaceMap] int;
   
   for L in Locales do 
     on L do writeln(A.localDomain());
--------------------------------------------------
if there are 4 locales, the output will be:

  {1..5, 1..5}
  {1..5, 6..10}
  {6..10, 1..5}
  {6..10, 6..10}
--------------------------------------------------

Note that these features are a work in progress, and are subject 
to change.
*/ 

// currently supported distributions
use StencilDist, CyclicDist, BlockCycDist, ReplicatedDist;

 
// TODO: Replicated doesn't use  targetLocDom. Do we need to have each 
// distribution define its own proc?
proc _array.locGridDom() {
  return _value.dom.dist.targetLocDom;
}

proc _array.locGrid() {
  return _value.dom.dist.targetLocales;
}

// wrapper to hide _value from user
proc _array.locIsSingle() param {
  return _value.locIsSingle();
}

// TODO: add dsi prefix
// tell the user what to expect
proc BlockArr.locIsSingle() param return true;

proc CyclicArr.locIsSingle() param return true;

proc ReplicatedArr.locIsSingle() param  return true;

proc BlockCyclicArr.locIsSingle() param return false;

proc DefaultRectangularArr.locIsSingle() param return true;

// wrapper to hide _value from the user
proc _array.localDomain() {
  if !_value.locIsSingle() then
    compilerError("Array's local domain is not a single domain");
  return _value.localDomain();
}

proc DefaultRectangularArr.localDomain() {
  return dom.ranges;
}

proc DefaultSparseArr.localDomain() {
  return dom.nnzDom;
}

// cover's Opaque too
proc DefaultAssociativeArr.localDomain() {
  return dom.tableDom;
}

// Block and Cyclic are easiest, simply return the single domain
proc BlockArr.localDomain() {
  return myLocArr.locDom.myBlock;
}

proc CyclicArr.localDomain() {
  return myLocArr.locDom.myBlock;
}

// Replicated ony uses one domain, but there are local copies we should return
proc ReplicatedArr.localDomain() {
  return localArrs[here.id].myDom.domLocalRep;
}

iter _array.allLocalDomains() {
  if _value.locIsSingle() then 
    yield _value.localDomain();
  else 
    for d in _value.allLocalDomains() do yield d;
}

// essentially enumerateBlocks()
// basically add blocksize to the start indices
iter BlockCyclicArr.allLocalDomains() {
  for i in myLocArr.indexDom.myStarts {
    var temp : rank*range;
    for param j in 1..rank {
      var lo: idxType;
      if rank == 1 then lo = i;
      else lo = i(j);
      temp(j) = lo .. min(lo + myLocArr.indexDom.globDom.dist.blocksize(j)-1, myLocArr.indexDom.globDom.whole.dim(j).high);
    }
    const r : domain(rank, idxType, stridable) = temp;
    yield r; // yield a domain so the user can use procs like expand/exterior/etc.
  }
}

// if the user incorrectly calls this, they still get something
proc BlockCyclicArr.localDomain() {
  return allLocalDomains();
}
