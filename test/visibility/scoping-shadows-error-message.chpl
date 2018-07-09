proc main() {
  var a = 12;
  {
    var b = a;
  }
  {
    var a = 1;
    var b = a;
  }
  {
    var b = a; // <-- error reported here.
    var a = 1;
  }
  {
    var b = a;
    {
      var a = 1;
    }
  }
}
