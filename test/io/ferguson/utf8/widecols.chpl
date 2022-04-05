use IO;

var strings = ["hello", "찠찠o", "bel=\x07", "\\\n", "snomwan ☃ snowflake ❄ gclef 𝄞  end"];

var st = stdout._styleInternal();
st.min_width_columns = 79;
st.leftjustify = 1;

stdout._set_styleInternal(st);
writeln(new ioLiteral("First group contains control characters (bell) and newlines and so is uneven"));
for s in strings {
  write(s);
  writeln(new ioLiteral("|"));
}

st.min_width_columns = 79;
st.leftjustify = 0;
stdout._set_styleInternal(st);

for s in strings {
  write(s);
  writeln(new ioLiteral("|"));
}

st.string_format = iostringformat.chpl:uint(8);
st.min_width_columns = 79;
st.leftjustify = 1;

stdout._set_styleInternal(st);
writeln(new ioLiteral("Second group should be nicely aligned with Chapel-style escapes"));
for s in strings {
  write(s);
  writeln(new ioLiteral("|"));
}

st.min_width_columns = 79;
st.leftjustify = 0;
stdout._set_styleInternal(st);

for s in strings {
  write(s);
  writeln(new ioLiteral("|"));
}

st.string_format = iostringformat.json:uint(8);
st.min_width_columns = 79;
st.leftjustify = 1;

stdout._set_styleInternal(st);
writeln(new ioLiteral("Third group should be nicely aligned with JSON-style escapes"));
for s in strings {
  write(s);
  writeln(new ioLiteral("|"));
}

st.min_width_columns = 79;
st.leftjustify = 0;
stdout._set_styleInternal(st);

for s in strings {
  write(s);
  writeln(new ioLiteral("|"));
}
