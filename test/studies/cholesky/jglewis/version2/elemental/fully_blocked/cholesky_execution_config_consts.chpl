module cholesky_execution_config_consts
{
  // +======================================================+
  // |  Execution time constants control                    |
  // |                                                      |
  // |    1.  Problem Size                                  |
  // |    2.  Block Size                                    |
  // |    3.  Indexing Base                                 |
  // |    4.  Offset from base of row indexing              |
  // |    5.  Offset from base of column indexing           |
  // |    6.  Stride to be used in both dimensions          |
  // |                                                      |
  // |   All can be set from the command line.              |
  // +======================================================+

  // -------------
  // Problem Size:
  // -------------

  config const n = 59;

  config const factor_block_size           = 6;

  config const solve_block_size            = 2;

  config const schur_complement_block_size = 8;

  config const index_base = -3;

  // --------------
  // output control
  // --------------

  config const print_matrix_details = false;

  config const reproducible_output = false;
}