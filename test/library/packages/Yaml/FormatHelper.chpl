use IO, YAML;

var FormatWriter = new yamlSerializer();
var FormatReader = new yamlDeserializer();

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
