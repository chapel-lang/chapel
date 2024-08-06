proc positive(z : string) {
  const em = '';
  const sq = '~';
  const sp = ' ';
  const pl = '+';
  const vr = z;
  return if vr == sq then sp else (if vr == pl then vr else em);
}

const s = positive('+');
writeln(s);
