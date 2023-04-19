use IO, ChplConfig, CTypes;
use YamlClassHierarchy;

require CHPL_HOME + "/third-party/libyaml/include/yaml.h";

extern record yaml_node_type_t {};
extern record yaml_event_type_t {};
extern record yaml_parser_t {};
extern record yaml_event_t {
  extern "type" var t: yaml_event_type_t;
  var data: opaque;
  var start_mark: yaml_mark_t;
  var end_mark: yaml_mark_t;
};
extern record yaml_char_t {};
extern record yaml_mark_t {};

extern "struct yaml_node_s" record yaml_node {
  extern "type" var t: yaml_node_type_t;
  var tag: yaml_char_t;

  var data: opaque;

  var start_mark: yaml_mark_t;
  var end_mark: yaml_mark_t;
}

extern const YAML_NO_EVENT: yaml_event_type_t;
extern const YAML_STREAM_START_EVENT: yaml_event_type_t;
extern const YAML_STREAM_END_EVENT: yaml_event_type_t;
extern const YAML_DOCUMENT_START_EVENT: yaml_event_type_t;
extern const YAML_DOCUMENT_END_EVENT: yaml_event_type_t;
extern const YAML_ALIAS_EVENT: yaml_event_type_t;
extern const YAML_SCALAR_EVENT: yaml_event_type_t;
extern const YAML_SEQUENCE_START_EVENT: yaml_event_type_t;
extern const YAML_SEQUENCE_END_EVENT: yaml_event_type_t;
extern const YAML_MAPPING_START_EVENT: yaml_event_type_t;
extern const YAML_MAPPING_END_EVENT: yaml_event_type_t;

private extern proc yaml_parser_initialize(parser: c_ptr(yaml_parser_t)): c_int;
private extern proc yaml_parser_delete(parser: c_ptr(yaml_parser_t)): c_int;
private extern proc yaml_parser_set_input_file(parser: c_ptr(yaml_parser_t), file: c_FILE): c_int;
private extern proc yaml_parser_parse(parser: c_ptr(yaml_parser_t), event: c_ptr(yaml_event_t)): c_int;
private extern proc yaml_event_delete(event: c_ptr(yaml_event_t)): c_int;

private extern proc fopen(filename: c_string, mode: c_string): c_FILE;
private extern proc fclose(file: c_FILE): c_int;

proc _parseYaml(filePath: string) throws {
  var file = fopen(filePath.c_str(), "r".c_str());

  var parser: yaml_parser_t,
      event: yaml_event_t,
      done: c_int = 0;

  if !yaml_parser_initialize(c_ptrTo(parser)) then
    throw new Error("Failed to initialize parser");
  yaml_parser_set_input_file(c_ptrTo(parser), file);

  while !done {
    if !yaml_parser_parse(c_ptrTo(parser), c_ptrTo(event)) then
      throw new Error("Failed to parse YAML");

    done = event.t == YAML_STREAM_END_EVENT;
    yaml_event_delete(c_ptrTo(event));
  }

  yaml_parser_delete(c_ptrTo(parser));
  fclose(file);
}

_parseYaml("./test.yaml");
