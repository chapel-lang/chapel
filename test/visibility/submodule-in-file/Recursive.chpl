// This (and SubModule) were created as a reproducer for issue #25569

module Recursive {
  config param EXTRA_CHECKS = 42;

  include public module SubModule;

  use this.SubModule;

  writeln("in Recursive, EXTRA_CHECKS=", EXTRA_CHECKS);

  foo();
}
