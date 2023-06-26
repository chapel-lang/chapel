use IO, YAML;

var FormatWriter = new YamlSerializer();
var FormatReader = new YamlDeserializer();

proc printDebugFmt(val) throws {
    writeln("===== writing: =====");
    stdout.writeln(val);
    writeln("--------------------");
    try {
    stdout.withSerializer(FormatWriter).writeln(val);
    } catch e {
    stdout.writeln("ERROR: ", e);
    }
    writeln("====================");
  }
