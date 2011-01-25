module cholesky_test {
 
  use CyclicDist;
 
  config const n = 59; 
  config const index_base = -3;
 
  proc main {
 
    const MatIdx = [ index_base .. #n, index_base .. #n ];
 
    const mat_dom : domain (2) dmapped Cyclic ( startIdx = MatIdx.low )
      = MatIdx;
 
    var L : [mat_dom] real;
 
    var positive_definite : bool;
 
    positive_definite = elemental_cholesky ( L );
 
   }
 
  proc elemental_cholesky ( A : [] )
    {
    class barrier_data {
      var   tasks_to_finish$         : sync int; // = n_processors;
      var   first_barrier_unlocked$  : sync bool;
      var   second_barrier_unlocked$ : sync bool; // = true;
    }
 
    var locks : barrier_data = new barrier_data ( );
 
    return true;
  }
 
}
