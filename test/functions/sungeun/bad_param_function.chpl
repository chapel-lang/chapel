proc foo() param {
  writeln("hi");
  return true;
}

if foo() then writeln("bye");
