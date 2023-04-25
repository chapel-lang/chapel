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

proc parseYamlFile(filePath: string): [] owned YamlValue throws {
  var file = fopen(filePath.c_str(), "r".c_str()),
      fr = openReader(filePath, locking=false);

  var parser: yaml_parser_t;
  c_memset(c_ptrTo(parser):c_void_ptr, 0, c_sizeof(yaml_parser_t));

  if !yaml_parser_initialize(c_ptrTo(parser)) then
    throw new Error("Failed to initialize parser");
  yaml_parser_set_input_file(c_ptrTo(parser), file);

  var yvs = parseUntilEvent(E_STREAM_END, parser, fr);

  yaml_parser_delete(c_ptrTo(parser));
  fclose(file);

  return yvs;
}

iter parseUntilEvent(e_stop: c_int, ref parser: yaml_parser_t, reader: fileReader): owned YamlValue {
  var event: yaml_event_t;
  c_memset(c_ptrTo(event):c_void_ptr, 0, c_sizeof(yaml_event_t));

  inline proc finish() {
    yaml_event_delete(c_ptrTo(event));
    return;
  }

  while true {
    // parse until the next event
    if !yaml_parser_parse(c_ptrTo(parser), c_ptrTo(event)) then
        halt("Failed to parse next YAML event");

    // handle event
    select event.t {
      when E_STREAM_START {
        for e in parseUntilEvent(E_STREAM_END, parser, reader) {
          yield e;
        }
      }
      when E_STREAM_END {
        if e_stop != E_STREAM_END then
          writeln("wrong closing event. Expected ", e_stop", got E_STREAM_END");
        finish();
      }
      when E_DOCUMENT_START {
        for e in parseUntilEvent(E_DOCUMENT_END, parser, reader) {
          yield e;
        }
      }
      when E_DOCUMENT_END {
        if e_stop != E_DOCUMENT_END then
          writeln("wrong closing event. Expected ", e_stop", got E_DOCUMENT_END");
        finish();
      }
      when E_ALIAS {
        reader.seek((event.start_mark.idx:int)..);
        yield new YamlAlias(reader.readString((event.end_mark.idx - event.start_mark.idx):int));
      }
      when E_SCALAR {
        reader.seek((event.start_mark.idx:int)..);
        yield new YamlScalar(reader.readString((event.end_mark.idx - event.start_mark.idx):int));
      }
      when E_SEQUENCE_START {
        var seq = new YamlSequence();
        for e in parseUntilEvent(E_SEQUENCE_END, parser, reader) {
          seq._append(e);
        }
        yield seq;
      }
      when E_SEQUENCE_END {
        if e_stop != E_SEQUENCE_END then
          writeln("wrong closing event. Expected ", e_stop", got E_SEQUENCE_END");
        finish();
      }
      when E_MAPPING_START {
        var mapping = new YamlMapping(),
            nextKey = new owned YamlValue(),
            key = true;

        for e in parseUntilEvent(E_MAPPING_END, parser, reader) {
          // TODO: is there a better way to do this without using unmanaged?
          // should this pattern be supported for owned?
          var cpy: unmanaged YamlValue = owned.release(e);
          if key {
            nextKey = owned.adopt(cpy);
            key = false;
          } else {
            mapping._add(nextKey, owned.adopt(cpy));
            key = true;
          }
        }
        yield mapping;
      }
      when E_MAPPING_END {
        if e_stop != E_MAPPING_END then
          writeln("wrong closing event. Expected ", e_stop", got E_MAPPING_END");
        finish();
      }
      when E_NO_EVENT {
        finish();
      }
      otherwise {
        yaml_event_delete(c_ptrTo(event));
        writeln("Unexpected YAML event");
        finish();
      }
    }
    yaml_event_delete(c_ptrTo(event));
  }
}
