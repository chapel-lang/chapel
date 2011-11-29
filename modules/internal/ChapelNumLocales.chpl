// ChapelNumLocales.chpl
//
pragma "no use ChapelStandard"
module ChapelNumLocales {

// the number of locales on which to run the program
config const numLocales: int = __primitive("chpl_comm_default_num_locales");

//proc chpl_numLocales(r) {
//  assert(r == 0);
//  return numLocales;
//}

}
