use IO;

var strings = ["hello", "찠찠o", "bel=\x07", "\\\n", "snomwan ☃ snowflake ❄ gclef 𝄞  end"];

var st = stdout._styleInternal();
st.min_width_columns = 79;
st.leftjustify = 1;

stdout._set_styleInternal(st);
stdout.writeLiteral("First group contains control characters (bell) and newlines and so is uneven\n");
for s in strings {
  write(s);
  stdout.writeLiteral("|\n");
}

st.min_width_columns = 79;
st.leftjustify = 0;
stdout._set_styleInternal(st);

for s in strings {
  write(s);
  stdout.writeLiteral("|\n");
}

st.string_format = iostringformatInternal.chpl:uint(8);
st.min_width_columns = 79;
st.leftjustify = 1;

stdout._set_styleInternal(st);
stdout.writeLiteral("Second group should be nicely aligned with Chapel-style escapes\n");
for s in strings {
  write(s);
  stdout.writeLiteral("|\n");
}

st.min_width_columns = 79;
st.leftjustify = 0;
stdout._set_styleInternal(st);

for s in strings {
  write(s);
  stdout.writeLiteral("|\n");
}

st.string_format = iostringformatInternal.json:uint(8);
st.min_width_columns = 79;
st.leftjustify = 1;

stdout._set_styleInternal(st);
stdout.writeLiteral("Third group should be nicely aligned with JSON-style escapes\n");
for s in strings {
  write(s);
  stdout.writeLiteral("|\n");
}

st.min_width_columns = 79;
st.leftjustify = 0;
stdout._set_styleInternal(st);

for s in strings {
  write(s);
  stdout.writeLiteral("|\n");
}
