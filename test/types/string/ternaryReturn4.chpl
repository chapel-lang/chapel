proc positive(z : string) {
  const ref em = '';
  const ref sq = '~';
  const ref sp = ' ';
  const ref pl = '+';
  const ref vr = z;
  return if vr == sq then sp else (if vr == pl then vr else em);
}

const s = positive('+');
writeln(s);
