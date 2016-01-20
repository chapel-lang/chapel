//This code is based on the XMT version of the Graph5000 benchmark submitted by
//John Feo <john.feo@pnl.gov> and Kristi Maschhoff <kristyn@cray.com> in 2010.

module Scalable_Graph_Generator
{
   use Graph500_defs;

// A Chapel implementation of the Scalable Graph Generator 
// for the Graph500 Benchmark

// The scalable data generator constructs a list of edge tuples containing 
// vertex identifiers. Each edge is undirected with its endpoints given in 
// the tuple as StartVertex and EndVertex. 

// I am currently leveraging some of the code from the Chapel implementation 
// of the RMAT Graph generator developed by John Lewis for SSCA2 but 
// eventually will try to more closely follow the XMT implementation of the 
// ScalableGraphGenerator.c used for the 2010 Graph500 submission

// Here I will use a similar data structure as the SSCA2 code and have Edges
// be a colelction of records, with each record a vertex pair

// These are now defined in defs.chpl
//  record directed_vertex_pair {
//    var start = 1: int;
//    var end   = 1: int;
//  }
//
//  proc +(l: directed_vertex_pair, r: directed_vertex_pair)
//      return new directed_vertex_pair (l.start + r.start, l.end + r.end);

  // ============================
  // Quadrant selection algorithm
  // ============================

  proc assign_quadrant ( u: real, a: real, b: real, c: real, d : real,
                        bit : int ) : directed_vertex_pair
    {
      var start_inc = 0;
      var end_inc   = 0;
      var edge : directed_vertex_pair;

      if u <= a then
        {}
      else if u <= a + b then
        { end_inc = 1;}
      else if u <= a + b + c then
        { start_inc = 1;}
      else
        { start_inc = 1; end_inc = 1;};

      edge.start = bit * start_inc;
      edge.end   = bit * end_inc;
      return ( edge );
    }


  // ====================================
  // Main RMAT Graph Generation Procedure
  // ====================================~

// Note that we include the inquiry of the domain type for edges so we can use 
// this to allocate additional arrays using the same distribution

  proc Scalable_Data_Generator ( scale :int, n_vertices : int,
                                n_raw_edges : int, Edges:[?ArrD] )

    {
      use BlockDist;
      use Graph500_defs;
      use Random;
      use Time;

      // Random Numbers return in the range [0.0, 1.0)

      var Rand_Gen = if REPRODUCIBLE_PROBLEMS then
                       new RandomStream (seed = 0556707007)
                     else
                       new RandomStream ();


      const vertex_range = 1..n_vertices;

// Graph500 settings for RMAT parameters
        const a: real = 0.57;
        const b: real = 0.19;
        const c: real = 0.19;
        const d: real = 0.05;

// SSCA2 settings for RMAT parameters
//        const a: real = 0.55;
//        const b: real = 0.10;
//        const c: real = 0.10;
//        const d: real = 0.25;

        const N_SHUFFLE = scale*n_vertices;
        const Half_N_SHUFFLE = scale*n_vertices/2;

//      const ab: real = a+b; 
//            abc: real = a+b+c;

      var   Noisy_a     : [ArrD] real,
            Noisy_b     : [ArrD] real,
            Noisy_c     : [ArrD] real,
            Noisy_d     : [ArrD] real,
            norm        : [ArrD] real;


      var   Unif_Random  : [ArrD] real;
      var   Unif_Random2 : [ArrD] real;

      var   Edge_lock$   : [ArrD] sync bool = true;

      var   graph_gen_time: Timer;


// Initialize records in Edges

   Edges.start = 1;
   Edges.end = 1;


// Step 0: Construct permutation array
      graph_gen_time.clear();
      graph_gen_time.start();

      var permutation$ : [vertex_range] sync int = vertex_range;
// Note: need to be very careful with sync variables as a writeln
// invokes a read, which then sets to empty

   for i in 1..scale do {
      var   skip : real;
      Rand_Gen.fillRandom ( Unif_Random );
      skip = Rand_Gen.getNext ();
      Rand_Gen.fillRandom ( Unif_Random2 );

   forall j in ArrD do
     { 

//     Choose two locations at random
       var ndx1 = floor (1 + Unif_Random (j) * n_vertices) : int;
       var ndx2 = floor (1 + Unif_Random2(j) * n_vertices) : int;

//     If the locations are not the same, then swap
       if (ndx1 != ndx2){

//       If the first location is greater than the second, swap. Insures
//       that the locations are locked in order, preventing deadlock
         if (ndx1 > ndx2) {ndx1 <=> ndx2;};

//       Lock locations in permutation array

         var label1 = permutation$ (ndx1).readFE () : int;
         var label2 = permutation$ (ndx2).readFE () : int;

//       Swap labels

         permutation$ (ndx1).writeEF (label2);
         permutation$ (ndx2).writeEF (label1);
       };

     }; 
     }; 
      graph_gen_time.stop();
     if ENABLE_PRINTOUTS then
      writeln("Time for SDG: Construct Perm Array  = ", graph_gen_time.elapsed());

// Step 1: Construct edges
      graph_gen_time.clear();
      graph_gen_time.start();

// Approach A: Leverage Lewis code using assign_quadrant, but this looks 
//             to have multiple trips through the full list of edges

  if RMAT_WITH_NOISE then {
      var bit = 1 << scale;

      for depth in 1..scale do {
          bit >>= 1;
          var   skip : real;

          // randomize the coefficients, tweaking them by numbers in [-.05, .05)

          skip = Rand_Gen.getNext ();
          Rand_Gen.fillRandom (Unif_Random);
          Noisy_a = a * (0.95 + 0.1 * Unif_Random);

          skip = Rand_Gen.getNext ();
          Rand_Gen.fillRandom (Unif_Random);
          Noisy_b = b * (0.95 + 0.1 * Unif_Random);

          skip = Rand_Gen.getNext ();
          Rand_Gen.fillRandom (Unif_Random);
          Noisy_c = c * (0.95 + 0.1 * Unif_Random);

          skip = Rand_Gen.getNext ();
          Rand_Gen.fillRandom (Unif_Random);
          Noisy_d = d * (0.95 + 0.1 * Unif_Random);

          norm     = 1.0 / (Noisy_a + Noisy_b + Noisy_c + Noisy_d);

          Noisy_a *= norm;
          Noisy_b *= norm;
          Noisy_c *= norm;
          Noisy_d *= norm;

          skip = Rand_Gen.getNext ();
          Rand_Gen.fillRandom (Unif_Random);


          Edges += assign_quadrant ( Unif_Random, Noisy_a, Noisy_b,
                                     Noisy_c, Noisy_d, bit );

        };
   }
   else
   {

//    Use faster code with does not include noise

      var bit = 1 << scale;

      for depth in 1..scale do {
          bit >>= 1;
          var   skip : real;

          skip = Rand_Gen.getNext ();
          Rand_Gen.fillRandom (Unif_Random);

          forall e in ArrD do {

            var start_inc = 0;
            var end_inc   = 0;
            var u = Unif_Random[e];

            if u <= a then
              {}
            else if u <= a + b then
              { end_inc = 1;}
            else if u <= a + b + c then
              { start_inc = 1;}
            else
              { start_inc = 1; end_inc = 1;};

            Edges(e).start += bit * start_inc;
            Edges(e).end   += bit * end_inc;
          }
     }

   }
      graph_gen_time.stop();
     if ENABLE_PRINTOUTS then
      writeln("Time for SDG: Construct Edges  = ", graph_gen_time.elapsed());

// Permute labels
   graph_gen_time.clear();
   graph_gen_time.start();

   forall e in ArrD do {
      Edges(e).start = permutation$ (Edges(e).start).readFF();
      Edges(e).end   = permutation$ (Edges(e).end  ).readFF();
   };
   graph_gen_time.stop();
  if ENABLE_PRINTOUTS then
   writeln("Time for SDG: Permute labels  = ", graph_gen_time.elapsed());

// Step 2: Shuffle edges
   if RMAT_WITH_SHUFFLE then {
   graph_gen_time.clear();
   graph_gen_time.start();

//   Sample specification only applies edgefactor*N swaps
//   for i in 1..scale do {
      var   skip : real;
      Rand_Gen.fillRandom ( Unif_Random );
      skip = Rand_Gen.getNext ();
      Rand_Gen.fillRandom ( Unif_Random2 );

     forall j in ArrD do
     { 

//     Choose two locations at random
       var ndx1 = floor (1 + Unif_Random (j) * n_vertices) : int;
       var ndx2 = floor (1 + Unif_Random2 (j) * n_vertices) : int;

//     If the locations are not the same, then swap
       if (ndx1 != ndx2){

//       If the first location is greater than the second, swap. Insures
//       that the locations are locked in order, preventing deadlock
         if (ndx1 > ndx2) {ndx1 <=> ndx2; };

//       Lock Edge Pairs

         Edge_lock$ (ndx1).readFE();
         Edge_lock$ (ndx2).readFE();
         var label1 = Edges (ndx1).start : int;
         var label2 = Edges (ndx1).end : int;
         var label3 = Edges (ndx2).start : int;
         var label4 = Edges (ndx2).end : int;

//       Swap Edge Pairs

         Edges (ndx1).start = label3;
         Edges (ndx1).end = label4;
         Edges (ndx2).start = label1;
         Edges (ndx2).end = label2;
         Edge_lock$ (ndx1).writeEF(true);
         Edge_lock$ (ndx2).writeEF(true);
       };
//     };
     };

     graph_gen_time.stop();

    if ENABLE_PRINTOUTS then
     writeln("Time for SDG: Shuffle Edges  = ", graph_gen_time.elapsed());
     }

//   writeln("Upon exit, Edges is:\n", Edges, "\n");
}
}
