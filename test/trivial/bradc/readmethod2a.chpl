class G {
  function readit(): integer {
    return 1;
  }
}

function main() {
  var goo = G();
  var gooResult = goo.readit();
  writeln("gooResult = ", gooResult);
}
