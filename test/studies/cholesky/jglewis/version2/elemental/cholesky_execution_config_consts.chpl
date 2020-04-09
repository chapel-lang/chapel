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

  config const block_size = 7;

  config const index_base = -3;

  config const row_offset = 13;

  config const col_offset = 8;

  config const stride = 5;

  // --------------
  // output control
  // --------------

  config const print_matrix_details = false;

  config const reproducible_output = false;
}