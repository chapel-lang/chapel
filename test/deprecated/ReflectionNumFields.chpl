use Reflection;

record rec { var x = 0; }

proc main() {
  param n = numFields(rec);
}
