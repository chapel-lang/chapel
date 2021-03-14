proc f(x) {
    if x == 0 {
        var y : [1..10] int;
        return y;
    }

    return 5;
}

proc main() {
  var x = f(10);
}
