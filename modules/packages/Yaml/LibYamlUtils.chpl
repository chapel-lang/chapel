module LibYamlUtils {
  private use CTypes;
  require "yaml.h", "-lyaml";

  import super.YamlSequenceStyle,
         super.YamlMappingStyle,
         super.YamlScalarStyle,
         super.YamlDocumentStyle;

  import super.YamlEmitterError,
         super.YamlParserError,
         super.YamlUnexpectedEventError;

  // ----------------------------------------------------
  // Event Types
  // ----------------------------------------------------

  extern const YAML_NO_EVENT: c_int;
  extern const YAML_STREAM_START_EVENT: c_int;
  extern const YAML_STREAM_END_EVENT: c_int;
  extern const YAML_DOCUMENT_START_EVENT: c_int;
  extern const YAML_DOCUMENT_END_EVENT: c_int;
  extern const YAML_ALIAS_EVENT: c_int;
  extern const YAML_SCALAR_EVENT: c_int;
  extern const YAML_SEQUENCE_START_EVENT: c_int;
  extern const YAML_SEQUENCE_END_EVENT: c_int;
  extern const YAML_MAPPING_START_EVENT: c_int;
  extern const YAML_MAPPING_END_EVENT: c_int;

  enum EventType {
    None,           // YAML_NO_EVENT
    StreamStart,    // YAML_STREAM_START_EVENT
    StreamEnd,      // YAML_STREAM_END_EVENT
    DocumentStart,  // YAML_DOCUMENT_START_EVENT
    DocumentEnd,    // YAML_DOCUMENT_END_EVENT
    Alias,          // YAML_ALIAS_EVENT
    Scalar,         // YAML_SCALAR_EVENT
    SequenceStart,  // YAML_SEQUENCE_START_EVENT
    SequenceEnd,    // YAML_SEQUENCE_END_EVENT
    MappingStart,   // YAML_MAPPING_START_EVENT
    MappingEnd      // YAML_MAPPING_END_EVENT
  }

  inline proc getEvent(flag: c_int): EventType {
    select flag {
      when YAML_NO_EVENT do return EventType.None;
      when YAML_STREAM_START_EVENT do return EventType.StreamStart;
      when YAML_STREAM_END_EVENT do return EventType.StreamEnd;
      when YAML_DOCUMENT_START_EVENT do return EventType.DocumentStart;
      when YAML_DOCUMENT_END_EVENT do return EventType.DocumentEnd;
      when YAML_ALIAS_EVENT do return EventType.Alias;
      when YAML_SCALAR_EVENT do return EventType.Scalar;
      when YAML_SEQUENCE_START_EVENT do return EventType.SequenceStart;
      when YAML_SEQUENCE_END_EVENT do return EventType.SequenceEnd;
      when YAML_MAPPING_START_EVENT do return EventType.MappingStart;
      when YAML_MAPPING_END_EVENT do return EventType.MappingEnd;
      otherwise halt("unknown libyaml event type");
    }
  }

  operator :(e: EventType, type t:string) {
    select e {
      when EventType.None do return "NO EVENT";
      when EventType.StreamStart do return "STREAM START";
      when EventType.StreamEnd do return "STREAM END";
      when EventType.DocumentStart do return "DOCUMENT START";
      when EventType.DocumentEnd do return "DOCUMENT END";
      when EventType.Alias do return "ALIAS";
      when EventType.Scalar do return "SCALAR";
      when EventType.SequenceStart do return "SEQUENCE START";
      when EventType.SequenceEnd do return "SEQUENCE END";
      when EventType.MappingStart do return "MAPPING START";
      when EventType.MappingEnd do return "MAPPING END";
    }
  }

  // ----------------------------------------------------
  // structures
  // ----------------------------------------------------

  extern record yaml_parser_t { }
  extern record yaml_emitter_t { }
  extern record yaml_event_t {
    extern "type" var t: c_int;   // event type
    var data: opaque;
    var start_mark: yaml_mark_t;  // start location in file
    var end_mark: yaml_mark_t;    // end location in file
  }
  extern record yaml_mark_t {
    extern "index" var idx: c_size_t; // byte index
    var line: c_size_t;
    var column: c_size_t;
  }
  extern record yaml_version_directive_t {
    var major: c_int;
    var minor: c_int;
  }
  extern record yaml_tag_directive_t {
    var handle: c_string;
    var prefix: c_string;
  }

  // ----------------------------------------------------
  // emitter API
  // ----------------------------------------------------

  private extern proc yaml_emitter_initialize(emitter: c_ptr(yaml_emitter_t)): c_int;
  private extern proc yaml_emitter_set_output_file(emitter: c_ptr(yaml_emitter_t), file: c_FILE): c_int;
  private extern proc yaml_emitter_set_canonical(emitter: c_ptr(yaml_emitter_t), isC: c_int);
  private extern proc yaml_emitter_set_unicode(emitter: c_ptr(yaml_emitter_t), isU: c_int);
  private extern proc yaml_emitter_emit(emitter: c_ptr(yaml_emitter_t), event: c_ptr(yaml_event_t)): c_int;
  private extern proc yaml_emitter_delete(emitter: c_ptr(yaml_emitter_t));
  private extern proc yaml_emitter_flush(emitter: c_ptr(yaml_emitter_t)): c_int;

  // ----------------------------------------------------
  // parser API
  // ----------------------------------------------------

  private extern proc yaml_parser_initialize(parser: c_ptr(yaml_parser_t)): c_int;
  private extern proc yaml_parser_set_input_file(parser: c_ptr(yaml_parser_t), file: c_FILE): c_int;
  private extern proc yaml_parser_parse(parser: c_ptr(yaml_parser_t), event: c_ptr(yaml_event_t)): c_int;
  private extern proc yaml_parser_delete(parser: c_ptr(yaml_parser_t)): c_int;

  // ----------------------------------------------------
  // event API
  // ----------------------------------------------------

  private extern proc yaml_event_delete(event: c_ptr(yaml_event_t));

  private extern proc yaml_stream_start_event_initialize(event: c_ptr(yaml_event_t), encoding: c_int): c_int;
  private extern proc yaml_stream_end_event_initialize(event: c_ptr(yaml_event_t)): c_int;

  private extern proc yaml_document_start_event_initialize(
                      event: c_ptr(yaml_event_t),
                      version: c_ptr(yaml_version_directive_t),
                      start: c_ptr(yaml_tag_directive_t),
                      end: c_ptr(yaml_tag_directive_t),
                      implicit: c_int
                    ): c_int;
  private extern proc yaml_document_end_event_initialize(event: c_ptr(yaml_event_t), implicit: c_int): c_int;

  private extern proc yaml_sequence_start_event_initialize(
                        event: c_ptr(yaml_event_t),
                        anchor: c_ptr(c_uchar),
                        tag: c_ptr(c_uchar),
                        implicit: c_int,
                        style: c_int
                      ): c_int;
  private extern proc yaml_sequence_end_event_initialize(event: c_ptr(yaml_event_t)): c_int;

  private extern proc yaml_mapping_start_event_initialize(
                        event: c_ptr(yaml_event_t),
                        anchor: c_ptr(c_uchar),
                        tag: c_ptr(c_uchar),
                        implicit: c_int,
                        style: c_int
                      ): c_int;
  private extern proc yaml_mapping_end_event_initialize(event: c_ptr(yaml_event_t)): c_int;

  private extern proc yaml_scalar_event_initialize(
                        event: c_ptr(yaml_event_t),
                        anchor: c_ptr(c_uchar),
                        tag: c_ptr(c_uchar),
                        value: c_ptr(c_uchar),
                        length: c_int,
                        plain_implicit: c_int,
                        quoted_implicit: c_int,
                        style: c_int
                      ): c_int;
  private extern proc yaml_alias_event_initialize(event: c_ptr(yaml_event_t), anchor: c_ptr(c_uchar)): c_int;

  // ----------------------------------------------------
  // encodings and styles
  // ----------------------------------------------------

  extern const YAML_ANY_ENCODING: c_int;
  extern const YAML_UTF8_ENCODING: c_int;
  extern const YAML_UTF16LE_ENCODING: c_int;
  extern const YAML_UTF16BE_ENCODING: c_int;

  extern const YAML_ANY_SEQUENCE_STYLE: c_int;
  extern const YAML_BLOCK_SEQUENCE_STYLE: c_int;
  extern const YAML_FLOW_SEQUENCE_STYLE: c_int;

  extern const YAML_ANY_MAPPING_STYLE: c_int;
  extern const YAML_BLOCK_MAPPING_STYLE: c_int;
  extern const YAML_FLOW_MAPPING_STYLE: c_int;

  extern const YAML_ANY_SCALAR_STYLE: c_int;
  extern const YAML_PLAIN_SCALAR_STYLE: c_int;
  extern const YAML_SINGLE_QUOTED_SCALAR_STYLE: c_int;
  extern const YAML_DOUBLE_QUOTED_SCALAR_STYLE: c_int;
  extern const YAML_LITERAL_SCALAR_STYLE: c_int;
  extern const YAML_FOLDED_SCALAR_STYLE: c_int;

  inline proc seqStyleToFlag(style: YamlSequenceStyle): c_int {
    select style {
      when YamlSequenceStyle.Any do return YAML_ANY_SEQUENCE_STYLE;
      when YamlSequenceStyle.Block do return YAML_BLOCK_SEQUENCE_STYLE;
      when YamlSequenceStyle.Flow do return YAML_FLOW_SEQUENCE_STYLE;
    }
    return YAML_ANY_SEQUENCE_STYLE;
  }

  inline proc mapStyleToFlag(style: YamlMappingStyle): c_int {
    select style {
      when YamlMappingStyle.Any do return YAML_ANY_MAPPING_STYLE;
      when YamlMappingStyle.Block do return YAML_BLOCK_MAPPING_STYLE;
      when YamlMappingStyle.Flow do return YAML_FLOW_MAPPING_STYLE;
    }
    return YAML_ANY_MAPPING_STYLE;
  }

  inline proc scalarStyleToFlag(style: YamlScalarStyle): c_int {
    select style {
      when YamlScalarStyle.Any do return YAML_ANY_SCALAR_STYLE;
      when YamlScalarStyle.Plain do return YAML_PLAIN_SCALAR_STYLE;
      when YamlScalarStyle.SingleQuoted do return YAML_SINGLE_QUOTED_SCALAR_STYLE;
      when YamlScalarStyle.DoubleQuoted do return YAML_DOUBLE_QUOTED_SCALAR_STYLE;
      when YamlScalarStyle.Literal do return YAML_LITERAL_SCALAR_STYLE;
      when YamlScalarStyle.Folded do return YAML_FOLDED_SCALAR_STYLE;
    }
    return YAML_ANY_SCALAR_STYLE;
  }

  // ----------------------------------------------------
  // Emitter wrapper
  // ----------------------------------------------------

  class LibYamlEmitter {
    var seqStyle: YamlSequenceStyle;
    var mapStyle: YamlMappingStyle;
    var scalarStyle: YamlScalarStyle;
    var documentStyle: YamlDocumentStyle;

    var emitter: yaml_emitter_t;
    var event: yaml_event_t;

    var file: c_FILE;
  }

  // -------- initialization --------

  proc LibYamlEmitter.init(
    sequences = YamlSequenceStyle.Any,
    mappings = YamlMappingStyle.Any,
    scalars = YamlScalarStyle.Any,
    documents = YamlDocumentStyle.Any
  ) {
    this.seqStyle = sequences;
    this.mapStyle = mappings;
    this.scalarStyle = scalars;
    this.documentStyle = documents;

    var emitter: yaml_emitter_t;
    var event: yaml_event_t;
    c_memset(c_ptrTo(emitter):c_void_ptr, 0, c_sizeof(yaml_emitter_t));
    c_memset(c_ptrTo(event):c_void_ptr, 0, c_sizeof(yaml_event_t));
    this.emitter = emitter;
    this.event = event;
  }

  // assumes 'closeContext' has already been called
  proc LibYamlEmitter.deinit() {
    yaml_event_delete(c_ptrTo(this.event));
    c_free(c_ptrTo(this.event));
    c_free(c_ptrTo(this.emitter));
  }

  // -------- context management --------

  proc LibYamlEmitter.openContext() throws {
    c_memset(c_ptrTo(emitter):c_void_ptr, 0, c_sizeof(yaml_emitter_t));
    if !yaml_emitter_initialize(c_ptrTo(this.emitter))
      then throw new YamlEmitterError("Failed to initialize emitter");

    this.file = tmpfile();

    yaml_emitter_set_output_file(c_ptrTo(this.emitter), this.file);
    yaml_emitter_set_canonical(c_ptrTo(this.emitter), 0);
    yaml_emitter_set_unicode(c_ptrTo(this.emitter), 1);

    this._startOutputStream();
    this.beginDocument(false);
  }

  proc LibYamlEmitter.closeContext(): bytes throws {
    this.endDocument(false);
    this._endOutputStream();
    yaml_emitter_delete(c_ptrTo(this.emitter));

    fseek(this.file, 0, SEEK_END);
    var size = ftell(this.file);
    fseek(this.file, 0, SEEK_SET);

    var buf = c_malloc(uint(8), size+1);
    fread(buf, 1, size, this.file);
    buf[size] = 0;

    const b = createBytesWithNewBuffer(buf, size, size+1);
    c_free(buf);

    fclose(this.file);
    return b;
  }

  // -------- event emission --------

  proc LibYamlEmitter.emitEvent(param name: string) throws {
    if !yaml_emitter_emit(c_ptrTo(this.emitter), c_ptrTo(this.event)) then
      throw new YamlEmitterError("Failed to emit a libyaml '" + name + "' event.");
  }

  proc LibYamlEmitter.startSequence(ref tag: bytes = b"", styleHint = YamlSequenceStyle.Default) throws {
    if !yaml_sequence_start_event_initialize(
      c_ptrTo(this.event),
      nil, // TODO: anchor support
      if tag.numBytes > 0 then c_ptrTo(tag) else nil,
      (if tag.numBytes > 0 then 0 else 1): c_int,
      this.chooseSeqStyle(styleHint)
    ) then throw new YamlEmitterError("Failed to initialize 'sequence start' event.");

    this.emitEvent("sequence start");
  }

  proc LibYamlEmitter.endSequence() throws {
    if !yaml_sequence_end_event_initialize(c_ptrTo(this.event))
      then throw new YamlEmitterError("Failed to initialize 'sequence end' event.");

    this.emitEvent("sequence end");
  }

  proc LibYamlEmitter.startMapping(ref tag: bytes = b"", styleHint = YamlMappingStyle.Default) throws {
    if !yaml_mapping_start_event_initialize(
      c_ptrTo(this.event),
      nil, // TODO: anchor support
      if tag.numBytes > 0 then c_ptrTo(tag) else nil,
      (if tag.numBytes > 0 then 0 else 1): c_int,
      this.chooseMapStyle(styleHint)
    ) then throw new YamlEmitterError("Failed to initialize 'mapping start' event.");

    this.emitEvent("mapping start");
  }

  proc LibYamlEmitter.endMapping() throws {
    if !yaml_mapping_end_event_initialize(c_ptrTo(this.event))
      then throw new YamlEmitterError("Failed to initialize 'mapping end' event.");

    this.emitEvent("mapping end");
  }

  proc LibYamlEmitter.emitScalar(
    ref value: bytes,
    ref tag: bytes = b"",
    styleHint = YamlScalarStyle.Default
  ) throws {
    if !yaml_scalar_event_initialize(
      c_ptrTo(this.event),
      nil, // TODO: anchor support
      if tag.numBytes > 0 then c_ptrTo(tag) else nil,
      c_ptrTo(value),
      value.numBytes: c_int,
      (if tag.numBytes > 0 then 0 else 1): c_int,
      (if tag.numBytes > 0 then 0 else 1): c_int,
      this.chooseScalarStyle(styleHint)
    ) then throw new YamlEmitterError("Failed to initialize 'scalar' event.");

    this.emitEvent("scalar");
  }

  proc LibYamlEmitter.emitAlias(ref value: bytes) throws {
    if !yaml_alias_event_initialize(c_ptrTo(this.event), c_ptrTo(value))
      then throw new YamlEmitterError("Failed to initialize 'alias' event.");

    this.emitEvent("alias");
  }

  proc LibYamlEmitter.startDocument(borderHint = YamlDocumentStyle.Default) throws {
    if !yaml_document_start_event_initialize(
      c_ptrTo(this.event),
      nil, nil, nil,
      this.chooseDocumentStyle(borderHint)
    ) then throw new YamlEmitterError("Failed to initialize 'document start' event");

    this.emitEvent(errorMsg = "Failed to emit document start event");
  }

  proc LibYamlEmitter.endDocument(borderHint = YamlDocumentStyle.Default) throws {
    if !yaml_document_end_event_initialize(
      c_ptrTo(this.event),
      this.chooseDocumentStyle(borderHint)
    ) then throw new YamlEmitterError("Failed to initialize document end event");

    this.emitEvent(errorMsg = "Failed to emit document end event");
  }

  // -------- helpers --------

  proc LibYamlEmitter.chooseSeqStyle(styleHint: YamlSequenceStyle): c_int {
    if styleHint == YamlSequenceStyle.Default
      then return seqStyleToFlag(this.seqStyle);
      else return seqStyleToFlag(styleHint);
  }

  proc LibYamlEmitter.chooseMapStyle(styleHint: YamlMappingStyle): c_int {
    if styleHint == YamlMappingStyle.Default
      then return mapStyleToFlag(this.mapStyle);
      else return mapStyleToFlag(styleHint);
  }

  proc LibYamlEmitter.chooseScalarStyle(styleHint: YamlScalarStyle): c_int {
    if styleHint == YamlScalarStyle.Default
      then return scalarStyleToFlag(this.scalarStyle);
      else return scalarStyleToFlag(styleHint);
  }

  proc LibYamlEmitter.chooseDocumentStyle(hint: YamlDocumentStyle): c_int {
    if hint == YamlDocumentStyle.Default {
      select this.documentStyle {
        when YamlDocumentStyle.Explicit do return 1;
        when YamlDocumentStyle.Implicit do return 0;
      }
    } else {
      select hint {
        when YamlDocumentStyle.Explicit do return 1;
        when YamlDocumentStyle.Implicit do return 0;
      }
    }
    return 1;
  }

  // used to serialize simple types outside the context of a sequence or mapping
  proc LibYamlEmitter.yamlStringifySimple(val: ?t, processStrings:bool): bytes {
    if isBoolType(t) {
      return if t then b"Yes" else b"No";
    } else if t == string || t == bytes {
      if processStrings {
        select this.scalarStyle {
          when YamlScalarStyle.Plain do return t: bytes;
          when YamlScalarStyle.SingleQuoted do return b"'" + val: bytes + b"'";
          when YamlScalarStyle.DoubleQuoted do return b"\"" + val: bytes + b"\"";
          when YamlScalarStyle.Literal do return b"|" & val: bytes;
          when YamlScalarStyle.Folded {
            var left = val.size,
                lineStart = 0,
                folded = b"> ";
            while left > 0 {
              const next = val[lineStart..min(lineStart + 80, val.size)]: bytes + b"\n";
              left -= next.size;
              folded += next;
            }
            return folded;
          }
          otherwise return t: bytes;
        }
      } else {
        return t: bytes;
      }
    } else if val == nil || isNothingType(t) {
      return b"~";
    } else {
      return "%t".format(val):bytes;
    }
  }

  // ----------------------------------------------------
  // Parser Wrapper
  // ----------------------------------------------------

  class LibYamlParser {
    var parser: yaml_parser_t;
    var event: yaml_event_t;

    var fileAttached = false;
    var f: c_FILE;
  }

  // -------- initialization --------
  proc LibYamlParser.init() {
    var p: yaml_parser_t;
    var e: yaml_event_t;
    c_memset(c_ptrTo(p):c_void_ptr, 0, c_sizeof(yaml_parser_t));
    c_memset(c_ptrTo(e):c_void_ptr, 0, c_sizeof(yaml_event_t));
    this.parser = p;
    this.event = e;
    this.fileAttached = false;
  }

  proc LibYamlParser.deinit() {
    yaml_parser_delete(c_ptrTo(this.parser));
    yaml_event_delete(c_ptrTo(this.event));
    c_free(c_ptrTo(this.parser));
    c_free(c_ptrTo(this.event));
  }

  proc LibYamlParser.attachFile(fr) throws {
    // get a file pointer from the fileReader
    const (hasFp, fp) = fr._getFp();
    if !hasFp
      then throw new YamlParserError("Cannot parse yaml from a memory file.");
      else this.f = fp;

    // set the file offset
    fseek(this.f, fr.offset(), SEEK_SET);

    // attach the file to the parser
    if !yaml_parser_initialize(c_ptrTo(this.parser))
      then throw new YamlParserError("Failed to initialize libyaml parser.");

    if !yaml_parser_set_input_file(c_ptrTo(this.parser), this.f)
      then throw new YamlParserError("Failed to attach file to libyaml parser.");
      else this.fileIsAttached = true;

    this.expectEvent(EventType.StreamStart);
  }

  proc LibYamlParser.finishFile() throws {
    this.expectEvent(EventType.StreamEnd);

    this.f = nil;
    this.fileIsAttached = false;
  }

  // -------- parsing --------

  proc LibYamlParser.expectEvent(fr, e: EventType): 2*uint throws {
    const (found, start, end) = this.parseNext(fr);
    if e != found then throw new YamlUnexpectedEventError(e:string, found:string);

    return (start, end);
  }

  proc LibYamlParser.expectEither(fr, e1: EventType, e2: EventType): (bool, uint, uint) throws {
    const (found, start, end) = this.parseNext(fr);

    if e1 == found then return (true, start, end);
    if e2 == found then return (false, start, end);

    throw new YamlUnexpectedEventError(e1:string + " or " + e2:string, found:string);
  }

  proc LibYamlParser.parseNext(fr): (EventType, uint, uint) throws {
    if !this.fileIsAttached then this.attachFile(fr);

    if !yaml_parser_parse(c_ptrTo(this.parser), c_ptrTo(this.event)) then
      throw new YamlParserError("Failed to parse next libyaml event.");

    return (
      getEvent(event.t),
      this.event.start_mark.idx,
      this.event.end_mark.idx
    );
  }

  // ----------------------------------------------------
  // Filesystem stuff
  // ----------------------------------------------------

  private extern proc fclose(file: c_FILE): c_int;
  private extern proc fseek(file: c_FILE, offset: c_long, origin: c_int): c_int;
  private extern proc tmpfile(): c_FILE;
  private extern proc fread(ptr: c_ptr(c_uchar), size: c_size_t, nmemb: c_size_t, stream: c_FILE): c_size_t;
  private extern proc ftell(stream: c_FILE): c_long;
  private extern proc fflush(stream: c_FILE): c_int;
  private extern proc fgets(s: c_ptr(c_uchar), size: c_int, stream: c_FILE): c_ptr(c_uchar);
  extern const SEEK_SET: c_int;
  extern const SEEK_END: c_int;

  // ----------------------------------------------------
  // Context Helper
  // ----------------------------------------------------

  class ContextCounter {
    // used to keep track of the overall context depth
    //  i.e., how many mappings, sequences, etc. are currently open
    var count = 0;
    // use to keep track of the current position in a class hierarchy
    //  when child classes are being (de)serialized, the parent classes
    //  deserializing intializer is also called; however, only one new
    //  mapping should be opened for each class. This counter is used
    //  to prevent multiple mappings from being opened.
    var classDepth = 0;

    proc enter() do
      this.count += 1;

    proc leave() do
      this.count -= 1;

    proc isBase: bool do
      return this.count == 0;

    proc enterClass() do
      this.classDepth += 1;

    proc leaveClass() do
      this.classDepth -= 1;

    proc inSuperClass: bool do
      return this.classDepth > 1;
  }

}
