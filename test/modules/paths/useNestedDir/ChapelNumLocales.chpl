module ChapelNumLocales {
  private use ChapelBase;

  import TOML.TomlParser; // This is a nested package module

  var whatever = TomlParser.debugTomlParser; // Can we access its symbols?

  extern proc chpl_comm_default_num_locales(): int;
  config const numLocales = chpl_comm_default_num_locales();
}
