use BlockDist, Barriers;

config const n = 10;

config const epsilon = 0.01;

config const printLocaleData = false,
             printArrays = false;

//
// global domains -- describing whole problem; I'm using these to boostrap
// things like the computation of how to arrange the locales, who owns
// what, etc.  In a truly SPMD-style computation, you'd compute all that
// from nothing based on your locale ID and numLocales.  Since the Block
// distribution already encodes all this stuff, I took the shortcut of
// creating a block-distributed domain and then querying all of this
// information out of its guts.  A little lame, but you can only write
// such logic so many times in your life before it gets really old.
//
const LocDom = {1..n  , 1..n  },
         Dom = LocDom dmapped Block(LocDom),
      BigDom = {0..n+1, 0..n+1} dmapped Block(LocDom);

//
// query out the domain and array of the locales we're targeting
//
const LocaleGrid = Dom.targetLocales(),
      LocaleGridDom = LocaleGrid.domain;

if printLocaleData {
  writeln("Our locale grid is as follows:\n", LocaleGrid, "\n");


  //
  // print out our locale's ID and virtual coordinates
  //
  for (lr,lc) in LocaleGridDom {
    on LocaleGrid[lr,lc] {
      writeln("Hello from locale #", here.id, " at ", (lr,lc));
    }
  }

  //
  // query the sub-block of the whole problem space that each locale owns
  //
  // TODO: See the TODO above about querying the targetLocales() from
  // Dom.  By that same argument, we could use localSubdomain() here.
  //
  for (lr,lc) in LocaleGridDom {
    on LocaleGrid[lr,lc] {
      writeln("locale #", here.id, " owns ", Dom.localSubdomain());
    }
  }
}


//
// synchronization variables to coordinate between all of our locales
// once we switch into "fragmented" mode
//
var takeTurns$: sync int = 0;
var delta$: sync real = 0;

//
// a little helper class to store a domain/array pair
// Is there some way to get rid of this?  Once we push back into the domain
// map framework, it will automatically go away
//
class DomArr {
  var Dom: domain(2);
  var Arr: [Dom] real;
}

//
// an array of everyone's chunks of the global problem space that they own
// in a sense it's the distributed A array in a local view
//
var LocalDomArrs: [LocaleGridDom] unmanaged DomArr?;

var numIters: atomic int;


var b = new Barrier(LocaleGridDom.size);

coforall (lr,lc) in LocaleGridDom {
  on LocaleGrid[lr,lc] {
    //
    // What I own; and extended to include overlap with neighbors ("fluff")
    //
    // TODO: See the TODO above about localSubDomain()
    //
    const MyLocDom = Dom.localSubdomain();
    const WithFluff = MyLocDom.expand(1);

    //
    // Store my domain, with fluff, into the global directory so that
    // other locales can refer to it.
    //
    var MyDomArr = new unmanaged DomArr(Dom=WithFluff);
    LocalDomArrs[lr,lc] = MyDomArr;

    //
    // for convenience, let's alias A to save typing
    // we'll declare B to be private/local to this scope because
    // nobody else needs to refer to it.
    //
    ref A = MyDomArr.Arr;
    var B: [WithFluff] real;

    //
    // create a 3x3 array of the domains describing the fluff overlap
    // regions with my neighbors in the i,j direction.  (where 0,0
    // describes what I own myself)
    //
    const PanelDom = {-1..1, -1..1};
    var Panels: [PanelDom] domain(2);

    //
    // use 'exterior' to do the hard work of defining these 3x3 domains
    // for us
    //
    for ij in PanelDom {
      Panels[ij] = MyLocDom.exterior(ij);
    }

    if printLocaleData {
      //
      // Debug print what each locale owns
      //
      while (takeTurns$.readXX() != here.id) { }
      writeln("\nlocale #", here.id, " panels:");
      writeln(Panels);
      takeTurns$.writeXF((here.id + 1)%numLocales);
    }


    /*
        A[  n/4+1,   n/4+1] =  1.0;
        A[3*n/4+1, 3*n/4+1] =  1.0;
        A[  n/4+1, 3*n/4+1] = -1.0;
        A[3*n/4+1,   n/4+1] = -1.0;
    */

    //
    // define four nonzero coordinates
    //
    const p1 = (  n/4+1,   n/4+1);
    const p2 = (3*n/4+1, 3*n/4+1);
    const p3 = (  n/4+1, 3*n/4+1);
    const p4 = (3*n/4+1,   n/4+1);

    //
    // see if we own one or more of the points; if so, initialize A
    // at that point
    //
    if WithFluff.contains(p1) then
      A[p1] =  1.0;

    if WithFluff.contains(p2) then
      A[p2] =  1.0;

    if WithFluff.contains(p3) then
      A[p3] =  -1.0;

    if WithFluff.contains(p4) then
      A[p4] =  -1.0;

    //
    // Ensure we don't start computing until everybody's done initializing
    //
    b.barrier();

    if printArrays {
      //
      // Debug print to make sure everything got set up right
      //
      while (takeTurns$.readXX() != here.id) { }
      if here.id == 0 then
        writeln("Initial A:");
      writeln("locale #", here.id, "'s slab:\n", A[MyLocDom]);
      takeTurns$.writeXF((here.id + 1)%numLocales);
    }

    //
    // main loop
    //
    do {
      //
      // update fluff
      //
      for ij in PanelDom {
        // communication with ourselves is pointless, so skip that iteration
        if (ij != (0,0)) {
          //
          // compute the coordinates of our neighbor
          //
          var neighbor = (lr,lc)+ij;

          //
          // make sure that neighbor exists; skip if they don't
          // miniMD would need to wrap around; I didn't want to do that
          //
          if (neighbor(1) < 0 || neighbor(2) < 0 ||
              neighbor(1) > LocaleGridDom.high(1) ||
              neighbor(2) > LocaleGridDom.high(2)) then
            // out of bounds
            continue;

          //
          // TODO: If we took out all the coordinated debug printing,
          // we would need to do some sort of synchronization to ensure
          // that our neighbors were ready for us to read their
          // values.  We could use the Barrier module/type to do so.
          //

          //
          // TODO: I think this conditional is unnecessary -- should remove
          //
          if (Dom[Panels[ij]].size > 0) {
            // Update our fluff by assigning from our neighbor's array
            // using the same coordinates.
            //
            // It's cool that we can use the same domain (Panels[ij])
            // for both source and destination; and slicing is a nice
            // way to express the copy.
            //
            // 
            A[Panels[ij]] = LocalDomArrs[neighbor]!.Arr[Panels[ij]];
          }
        }
      }

      /* Old debug print
      while (takeTurns$.readXX() != here.id) { }
      writeln("locale #", here.id, "'s slab:\n", A[MyLocDom]);
      takeTurns$.writeXF((here.id + 1)%numLocales);
      */

      //
      // Do computation on our local chunks
      //
      // TODO: wrap this in a local block.  Big perf boost expected
      //
      forall (i,j) in MyLocDom do
        B[i,j] = 0.25   * A[i,j]
               + 0.125  * (A[i+1,j  ] + A[i-1,j  ] + A[i  ,j-1] + A[i  ,j+1])
               + 0.0625 * (A[i-1,j-1] + A[i-1,j+1] + A[i+1,j-1] + A[i+1,j+1]);

      /* Old debug print
      while (takeTurns$.readXX() != here.id) { }
      writeln("locale #", here.id, "'s slab:\n", A[MyLocDom]);
      takeTurns$.writeXF((here.id + 1)%numLocales);
      */

      //
      // Barrier to make sure we don't swap our A's and B's while
      // someone is still reading our A's to update their fluff.
      //
      b.barrier();

      // TODO: could parallelize if we made the reduction safe
      var locDelta = 0.0;
      for ij in MyLocDom {
        // local reduction
        const diff = abs(B[ij] - A[ij]);

        if (diff > locDelta) then
          locDelta = diff;

        // and swap while it's hot in cache
        B[ij] <=> A[ij];
      }

      //
      // global reduction -- everybody accumulate into the global delta$
      // I wrapped each of the following two idioms in the takeTurns$
      // framework in order to guarantee that nobody read delta$ before
      // everyone had written to it; another approach would be to insert
      // a barrier between the two idioms (e.g., potentially using the
      // poor man's barrier from above?).  But I think what we have here
      // is also correct.
      //
      // TODO: Should consider using an atomic with CAS rather than a
      // sync here...
      //
      while (takeTurns$.readXX() != here.id) { }
      const prevDelta = delta$;
      if (locDelta > prevDelta) then
        delta$ = locDelta;
      else
        delta$ = prevDelta;
      takeTurns$.writeXF((here.id + 1)%numLocales);

      // Now delta$ holds the global max delta

      // See whether we're done
      while (takeTurns$.readXX() != here.id) { }
      const done = (delta$.readXX() <= epsilon);
      takeTurns$.writeXF((here.id + 1)%numLocales);

      // reset delta for next iteration (TODO: could do this only
      // when !done).  And increment the # of iterations
      if here.id == numLocales-1 {
        delta$.writeXF(0);
        numIters.add(1);
      }

    } while (!done);
  }
}

//
// 
//
if printArrays {
  //
  // This is a global-view array across the global problem size;
  // We'll have everybody assign their little fragmented piece
  // into the global array in order to simplify printing.
  //
  // TODO: Note that Result is local to locale #0.  Should've
  // used the 'Dom' block-distributed version instead for scalability.
  //
  var Result: [LocDom] real;

  //
  // iterate over array of local dom/arr descriptors
  //
  for lda in LocalDomArrs {
    var Interior = lda!.Dom.expand(-1);
    Result[Interior] = lda!.Arr[Interior];
  }
  //
  // print out the global result array
  //
  writeln("Final A:\n", Result);
}

//
// use as checksum
//
writeln("# iterations: ", numIters.read());

for loc in LocalDomArrs do delete loc;
