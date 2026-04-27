module Included {
  writeln("Included module");
  include module Sub;
  public use Sub;
}
