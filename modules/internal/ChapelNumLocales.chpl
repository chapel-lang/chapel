// ChapelNumLocales.chpl
//
pragma "no use ChapelStandard"
module ChapelNumLocales {
  
  extern proc chpl_comm_default_num_locales(): int;
  
  //
  // the number of locales on which to run the program
  //
  config const numLocales = chpl_comm_default_num_locales();
  
}
