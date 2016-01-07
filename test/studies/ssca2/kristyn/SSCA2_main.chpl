module SSCA2_main

// +===========================================================================+
// |       Chapel Implementation of SSCA #2 Benchmark Version 2.2              |
// |                                                                           |
// |            Implementation version 1.0, March 1, 2010                      |
// |                                                                           |
// | Primary characteristics demonstrated:                                     |
// |                                                                           |
// |   o  Graph algorithms can be expressed in set-like notation, using only   |
// |      the capabilities to instantiate a set, add to it and iterate over    |
// |      it.  All of these capabilities are supported natively by Chapel      |
// |      sparse arrays and by Chapel associative domains.                     |
// |                                                                           |
// |   o  Chapel's separation of concerns makes it possible to write           |
// |      polymorphic code that accepts many different underlying data         |
// |      structures.  This is demonstrated in this code by an implementation  |
// |      of the SSCA #2 kernels which accepts multiple representations of     |
// |      graphs.  The graph representations provide only the capabilities to  |
// |      iterate over neighbor lists (sets of vertices) and to extract the    |
// |      respective edge weights (integer arrays over sets of vertices).      |
// |                                                                           |
// | Eventual goals for this Chapel implementation:                            |
// |                                                                           |
// |   o  Provide a fully parallel implementation with tuning for locality     |
// |                                                                           |
// |   o  Demonstrate and compare sparse array and associative array           |
// |      implementations (polymorphism in array types)                        |
// |                                                                           |
// |   o  Demonstrate polymorphism in graph data structures by providing       |
// |      - associative array and sparse array representations of general      |
// |        RMAT power law graphs                                              |
// |      - native tuple representations of regular tori, with both dense      |
// |        array and stencil (sparse array) representations of neighbor sets. |
// |                                                                           |
// | Compiler-imposed limitations on this preliminary version                  |
// |                                                                           |
// |   o  Associative domain and associative array representations are         |
// |      provided.  Sparse domaina and sparse array representations are not   |
// |      yet supported adequately (no parallel iteration, no array slicing).  |
// |                                                                           |
// |      Only the associative domain versions of the kernel codes are         |
// |      now implemented.                                                     |
// |                                                                           |
// |      The polymorphic graph representations provided here are              |
// |      - associative domain representations of RMAT graphs                  |
// |      - dense array representations of regular tori.                       |
// |      - sparse array stencil representations of regular tori.              |
// |                                                                           |
// |   o A key zippered iteration in kernel 2 is not supported in parallel     |
// |     yet for associative domains. Therefore, the RMAT graph problems       |
// |     do not yet run in parallel.  The dense array versions of regular      |
// |     tori do run in parallel.                                              |
// |                                                                           |
// |   o More important, a zippered reduction in the second pass of kernel 4   |
// |     is not yet supported in parallel for associative domains.             |
// |                                                                           |
// |   o Atomic operations are not supported.  Atomicity (or locks) is         |
// |     obtained with sync variables.  The expectations on full / empty bits  |
// |     is made completely explicit by specifying the operations fully;       |
// |     defaults for reads and writes of sync variables are not used.         |
// |                                                                           |
// |   o No tuning for locality has been done to date.                         |
// |                                                                           |
// +---------------------------------------------------------------------------+
// |                                                                           |
// | File Structure:                                                           |
// |   SSCA2_main.chpl -- (this file) main driver for polymorphism over        |
// |                      graph representations                                |
// |   SSCA2_compilation_config_params.chpl -- parameters that control         |
// |                      conditional compilation.                             |
// |   SSCA2_execution_config_consts.chpl -- defaults for execution time       |
// |                      problem size parameters                              |
// |   SSCA2_driver.chpl -- code for problem set up, execution timing and      |
// |                      documenting results                                  |
// |   SSCA2_kernels.chpl -- code for Kernels 2, 3 and 4                       |
// |                                                                           |
// |   analyze_RMAT_graph_associate_array.chpl -- code to represent an         |
// |                      irregular sparse graph with associative arrays,      |
// |                      with graph generation specialized to RMAT graphs.    |
// |                      We expect a future development to include a          |
// |                      completely compatible sparse array implementation,   |
// |                      mirroring the associative array implementation.      |
// |   SSCA2_RMAT_graph_generator.chpl -- generic code to generate RMAT        |
// |                      approximate power level graphs                       |
// |                                                                           |
// |   analyze_torus_graphs_....chpl -- code to represent regular tori of      |
// |                      dimensions 1 to 4, of varying levels for genericity. |
// |                      provided in four versions, with the selected version |
// |                      to be specified on the compilation command line.     |
// |                      Versions available, each in a separate file:         |
// |                      o  dense array representation of neighbor lists      |
// |                      o  explicit sparse array representation of neighbor  |
// |                         lists, indexed over a sparse array stencil domain |
// |                      o  semi-implicit representation of neighbor lists,   |
// |                         using a dense extended domain to provide indirect |
// |                         indexing of torus neighbors                       |
// |                      o  implicit representation of neighbor lists, in     |
// |                         which each neighbor index is computed as needed,  |
// |                         accounting for boundary conditions in the         |
// |                         the computation of the index.                     |
// |   torus_graph_generator_utilities.chpl -- utilities common to the torus   |
// |                         graph implementations.                            |
// +===========================================================================+

{
  proc main () {

    use SSCA2_compilation_config_params, 
        SSCA2_execution_config_consts,
        analyze_torus_graphs,
        analyze_RMAT_graph_associative_array,
        analyze_RMAT_graph_1D_array,
        BlockDist;

//  Write out locale information
    coforall loc in Locales do
    {
    on loc do
      writeln("Locale ID: ", loc.id, " of ", numLocales);
      writeln("Locale ID: ", loc.id, " Number of cores " , loc.numPUs());
      writeln("Locale ID: ", loc.id, " Max task parallelism " , loc.maxTaskPar);
    }
    
    var graph_types : domain (graph_type);

    for this_graph_type in graph_types do {

      if testing_type (this_graph_type) then {

	writeln ( "=========================================================" );
	writeln ( "Problem Dimensions");
	writeln ( "                   Scale: ", SCALE );
	writeln ( "      Number of Vertices: ", N_VERTICES );
	writeln ( "    Kernel 3 Path Length: ", SUBGRAPH_PATH_LENGTH );
	writeln ( "    Kernel 4 Starting Set range" );
	writeln ( "        low scale: ", LOW_APPROX_SCALE );
	writeln ( "       high scale: ", TOP_APPROX_SCALE );
	writeln ();
	writeln ( "Code Characteristics");

	if FILTERING then
	  writeln ( "   Filtering Edges by Weight" );
	else
	  writeln ( "   Not filtering Edges by Weight" );

	if REPRODUCIBLE_PROBLEMS then
	  writeln ( "   Reproducible problem" );
	else
	  writeln ( "   Randomizing problems" );

	if VALIDATE_BC then
	  writeln ( "   Collecting graph statistics for bounds computation" );
	else
	  writeln ( "   No additional work for bounds computation" );

	if PRINT_TIMING_STATISTICS then
	  writeln ( "   Collecting timing statistics" );
	else
	  writeln ( "   Not collecting timing statistics" );

	if DISTRIBUTION_TYPE == "BLOCK" then
	  writeln ( "   Vertex domain is block distributed" );
	else
	  writeln ( "   Vertex domain is not distributed" );

	if this_graph_type == graph_type.RMAT_associative  then
	  writeln ( "   RMAT graph generation constants:", 
		    (RMAT_a, RMAT_b, RMAT_c , RMAT_d) );
	writeln ( "=========================================================" );


	select this_graph_type {

           when graph_type.Torus_1D do

             // ----------------------------------------------------
             // compute graph of one dimensional torus with  2^SCALE
             // vertices, then execute Kernels 2, 3 and 4 of SSCA #2
             // ----------------------------------------------------

             if BUILD_TORUS_VERSIONS then
               generate_and_analyze_1D_torus;


           when graph_type.Torus_2D do

             // ----------------------------------------------
             // compute graph of an approximately square two 
             // dimensional torus with  2^SCALE vertices, then 
             // execute Kernels 2, 3 and 4 of SSCA #2
             // ----------------------------------------------

             if BUILD_TORUS_VERSIONS then
               generate_and_analyze_2D_torus;


           when graph_type.Torus_3D do

             // -----------------------------------------------
             // compute graph of an approximately cubical three
             // dimensional torus with  2^SCALE vertices, then 
             // execute Kernels 2, 3 and 4 of SSCA #2
             // -----------------------------------------------

             if BUILD_TORUS_VERSIONS then
               generate_and_analyze_3D_torus;


           when graph_type.Torus_4D do

             // ------------------------------------------------------------
             // compute graph of an approximately four-cubical torus with
             // 2^SCALE vertices, then execute Kernels 2, 3 and 4 of SSCA #2
             // ------------------------------------------------------------

             if BUILD_TORUS_VERSIONS then
               generate_and_analyze_4D_torus;

           when graph_type.RMAT_associative do

             // -----------------------------------------------------------------
             // compute a random power law graph with 2^SCALE vertices, using 
             // the RMAT generator. Initially generate a list of triples. 
             // Then convert it to a Chapel representation of a sparse graph, 
             // timing this step (Kernel 1).  Finally, execute Kernels 2, 3 and 4
             // of SSCA #2, using identically the same code as in the various
             // torus cases.
             // -----------------------------------------------------------------

             generate_and_analyze_associative_array_RMAT_graph_representation;

           when graph_type.RMAT_1D_array do

             // -----------------------------------------------------------------
             // compute a random power law graph with 2^SCALE vertices, using 
             // the RMAT generator. Initially generate a list of triples. 
             // Then convert it to a Chapel representation of a sparse graph, 
             // timing this step (Kernel 1).  Finally, execute Kernels 2, 3 and 4
             // of SSCA #2, using identically the same code as in the various
             // torus cases.
             // -----------------------------------------------------------------

             generate_and_analyze_1D_array_RMAT_graph_representation;
           } // end select/ 

         writeln ( "=========================================================" );
         writeln (); writeln ();
       }

    } // end of for over graph types

  } // end of main

}
