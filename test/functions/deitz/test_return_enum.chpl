enum E { A };

proc f() param {
  return E.A;
}

writeln(f());
