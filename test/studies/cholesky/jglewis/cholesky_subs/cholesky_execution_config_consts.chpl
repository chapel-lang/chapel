module cholesky_execution_config_consts
{
  // +======================================================+
  // |  Execution time constants control                    |
  // |                                                      |
  // |    1.  Problem Size                                  |
  // |    2.  Block Size                                    |
  // |    3.  Indexing Base                                 |
  // |                                                      |
  // |   All can be set from the command line.              |
  // +======================================================+

  // -------------
  // Problem Size:
  // -------------

  config const n = 59;

  config const block_size = 13;

  config const index_base = -7;

}