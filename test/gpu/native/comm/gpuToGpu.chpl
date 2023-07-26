on Locales[0].gpus[0] {
  var A: [1..10] int;

  on Locales[1].gpus[0] {
    var B: [1..10] int = 3;

    A = B;
  }

  writeln(A);
}
