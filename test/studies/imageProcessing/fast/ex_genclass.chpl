
/*****
      ex_genclass.chpl -
      An example of a generic class implementing a chunked array, which
      over-allocates an array to amortize the cost of re-sizing when adding
      new elements.  See text for details.

      c 2015-2018 Primordial Machine Vision Systems
*****/

class chunkarray {
  type eltType;                         /* generic array */
  param INIT_ALLOC                      /* initial allocation size */
    = 2;
  param GROW_ALLOC                      /* chunk/allocation size */
    = 2;
  var Lalloc : domain(rank=1)           /* internal allocation range */
    = 1..INIT_ALLOC;
  var Ldata : domain(rank=1)            /* range for data (starts empty) */
    = 1..0;
  var data : [Lalloc] eltType;          /* the allocation */
  var lock : sync int                   /* synchronization lock in append */
    = 1;

  /***
      init:  Default initializer.
  ***/
  proc init(type elemType) {
    eltType = elemType;
  }

  /***
      this:  Retrieve the array.
  ***/
  proc this() {
    return data[Ldata];
  }

  /***
      this:  Retrieve an element at an index.
      args:  ind - position in array to retrieve
      returns:  value stored at ind
  ***/
  proc this(ind : int) : eltType {
    return data(ind);
  }

  /***
      these:  Iterator across the data that's been stored.
  ***/
  iter these() {
    for d in data[Ldata] do
      yield d;
  }

  /***
      append:  Add an element to the end of the array.
      args:    val - element to add
      modifies:  data, Lalloc, Ldata
  ***/
  proc append(const in val : eltType) {
    lock.readFE();

    /* Slice the expansion to trim negative indices. */
    if (Ldata.high == Lalloc.high) {
      Lalloc = Lalloc.expand(GROW_ALLOC)[1..];
      /* For demo to show we're re-allocating. */
      writeln("grew array to ", Lalloc);
    }
    Ldata = Ldata.expand(1)[1..];
    data[Ldata.high] = val;

    lock.writeEF(1);
  }

  /***
      size:  Return the actual number of elements stored in the array.
  ***/
  proc size() {
    return Ldata.high;
  }
}


/* Simple record that we'll use as the element type. */
record corner {
  var xc, yc : int;                     /* coords of corner center */
  var len : int;                        /* length of run of differing pixels */
}


var corners = new unmanaged chunkarray(corner);
var details : corner;

writeln("\nfilling array, storing first element");
details.xc = 100;   details.yc = 50;   details.len = 10;
corners.append(details);
writeln(" storing second element");
details.xc = 150;   details.yc = 75;   details.len = 13;
corners.append(details);
writeln(" storing third element");
details.xc = 125;   details.yc = 100;  details.len = 12;
corners.append(details);

writeln("\nusing array");
writeln(" corners data in ", corners().domain, "   size ", corners.size());
writeln(" corner #2 ", corners(2));
writeln(" dump array:");
for c in corners do writeln("  corner at ", c.xc, ", ", c.yc, "   len ", c.len);

delete corners;
