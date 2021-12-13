proc main()
{
  var xs : [0 ..# 3] int = [1, 2, 3];
  coforall loc in Locales with (const in xs) do on loc {
    writeln(xs);
  }
}
