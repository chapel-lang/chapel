// ChapelNumLocales.chpl
//
module ChapelNumLocales {
  private use ChapelBase;

  extern proc chpl_comm_default_num_locales(): int;

  extern proc printf(x...);
  printf("In personal NumLocales module\n");
  
  //
  // the number of locales on which to run the program
  //
  config const numLocales = chpl_comm_default_num_locales();
}
