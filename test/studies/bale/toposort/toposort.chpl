use Random;
use Time;
use LayoutCS;

config const debugAll : bool = false;
config const debugTopo = debugAll;
config const debugTopoTasking = debugAll || debugTopo;
config const debugCreateSparseUTDomain = debugAll;
config const debugPermute = debugAll;
config param use_dimIter = true;
config param use_dimIterRow = false; // not currently supported on master
config param use_dimIterCol = use_dimIter;

record Lock {
  var lock$ : sync bool;

  proc init(){
    this.complete();
    this.lock$.writeEF(true);
  }

  proc lock(){
    lock$.readFE();
  }

  proc unlock(){
    lock$.writeEF(true);
  }

  proc forceUnlock(){
    lock$.writeXF(true);
  }
}

class PermutationMap {
  type idxType;
  param rank = 2;
  var row_dom : domain(1);
  var column_dom : domain(1);
  var row_map : [row_dom] idxType;
  var column_map : [column_dom] idxType;

  proc init( row_map : [] ?idxType, column_map : [] idxType ){
    this.idxType = idxType;
    this.row_dom = row_map.domain;
    this.column_dom = column_map.domain;
    this.row_map = row_map;
    this.column_map = column_map;
  }

  inline proc map( idx : rank*idxType ) : rank*idxType {
    return (row_map[idx[1]], column_map[idx[2]]);
  }

  inline proc inverseMap( idx : rank*idxType ) : rank*idxType {
    return ( linearSearch(row_map, idx[1]), linearSearch(row_map, idx[1]) );
  }

  inline proc this( idx : rank*idxType ) : rank*idxType {
    return map( idx );
  }

  proc createInverseMap() : PermutationMap(idxType) {
    var inverse_row_map : [row_map.domain] row_map.eltType;
    var inverse_column_map : [column_map.domain] column_map.eltType;

    for i in row_map.domain {
      inverse_row_map[ row_map[i] ] = i;
    }

    for i in column_map.domain {
      inverse_column_map[ column_map[i] ] = i;
    }

    return new PermutationMap( inverse_row_map, inverse_column_map );
  }

  iter these( on_domain : domain ) : rank*idxType
  where on_domain.rank == 2
  {
    for idx in on_domain do yield this.map( idx );
  }

  iter inverseThese( on_domain : domain ) : rank*idxType
  where on_domain.rank == 2
  {
    for idx in on_domain do yield inverseMap( idx );
  }


  // TODO make parallel
  iter these(param tag : iterKind, on_domain : domain) : rank*idxType
  where tag == iterKind.standalone && on_domain.rank == 2
  {
    for idx in this.these( on_domain ) do yield idx;
  }

  // TODO leader follower iterator

  proc writeThis( f ){
    const max_val = max( (max reduce row_map), (max reduce column_map) ) : string;
    const min_val = min( (min reduce row_map), (min reduce column_map) ) : string;
    const padding = max( max_val.length, min_val.length );
    const format_string = "%%%nn -> %%%nn".format( max(2,padding), padding );
    const in_space = max(padding-2,0);
    f <~> "Row map\n";
    for i in 0..#in_space do f <~> " ";
    f <~> "in -> out\n";
    for i in row_dom {
      f <~> format_string.format( i, row_map[i] ) <~> "\n";
    }
    f <~> "Column map\n" ;
    for i in 0..#in_space do f <~> " ";
    f <~> "in -> out\n";
    for i in column_dom {
      f <~> format_string.format( i, column_map[i] ) <~> "\n";
    }
  }
}

proc create_random_permutation_map( D : domain ) : PermutationMap(D.idxType)
where D.rank == 2
{
  var row_map : [D.dim(1)] D.idxType = D.dim(1);
  var column_map : [D.dim(2)] D.idxType = D.dim(2);
  shuffle( row_map );
  shuffle( column_map );
  return new PermutationMap( row_map, column_map );
}

proc permute( M : [?D] ?T, permutation_map : PermutationMap(D.idxType), copy_values : bool = false )
where D.rank == 2 && isSparseDom( D )
{
  var timer : Timer;
  timer.start();
  var sD : if isSparseDom(D) then D.type else sparse subdomain({D.dim(1),D.dim(2)});
  timer.stop();
  if debugPermute then writeln( "Create domain ", timer.elapsed() );
  timer.clear();

  timer.start();
  var sD_bulk : [1..#D.size] D.rank*D.idxType;
  // TODO make parallel
  for (pos,idx) in zip(1..#D.size, D) {
    sD_bulk[pos]= permutation_map[idx];
  }
  sD.bulkAdd( sD_bulk );
  timer.stop();
  if debugPermute then writeln( "Add domain ", timer.elapsed() );
  timer.clear();

  timer.start();
  var permuted_M : [sD] T;
  timer.stop();
  if debugPermute then writeln( "Create array ", timer.elapsed() );
  timer.clear();

  if copy_values {
    timer.start();
    permuted_M.irv = M.irv;
    forall idx in D {
      permuted_M[ permutation_map[idx] ] = M[idx];
    }
    timer.stop();
    if debugPermute then writeln( "Add array ", timer.elapsed() );
    timer.clear();
  }

  return permuted_M;
}

record TopoSortResult {
  type idxType;
  var permutation_map : PermutationMap(idxType);
  var timer_dom : domain(string);
  var timers : [timer_dom] Timer;

  proc init(type idxType){
    this.idxType = idxType;
    this.permutation_map = nil;
    this.timer_dom = {"whole","initialization","toposort"};
  }
}

proc toposort_parallel( M : [?D] ?T, numTasks : int = here.maxTaskPar )
where D.rank == 2
{
  if numTasks < 1 then halt("Must run with numTaks >= 1");

  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  var num_diagonals = min( D.dim(1).size, D.dim(2).size );

  var row_map : [D.dim(1)] D.idxType = [i in D.dim(1)] -1;
  var column_map : [D.dim(2)] D.idxType = [i in D.dim(2)] -1;

  var row_sum : [D.dim(1)] atomic int;
  var row_count : [D.dim(1)] atomic int;
  var work_queue : list(D.idxType);
  var shared_lock = new Lock();

  // initialize row_count and row_sum and put work in queue
  result.timers["initialization"].start();
  forall row in D.dim(1) with (ref row_sum, ref row_count, ref work_queue, ref shared_lock) {
    // Accumulate task locally, then write at end.
    var count = 0;
    var sum = 0;

    if debugTopo then writeln( "initializing row ", row );
    if use_dimIterCol {
     // compilerWarning("iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        count += 1;
        sum += col;
      }
    } else {
      // compilerWarning("iterating over columns in init with dim");
      for col in D.dim(2) {
        if D.member((row,col)) {
          count += 1;
          sum += col;
        }
      }
    }
    if count == 1 {
      /* result.timers["init_queue"].start(); */
      shared_lock.lock();
      work_queue.push_back( row );
      shared_lock.unlock();
      /* result.timers["init_stop"].stop(); */
    }

    row_count[row].write( count);
    row_sum[row].write( sum );
  }
  result.timers["initialization"].stop();

  if debugTopo {
    writeln( "initial work_queue ", work_queue );
    writeln( "initial row_sum    ", row_sum );
    writeln( "initial row_count  ", row_count );
  }

  // insert position along diagonal from (N,N)
  // must be accessed under shared_lock
  var diagonal_position : int = 0;
  // Values for work signal
  enum TaskSignal {
    kill = min(int), // no more work signal, die when recieving
    fail = kill : int + 1, // signal that someone failed, and so should exit with error
    wake = 1 // more work signal, pop from stack and continue with recieving
  };
  // This will signal tasks to get work, and keeps track of queue size;
  var work_signal : sync TaskSignal;
  // initialize work signal.
  // sanity check: if queue is empty, set to kill signal.
  // (I dont think this-^ is possible, but it could in the future?)
  work_signal.writeEF( if work_queue.size > 0 then TaskSignal.wake else TaskSignal.kill );
  if debugTopoTasking then writeln( "Spawning tasks ", 1..#numTasks );

  result.timers["toposort"].start();
  coforall task_id in 1..#numTasks
    with (ref diagonal_position, ref row_sum, ref row_count, ref work_queue, ref shared_lock)
  {
    if debugTopoTasking then writeln(task_id, " spawning." );
    // continue looping until kill or fail signal is recieved
    var continue_working = true;
    while continue_working {
      if debugTopoTasking then writeln( task_id, " waiting." );
      // wait for work and get signal
      var signal = work_signal;
      if debugTopoTasking then writeln( task_id, " recieved signal: ", signal );
      // if signal is kill signal relay message and exit
      select signal {
        when TaskSignal.kill {
          work_signal.writeEF( TaskSignal.kill );
          continue_working = false;
          if debugTopoTasking then writeln( task_id, " killed." );
          break; // break out of loop;
        }
        when TaskSignal.fail {
          work_signal.writeXF( TaskSignal.fail );
          continue_working = false;
          if debugTopoTasking then writeln( task_id, " failing out.");
          break; // break out of loop;
        }
        otherwise { /* no need to popogate signal */ }
      }

      // Critical section
      shared_lock.lock();
      if debugTopo {
        writeln(
          "========================",
          "\ntask_id ",  task_id,
          "\nwork_queue ", work_queue,
          "\nrow_sum    ", row_sum,
          "\nrow_count  ", row_count,
          "\nrow_map    ", row_map,
          "\ncolumn_map ", column_map,
          "\n========================"
        );
      }

      var queue_size = work_queue.size;
      // if queue_size is < 1, someone stole our work. unlock and rewait
      if queue_size < 1 {
        if debugTopoTasking then writeln( task_id, " queue size is ", queue_size);
        /* continue_working = false;
        work_signal.writeXF(TaskSignal.fail); */
        shared_lock.unlock();
        continue;
      }

      // safely get row and diagonal position
      var local_diagonal_position = diagonal_position;
      diagonal_position += 1; // increment to next position
      var swap_row = work_queue.pop_front();
      queue_size = work_queue.size;

      // if there are no more diagonal to place, then there is no work, send kill signal
      if debugTopo then writeln( (task_id, local_diagonal_position, diagonal_position) );
      if diagonal_position >= num_diagonals {
        if debugTopoTasking then writeln( task_id, " sending kill signal." );
        work_signal.writeXF( TaskSignal.kill );
        continue_working = false;
      } else if queue_size > 0 {
        if debugTopoTasking then writeln( task_id, " sending work signal." );
        work_signal.writeXF( TaskSignal.wake );
      }
      shared_lock.unlock();

      // get non-zero column
      var swap_column = row_sum[swap_row].read();

      // permute this row to the diagonal
      row_map[swap_row] = D.dim(1).high - local_diagonal_position;
      column_map[swap_column] = D.dim(2).high - local_diagonal_position;

      if debugTopo then writeln( "Swaping ", (swap_row,swap_column), " -> ", (row_map[swap_row], column_map[swap_column]) );

      // foreach row along the swapped column who has a nonzero at (row, swap_column)
      // remove swap_column from row_sum and reduce row_count
      if use_dimIterRow {
        // compilerWarning("iterating over rows in kernel with dimIter");
        for row in D.dimIter(1,swap_column) {
          var previous_row_count = row_count[row].fetchAdd( -1 );
          row_sum[row].add( -swap_column );
          // if previous_row_count = 2 (ie row_count[row] == 1)
          if previous_row_count == 2 {
            shared_lock.lock();
            if debugTopo then writeln( "Queueing ", row);
            work_queue.push_back( row );
            shared_lock.unlock();
            work_signal.writeXF( TaskSignal.wake );
          }
        }
      } else {
        // compilerWarning("iterating over rows in kernel with dim");
        for row in D.dim(1) {
          if D.member((row, swap_column)) {
            var previous_row_count = row_count[row].fetchAdd( -1 );
            row_sum[row].add( -swap_column );
            // if previous_row_count = 2 (ie row_count[row] == 1)
            if previous_row_count == 2 {
              shared_lock.lock();
              if debugTopo then writeln( "Queueing ", row);
              work_queue.push_back( row );
              shared_lock.unlock();
              work_signal.writeXF( TaskSignal.wake );
            }
          }
        }
      }

    } // while continue_working
    if debugTopoTasking then writeln( task_id, " completed." );
  } // coforall tasks
  result.timers["toposort"].stop();
  var signal = work_signal;
  if signal == TaskSignal.fail {
    writeln("Recieved fail signal at some point in execution.");
    exit( -1 );
  }

  result.permutation_map = new PermutationMap( row_map, column_map );

  result.timers["whole"].stop();

  return result;
}

proc toposort_serial( M : [?D] ?T )
where D.rank == 2
{
  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  var num_diagonals = min( D.dim(1).size, D.dim(2).size );

  var row_map : [D.dim(1)] D.idxType = [i in D.dim(1)] -1;
  var column_map : [D.dim(2)] D.idxType = [i in D.dim(2)] -1;

  var row_sum : [D.dim(1)] int;
  var row_count : [D.dim(1)] int;
  var work_queue : list(D.idxType);

  // initialize row_count and row_sum and put work in queue
  result.timers["initialization"].start();
  for row in D.dim(1) {
    if debugTopo then writeln( "initializing row ", row );
    if use_dimIterCol {
     // compilerWarning("iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        row_count[row] += 1;
        row_sum[row] += col;
      }
    } else {
      // compilerWarning("iterating over columns in init with dim");
      for col in D.dim(2) {
        if D.member((row,col)) {
          row_count[row] += 1;
          row_sum[row] += col;
        }
      }
    }

    if row_count[row] == 1 {
      work_queue.push_back( row );
    }
  }
  result.timers["initialization"].stop();

  if debugTopo {
    writeln( "initial work_queue ", work_queue );
    writeln( "initial row_sum    ", row_sum );
    writeln( "initial row_count  ", row_count );
  }

  // insert position along diagonal from (N,N)
  var diagonal_position : int = 0;

  result.timers["toposort"].start();
  while work_queue.size > 0 {
    if debugTopo {
      writeln(
        "========================",
        "\nwork_queue ", work_queue,
        "\nrow_sum    ", row_sum,
        "\nrow_count  ", row_count,
        "\nrow_map    ", row_map,
        "\ncolumn_map ", column_map,
        "\n========================"
      );
    }

    // get work row
    var swap_row = work_queue.pop_front();

    // get non-zero column
    var swap_column = row_sum[swap_row];

    // permute this row to the diagonal
    row_map[swap_row] = D.dim(1).high - diagonal_position;
    column_map[swap_column] = D.dim(2).high - diagonal_position;
    diagonal_position += 1; // increment to next position

    if debugTopo then writeln( "Swaping ", (swap_row,swap_column), " -> ", (row_map[swap_row], column_map[swap_column]) );

    // foreach row along the swapped column who has a nonzero at (row, swap_column)
    // remove swap_column from row_sum and reduce row_count
    if use_dimIterRow {
      // compilerWarning("iterating over rows in kernel with dimIter");
      for row in D.dimIter(1,swap_column) {
        row_count[row] -= 1;
        row_sum[row] -= swap_column;
        if row_count[row] == 1 {
          if debugTopo then writeln( "Queueing ", row);
          work_queue.push_back( row );
        }
      }
    } else {
      // compilerWarning("iterating over rows in kernel with dim");
      for row in D.dim(1) {
        if D.member((row, swap_column)) {
          row_count[row] -= 1;
          row_sum[row] -= swap_column;
          if row_count[row] == 1 {
            if debugTopo then writeln( "Queueing ", row);
            work_queue.push_back( row );
          }
        }
      }
    }

  } // while work in queue
  result.timers["toposort"].stop();

  result.permutation_map = new PermutationMap( row_map, column_map );
  result.timers["whole"].stop();
  return result;
}

proc create_sparse_upper_triangluar_domain( D : domain(2), density : real, seed : int ) {
  // Must be square matrix, uniformly dimensioned dense domain
  if D.dim(1) != D.dim(2) then halt("Domain provided to create_sparse_upper_triangluar_domain is not square.");
  const N = D.dim(1).size;
  const minDensity : real = 1.0/N;
  const maxDensity : real = (N+1.0)/(2.0*N);

  // assert we have been given a legal density;
  if density < minDensity then halt( "Specified density (%n) is less than minimum density (%n) for N (%n)".format( density, minDensity, N));
  if density > maxDensity then halt( "Specified density (%n) is greater than maximum density (%n) for N (%n)".format( density, maxDensity, N));

  // number of elements in complete UT matrix
  var number_non_zeros_in_full_UT_domain : int = (( N * N + N)/2.0) : int;
  // number of elements added
  var number_non_zeros_added_in_UT : int = max( N, floor( N*N*density ) ) : int;
  // number of non-diagonal elements added
  var number_non_zeros_added_in_strictly_UT : int = number_non_zeros_added_in_UT - N;

  if debugCreateSparseUTDomain {
    writeln( "Dense: ", number_non_zeros_in_full_UT_domain );
    writeln( "Added: ", number_non_zeros_added_in_UT );
    writeln( "Non-diagonal: ", number_non_zeros_added_in_strictly_UT );
  }

  // resulting sparse domain
  var sparse_D : sparse subdomain(D) dmapped CS();

  // if adding more than diagonals...
  if number_non_zeros_added_in_strictly_UT > 0 {

    var sD_random_dom : domain(1) = {1..#number_non_zeros_in_full_UT_domain-N};
    var sD_random : [sD_random_dom] D.rank*D.idxType; //[1..#if number_non_zeros_added_in_strictly_UT > 0 then number_non_zeros_in_full_UT_domain-N else 0] D.rank*D.idxType;

    // TODO figure out effecient way to add small number of non-zeros

    forall i in D.dim(1).low..D.dim(1).high-1 {

      const delta = (D.dim(1).high-1 - i);
      const position_offset = ( delta*delta + delta ) / 2;
      const position = position_offset;
      const col_range = i+1..D.dim(1).high;

      if debugCreateSparseUTDomain then writeln("filling ", i, " x ", col_range);
      for j in col_range {
        sD_random[ position + (j - i) ] = (i,j);
      }
    }

    shuffle( sD_random, seed );
    sparse_D.bulkAdd( sD_random[1..#number_non_zeros_added_in_strictly_UT] );
  }

  // Diagonal indices
  var sD_diag : [D.dim(1)] D.rank*D.idxType;
  forall i in D.dim(1) {
    sD_diag[i] = (i,i);
  }
  sparse_D.bulkAdd( sD_diag );

  if debugCreateSparseUTDomain then writeln( "there are ", sparse_D.size, " non zeros, for density of ", sparse_D.size / ( N*N : real ) );

  if sparse_D.size != number_non_zeros_added_in_UT then halt("Created a domain with unexpected number of non-zero indices. Created %n, expected %n".format(sparse_D.size, number_non_zeros_added_in_UT));

  return sparse_D;
}

proc check_is_uper_triangular_domain( D : domain ) : bool
where D.rank == 2 && isSparseDom( D )
{
  var is_UT = true;
  for (i,j) in D {
    is_UT = is_UT && (i <= j);
    if !is_UT then break;
  }
  return is_UT;
}

proc pretty_print_sparse( M : [?D] ?T, print_IRV : bool = false, separate_elements : bool = true )
where D.rank == 2
{
  const padding = max reduce ( [i in M] (i : string).length );
  const format_string = "%%%ns%s".format( padding, if separate_elements then " " else "" );
  const blank_list = [i in 1..#padding+if separate_elements then 1 else 0 ] " ";
  const blank_string = "".join( blank_list );

  for i in D.dim(1){
    for j in D.dim(2){
      if print_IRV || D.member((i,j))
        then writef( format_string, M[i,j] : string );
        else write( blank_string );
    }
    writeln();
  }
}

// Number of rows and columns (square matrix)
config const N = 5;

// Density of matrix with only diagonal.
const minDensity : real = 1.0/N;
// Density of matrix with UT region totally filled
const maxDensity : real = (N+1.0)/(2.0*N);

// density in addition to minimum (for ease of use)
// density parameter will be calculated from this.
config const additionalDensity : real = 1.0 - minDensity;

config const density : real = min( maxDensity, minDensity + max(0, additionalDensity) );
config type eltType = string;

config const numTasks : int = here.maxTaskPar;

config const silentMode : bool = false;
config const printPerfStats : bool = false;
config const printMatrices : bool = false;
config const printNonZeros : bool = false;
config const printPermutations: bool = false;
config const padPrintedMatrixElements = true;
config const seed : int = SeedGenerator.oddCurrentTime;

enum ToposortImplementation { Serial, Parallel };
config const implementation : ToposortImplementation = ToposortImplementation.Parallel;

proc main(){
  if density < minDensity then halt("Specified density (%n) is less than min density (%n) for N (%n)".format( density, minDensity, N) );
  if density > maxDensity then halt("Specified density (%n) is greater than max density (%n) for N (%n)".format( density, maxDensity, N) );

  const sparse_number_non_zeros = max(0, floor( N*N*density) ) : int;
  const actual_density = sparse_number_non_zeros / (1.0*N*N);
  if !silentMode then writeln( "Number of tasks: %n\nN: %n\nSpecified density: %dr%%\nActual density %dr%%\nTotal Number NonZeros: %n".format(numTasks, N, density * 100.0, actual_density * 100.0, sparse_number_non_zeros) );

  // create upper triangular matrix
  const D : domain(2) = {1..#N,1..#N};
  if !silentMode then writeln("Creating sparse upper triangluar domain");
  const sparse_D = create_sparse_upper_triangluar_domain( D, density, seed );
  var M : [sparse_D] eltType;
  /* compilerError("M.dom: " + M.domain.type : string ); */

  if printMatrices {
    if !silentMode then writeln("Filling matrix with values");
    if isNumericType( eltType ) {
      var v = 1;
      for idx in sparse_D {
        M[idx] = v : eltType;
        v += 1;
      }
    } else if isStringType( eltType ) {
      M = "X";
      M.irv = "_";
    }
  }

  if printMatrices {
    writeln( "Upper triangluar matrix:" );
    pretty_print_sparse( M, print_IRV = printNonZeros, separate_elements = padPrintedMatrixElements );
  }

  var permutation_map = create_random_permutation_map( sparse_D );
  if printPermutations then writeln("Permutation Map:\n", permutation_map);

  if !silentMode then writeln("Permuting upper triangluar matrix");
  var permuted_M = permute( M, permutation_map, printMatrices );


  if printMatrices {
    writeln( "Permuted upper triangluar matrix:" );
    pretty_print_sparse( permuted_M, print_IRV = printNonZeros, separate_elements = padPrintedMatrixElements );
  }


  var topo_result : TopoSortResult(M.domain.idxType);

  select implementation {
    when ToposortImplementation.Serial {
      if !silentMode then writeln("Toposorting permuted upper triangluar matrix using Serial implementation.");
      topo_result = toposort_serial( permuted_M );
    }
    when ToposortImplementation.Parallel {
      if !silentMode then writeln("Toposorting permuted upper triangluar matrix using Parallel implementation.");
      topo_result = toposort_parallel( permuted_M, numTasks );
    }
    otherwise {
      writeln( "Unknown implementation: ", implementation );
      exit(-1);
    }
  }

  var solved_map = topo_result.permutation_map;

  if printPerfStats {
    writeln( "Benchmark timers:");
    for timer_name in topo_result.timer_dom {
      writeln(timer_name, ": ", topo_result.timers[timer_name].elapsed() );
    }
    writeln( "Rows/second: ", (N/topo_result.timers["whole"].elapsed()) );
  }

  if printPermutations then writeln( "Solved permutation map:\n", solved_map );

  var solved_permuated_permuted_M = permute( permuted_M, solved_map, printMatrices );

  if printMatrices {
    writeln( "Solved-permuted permuted upper triangluar matrix:" );
    pretty_print_sparse( solved_permuated_permuted_M, print_IRV = printNonZeros, separate_elements = padPrintedMatrixElements );
  }
  var is_UT = check_is_uper_triangular_domain( solved_permuated_permuted_M.domain );
  if !is_UT then halt("Solved-permuted permuted upper triangluar matrix is not upper triangular!");
}
