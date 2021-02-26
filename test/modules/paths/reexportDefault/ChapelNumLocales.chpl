module ChapelNumLocales {
  private use ChapelBase;

  import Other.Inner; // This is a nested package module

  var whatever = Inner.innerX; // Can we access its symbols?

  extern proc chpl_comm_default_num_locales(): int;
  config const numLocales = chpl_comm_default_num_locales();
}
