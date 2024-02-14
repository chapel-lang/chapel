use YAML, List, IO;

record R1 {
  var item: R2;
  var id: int;
  var info: string;
}

record R2 {
  var stuff: list(int);
  var data: string;
}

var sout = new file(1);

var a = sout.writer(
      serializer = new yamlSerializer(
        YamlSequenceStyle.Flow,
        YamlMappingStyle.Block,
        YamlScalarStyle.SingleQuoted,
        YamlDocumentStyle.Explicit
        )
      );

var b = sout.writer(
      serializer = new yamlSerializer(
        YamlSequenceStyle.Block,
        YamlMappingStyle.Flow,
        YamlScalarStyle.Plain,
        YamlDocumentStyle.Implicit
        )
      );

var c = sout.writer(
      serializer = new yamlSerializer(
        YamlSequenceStyle.Flow,
        YamlMappingStyle.Flow,
        YamlScalarStyle.Folded,
        YamlDocumentStyle.Explicit
        )
      );

var d = sout.writer(serializer = new yamlSerializer(), locking=false);

var defaultFormat = sout.writer(locking=false);

const longString = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. In in nisl lobortis, rhoncus \
  metus dapibus, molestie mi. Donec porta lacus vitae dignissim posuere. In non auctor lorem. Nullam nec dui \
  at enim pharetra efficitur vitae eget risus. Praesent sollicitudin justo tortor, vel lobortis ex vehicula";

const r1 = new R1(new R2(new list(1..5), "hello"), 111, longString);

defaultFormat.writeln("=== Flow, Block, Single, Explicit ===\n");
defaultFormat.flush();
a.write(r1);
a.flush();

defaultFormat.writeln("\n=== Block, Flow, Plain, Implicit ===\n");
defaultFormat.flush();
b.write(r1);
b.flush();

defaultFormat.writeln("\n=== Flow, Flow, Folded, Explicit ===\n");
defaultFormat.flush();
c.write(r1);
c.flush();

defaultFormat.writeln("\n=== default ===\n");
defaultFormat.flush();
d.write(r1);
d.flush();
