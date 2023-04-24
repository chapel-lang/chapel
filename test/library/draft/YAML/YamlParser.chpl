use IO, ChplConfig, CTypes;
use YamlClassHierarchy;
require "yaml.h", "-lyaml";

extern record yaml_parser_t {
  var offset: c_size_t;
  var mark: yaml_mark_t;
};
extern record yaml_event_t {
  extern "type" var t: c_int;
  var data: opaque;
  var start_mark: yaml_mark_t;
  var end_mark: yaml_mark_t;
};
extern record yaml_mark_t {
  extern "index" var idx: c_size_t;
  var line: c_size_t;
  var column: c_size_t;
};

// extern record yaml_char_t {};
// extern record yaml_node_type_t {};
// extern "struct yaml_node_s" record yaml_node {
//   extern "type" var t: yaml_node_type_t;
//   var tag: yaml_char_t;

//   var data: opaque;

//   var start_mark: yaml_mark_t;
//   var end_mark: yaml_mark_t;
// };

extern const E_NO_EVENT: c_int;
extern const E_STREAM_START: c_int;
extern const E_STREAM_END: c_int;
extern const E_DOCUMENT_START: c_int;
extern const E_DOCUMENT_END: c_int;
extern const E_ALIAS: c_int;
extern const E_SCALAR: c_int;
extern const E_SEQUENCE_START: c_int;
extern const E_SEQUENCE_END: c_int;
extern const E_MAPPING_START: c_int;
extern const E_MAPPING_END: c_int;

private extern proc yaml_parser_initialize(parser: c_ptr(yaml_parser_t)): c_int;
private extern proc yaml_parser_delete(parser: c_ptr(yaml_parser_t)): c_int;
private extern proc yaml_parser_set_input_file(parser: c_ptr(yaml_parser_t), file: c_FILE): c_int;
private extern proc yaml_parser_parse(parser: c_ptr(yaml_parser_t), event: c_ptr(yaml_event_t)): c_int;
private extern proc yaml_event_delete(event: c_ptr(yaml_event_t)): c_int;

private extern proc fopen(filename: c_string, mode: c_string): c_FILE;
private extern proc fclose(file: c_FILE): c_int;


proc parseYamlFile(filePath: string): YamlValue throws {
  var file = fopen(filePath.c_str(), "r".c_str()),
      fr = openReader(filePath, locking=false);

  var parser: yaml_parser_t;
  c_memset(c_ptrTo(parser):c_void_ptr, 0, c_sizeof(yaml_parser_t));

  if !yaml_parser_initialize(c_ptrTo(parser)) then
    throw new Error("Failed to initialize parser");
  yaml_parser_set_input_file(c_ptrTo(parser), file);

  var yvs = parseUntilNextCloser(c_ptrTo(parser), fr);

  yaml_parser_delete(c_ptrTo(parser));
  fclose(file);

  return yvs[0];
}

iter parseUntilNextCloser(parser: c_ptr(yaml_parser_t), reader: fileReader): owned YamlValue throws {
  var event: yaml_event_t;
  c_memset(c_ptrTo(event):c_void_ptr, 0, c_sizeof(yaml_event_t));

  if !yaml_parser_parse(parser, c_ptrTo(event)) then
      throw new Error("Failed to parse next YAML event");

  while true {
    select event.t {
      when E_STREAM_START {
        writeln("STREAM START");
        yield parseUntilNextCloser(parser, reader);
      }
      when E_STREAM_END {
        writeln("STREAM END");
        yaml_event_delete(c_ptrTo(event));
        return;
      }
      when E_DOCUMENT_START {
        writeln("DOCUMENT START");
        yield parseUntilNextCloser(parser, reader);
      }
      when E_DOCUMENT_END {
        writeln("DOCUMENT END");
        return;
      }
      when E_ALIAS {
        writeln("ALIAS");
        yaml_event_delete(c_ptrTo(event));
        throw new Error("Aliases not supported yet");
      }
      when E_SCALAR {
        writeln("SCALAR");
        var ys = new YamlScalar();
        reader.seek((event.start_mark.idx:int)..); // TODO: should the upper bound be provided here?
        const s = reader.readString((event.end_mark.idx - event.start_mark.idx):int);
        ys.value = s;

        yield ys;
      }
      when E_SEQUENCE_START {
        writeln("SEQUENCE START");
        var ys = new YamlSequence();
        for e in parseUntilNextCloser(parser, reader) {
          ys.append(e);
        }
        yield ys;
      }
      when E_SEQUENCE_END {
        writeln("SEQUENCE END");
        yaml_event_delete(c_ptrTo(event));
        return;
      }
      when E_MAPPING_START {
        writeln("MAPPING START");
        var ym = new YamlMapping(),
            nextKey: owned YamlValue,
            key = true;
        for e in parseUntilNextCloser(parser, reader) {
          if key {
            nextKey = e;
            key = false;
          } else {
            ym.add(nextKey, e);
            key = true;
          }
        }
        yield ym;
      }
      when E_MAPPING_END {
        writeln("MAPPING END");
        yaml_event_delete(c_ptrTo(event));
        return;
      }
      otherwise {
        writeln("OTHER");
        yaml_event_delete(c_ptrTo(event));
        throw new Error("Unexpected YAML event");
      }
    }
    yaml_event_delete(c_ptrTo(event));
  }
}
