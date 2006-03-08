class G {
  function read(): integer {
    return 1;
  }
}

function main() {
  var goo = G();
  var gooResult = goo.read();
  writeln("gooResult = ", gooResult);
}
