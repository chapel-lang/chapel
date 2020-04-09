module execution_config_consts
{
  // +======================================================+
  // |  Execution time constants control                    |
  // |                                                      |
  // |    1.  Problem Size                                  |
  // |    2.  Indexing Base                                 |
  // |                                                      |
  // |   All can be set from the command line.              |
  // +======================================================+

  // -------------
  // Problem Size:
  // -------------

  config const n = 499;

  config const index_base = -7;

  // --------------
  // output control
  // --------------

  config const print_matrix_details = false;

  config const reproducible_output = false;
}