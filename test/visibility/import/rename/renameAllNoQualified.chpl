module M {
  var x: int;
  var y = false;

  proc z() {
    writeln("In M.z()");
  }
}

module User {
  import M.{x as a, y as b, z as c};

  proc main() {
    writeln(M.a); // Should fail, `import M.{}` does not enable qualified access
    // This version uses the new name
  }
}
