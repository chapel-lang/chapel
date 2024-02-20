
use IO;

proc main() {
  var obj = """
  {
    "x": 5,
    "y": 42.0,
    "skipMe":false,
    "z": "hello"
  }
  """;

  var f = openMemFile();
  f.writer(locking=false).write(obj);

  var r = f.reader(locking=false);
  var st = r._styleInternal();
  var orig = st; defer { r._set_styleInternal(orig); }
  st.realfmt = 2;
  st.string_format = iostringformatInternal.json:uint(8);
  st.aggregate_style = QIO_AGGREGATE_FORMAT_JSON:uint(8);
  st.array_style = QIO_ARRAY_FORMAT_JSON:uint(8);
  st.tuple_style = QIO_TUPLE_FORMAT_JSON:uint(8);
  r._set_styleInternal(st);

  r.readLiteral("{");

  r.readLiteral("\"x\":");
  assert(r.read(int) == 5);
  r.readLiteral(",");

  r.readLiteral("\"y\":");
  assert(r.read(real) == 42.0);
  r.readLiteral(",");

  r.skipField();
  r.readLiteral(",");

  r.readLiteral("\"z\":");
  assert(r.read(string) == "hello");

  r.readLiteral("}");
}
