class C {
  def chpl_destroy() {
    writeln("chpl_destroy");
  }
}
def main {
  var c = new C();
  delete c;
}
