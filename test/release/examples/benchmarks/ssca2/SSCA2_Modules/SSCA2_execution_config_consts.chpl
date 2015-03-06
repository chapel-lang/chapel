module SSCA2_execution_config_consts
{
  // +======================================================+
  // |  Execution time constants control                    |
  // |                                                      |
  // |    1.  Problem Sizes                                 |
  // |        - number of vertices                          |
  // |        - size of starting vertex set for approximate |
  // |          betweenness centrality computation          |
  // |    2.  Graph Representations to test                 |
  // |    3.  RMAT generator constants                      |
  // |                                                      |
  // |   All can be set from the command line.              |
  // +======================================================+

  // -------------
  // Problem Size:
  // -------------

  config const SCALE = 6;

  config const TOP_APPROX_SCALE = 6;

  config const LOW_APPROX_SCALE = 4;

  const N_VERTICES = 2**SCALE;
    
  const MAX_EDGE_WEIGHT = 2**SCALE;

  config const SUBGRAPH_PATH_LENGTH = 3;

  // --------------------
  // Graph Types to Test:
  // --------------------

  enum graph_type { RMAT_associative,
		    Torus_1D,   Torus_2D,
		    Torus_3D,   Torus_4D };

  const graph_type_domain : domain ( graph_type );

  var testing_type : [graph_type_domain] bool = false;

  config const TEST_RMAT_ASSOCIATIVE = true,
               TEST_TORUS_1D         = false,
	       TEST_TORUS_2D         = false,
	       TEST_TORUS_3D         = false,
	       TEST_TORUS_4D         = false;

  config const RUN_KERNEL2 = true;
  config const RUN_KERNEL3 = true;
  config const RUN_KERNEL4 = true;

  testing_type (graph_type.RMAT_associative) = TEST_RMAT_ASSOCIATIVE; 
  testing_type (graph_type.Torus_1D)         = TEST_TORUS_1D;
  testing_type (graph_type.Torus_2D)         = TEST_TORUS_2D;
  testing_type (graph_type.Torus_3D)         = TEST_TORUS_3D;
  testing_type (graph_type.Torus_4D)         = TEST_TORUS_4D;

  // RMAT power law generation constants; these four should add to one

  config const RMAT_a = .55;

  config const RMAT_b = .10;

  config const RMAT_c = .10;

  config const RMAT_d = .25;

  // Torus indexing base, lower bound for each torus dimension.  The code is
  // polymorphic over any value, in particular for either zero or one-based 
  // indexing

  config param Torus_Base_Index = 1;
}
