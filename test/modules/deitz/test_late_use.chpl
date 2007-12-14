module M1 {
  var x: int = 4;
}

module M2 {
  def main {
    writeln(x);
    use M1;
  }
}
