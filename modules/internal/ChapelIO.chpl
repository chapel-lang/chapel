/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ChapelIO.chpl
//
module ChapelIO {
  use ChapelBase; // for uint().
  use SysBasic;
  // use IO; happens below once we need it.
  
  proc _isNilObject(val) {
    proc helper(o: object) return o == nil;
    proc helper(o)         return false;
    return helper(val);
  }
 
  // This routine is called in DefaultRectangular in order
  // to report an out of bounds access for a halt. A normal
  // call to halt with a tuple can't be made because of module
  // order issues.
  proc _stringify_index(tup:?t) where isTuple(t)
  {
    var str = "(";

    for param i in 1..tup.size {
      if i != 1 then str += ", ";
      str += tup[i]:string;
    }

   str += ")";

    return str;

  }

  use IO;

  /*
  class Writer {
    proc writing param return true;
    // if it's binary, we don't decorate class/record fields and values
    proc binary():bool { return false; }
    // get other style elements
    proc styleElement(element:int):int { return 0; }

    proc error():syserr { return ENOERR; }
    proc setError(e:syserr) { }
    proc clearError() { }
    proc writePrimitive(x) {
      //compilerError("Generic Writer.writePrimitive called");
      halt("Generic Writer.writePrimitive called");
    }
    proc writeBytes(x, len:ssize_t) {
      halt("Generic Writer.writeBytes called");
    }
    proc writeIt(x:?t) {
      if _isIoPrimitiveTypeOrNewline(t) {
        writePrimitive(x);
      } else {
        if isClassType(t) || chpl_isDdata(t) {
          // FUTURE -- write the class name/ID?
  
          if x == nil {
            var st = styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
            var iolit:ioLiteral;
            if st == QIO_AGGREGATE_FORMAT_JSON {
              iolit = new ioLiteral("null", !binary());
            } else {
              iolit = new ioLiteral("nil", !binary());
            }
            writePrimitive(iolit);
            return;
          }
        }
  
        x.writeThis(this);
      }
    }
    proc readwrite(x) {
      writeIt(x);
    }
    proc write(args ...?k) {
      for param i in 1..k {
        writeIt(args(i));
      }
    }
    proc writeln(args ...?k) {
      for param i in 1..k {
        writeIt(args(i));
      }
      var nl = new ioNewline();
      writeIt(nl);
    }
    proc writeln() {
      var nl = new ioNewline();
      writeIt(nl);
    }
  }*/
 
    proc writeThisFieldsDefaultImpl(writer, x:?t, inout first:bool) {
      param num_fields = __primitive("num fields", t);
      var isBinary = writer.binary();
  
      if (isClassType(t)) {
        if t != object {
          // only write parent fields for subclasses of object
          // since object has no .super field.
          writeThisFieldsDefaultImpl(writer, x.super, first);
        }
      }
  
      if !isUnionType(t) {
        // print out all fields for classes and records
        for param i in 1..num_fields {
          if !isBinary {
            var comma = new ioLiteral(", ");
            if !first then writer.readwrite(comma);
  
            var st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
            var eq:ioLiteral;
            if st == QIO_AGGREGATE_FORMAT_JSON {
              eq = new ioLiteral('"'+
                                 __primitive("field num to name", t, i) +
                                 '":');
            } else {
              eq = new ioLiteral(__primitive("field num to name", t, i) + " = ");
            }
            writer.readwrite(eq);
          }
  
          writer.readwrite(__primitive("field value by num", x, i));
  
          first = false;
        }
      } else {
        // Handle unions.
        // print out just the set field for a union.
        var id = __primitive("get_union_id", x);
        for param i in 1..num_fields {
          if __primitive("field id by num", t, i) == id {
            if isBinary {
              // store the union ID
              write(id);
            } else {
              var st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
              var eq:ioLiteral;
              if st == QIO_AGGREGATE_FORMAT_JSON {
                eq = new ioLiteral(__primitive("field num to name", t, i) + " : ");
              } else {
                eq = new ioLiteral(__primitive("field num to name", t, i) + " = ");
              }
              writer.readwrite(eq);
            }
            writer.readwrite(__primitive("field value by num", x, i));
          }
        }
      }
    }
    // Note; this is not a multi-method and so must be called
    // with the appropriate *concrete* type of x; that's what
    // happens now with buildDefaultWriteFunction
    // since it has the concrete type and then calls this method.
  
    // MPF: We would like to entirely write the default writeThis
    // method in Chapel, but that seems to be a bit of a challenge
    // right now and I'm having trouble with scoping/modules.
    // So I'll go back to writeThis being generated by the
    // compiler.... the writeThis generated by the compiler
    // calls writeThisDefaultImpl.
    proc writeThisDefaultImpl(writer, x:?t) {
      if !writer.binary() {
        var st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
        var start:ioLiteral;
        if st == QIO_AGGREGATE_FORMAT_JSON {
          start = new ioLiteral("{");
        } else if st == QIO_AGGREGATE_FORMAT_CHPL {
          start = new ioLiteral("new " + typeToString(t) + "(");
        } else {
          // the default 'braces' type
          if isClassType(t) {
            start = new ioLiteral("{");
          } else {
            start = new ioLiteral("(");
          }
        }
        writer.readwrite(start);
      }
  
      var first = true;
  
      writeThisFieldsDefaultImpl(writer, x, first);
  
      if !writer.binary() {
        var st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
        var end:ioLiteral;
        if st == QIO_AGGREGATE_FORMAT_JSON {
          end = new ioLiteral("}");
        } else if st == QIO_AGGREGATE_FORMAT_CHPL {
          end = new ioLiteral(")");
        } else {
          if isClassType(t) {
            end = new ioLiteral("}");
          } else {
            end = new ioLiteral(")");
          }
        }
        writer.readwrite(end);
      }
    }
 
  /*class Reader {
    proc writing param return false;
    // if it's binary, we don't decorate class/record fields and values
    proc binary():bool { return false; }
    // get other style elements
    proc styleElement(element:int):int { return 0; }

    proc error():syserr { return ENOERR; }
    proc setError(e:syserr) { }
    proc clearError() { }
  
    proc readPrimitive(ref x:?t) where _isIoPrimitiveTypeOrNewline(t) {
      //compilerError("Generic Reader.readPrimitive called");
      halt("Generic Reader.readPrimitive called");
    }
    proc readBytes(x, len:ssize_t) {
      halt("Generic Reader.readBytes called");
    }
    proc readIt(x:?t) where isClassType(t) {
      // FUTURE -- write the class name/ID? or nil?
      // possibly in a different 'Reader'
      /*
      var iolit = new ioLiteral("nil", !binary());
      readPrimitive(iolit);
      if !(error()) {
        // Return nil.
        delete x;
        x = nil;
        return;
      } else {
        clearError();
      }*/
      x.readThis(this);
    }
    proc readIt(ref x:?t) where !isClassType(t) {
      if _isIoPrimitiveTypeOrNewline(t) {
        readPrimitive(x);
      } else {
        x.readThis(this);
      }
    }
    proc readwrite(ref x) {
      readIt(x);
    }
    proc read(ref args ...?k):bool {
      for param i in 1..k {
        readIt(args(i));
      }
  
      if error() {
        return false;
      } else {
        return true;
      }
    }
    proc readln(ref args ...?k):bool {
      for param i in 1..k {
        readIt(args(i));
      }
      var nl = new ioNewline();
      readIt(nl);
      if error() {
        return false;
      } else {
        return true;
      }
    }
    proc readln():bool {
      var nl = new ioNewline();
      readIt(nl);
      if error()  {
        return false;
      } else {
        return true;
      }
    }
  }*/

    proc readThisFieldsDefaultImpl(reader, type t, ref x, inout first:bool) {
      param num_fields = __primitive("num fields", t);
      var isBinary = reader.binary();
  
      //writeln("Scanning fields for ", typeToString(t));
  
      if (isClassType(t)) {
        if t != object {
          // only write parent fields for subclasses of object
          // since object has no .super field.
          readThisFieldsDefaultImpl(reader, x.super.type, x, first);
        }
      }
  
      if !isUnionType(t) {
        // read all fields for classes and records
        if isBinary {
          for param i in 1..num_fields {
            reader.readwrite(__primitive("field value by num", x, i));
          }
        } else {
          var gotcomma = false;
          // the order should not matter.
          while true {
            if !first && !gotcomma {
              // read a comma
              var comma = new ioLiteral(",", true);
              reader.readwrite(comma);
              // clear the error if we didn't get a comma
              if error() == EFORMAT {
                clearError();
                break;
              }
            }

            // find a field name that matches.
            // TODO: this is not particularly efficient. If we
            // have a lot of fields, this is O(n**2), and there
            // are other potential problems with string reallocation.
            var st = styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
            var skip_unk = styleElement(QIO_STYLE_ELEMENT_SKIP_UNKNOWN_FIELDS);

            // the field name
            var fname:ioLiteral;

            var read_field_name = false;

            for param i in 1..num_fields {
              if !read_field_name {
                if st == QIO_AGGREGATE_FORMAT_JSON {
                  fname = new ioLiteral('"' +
                                        __primitive("field num to name", t, i)
                                        + '"',
                                        true);
                } else {
                  fname = new ioLiteral(__primitive("field num to name", t, i),
                                        true);
                }

                reader.readwrite(fname);
                if error() == EFORMAT {
                  // Try reading again with a different field name.
                  clearError();
                } else {
                  read_field_name = true;

                  var eq:ioLiteral;
                  if st == QIO_AGGREGATE_FORMAT_JSON {
                    eq = new ioLiteral(":", true);
                  } else {
                    eq = new ioLiteral("=", true);
                  }
                  reader.readwrite(eq);
        
                  reader.readwrite(__primitive("field value by num", x, i));
        
                  first = false;
                }
              }
            }

            // Stop with an error if we didn't read a field name
            // ... unless we skip unknown fields...
            if !read_field_name {
              if skip_unk && st == QIO_AGGREGATE_FORMAT_JSON {
                // Skip an unknown JSON field.

                //TODO;
                gotcomma = true;
              } else {
                setError(EFORMAT:syserr);
                break;
              }
            }
          }
        }
      } else {
        // Handle unions.
        if isBinary {
          var id = __primitive("get_union_id", x);
          // Read the ID
          reader.readwrite(id);
          for param i in 1..num_fields {
            if __primitive("field id by num", t, i) == id {
              reader.readwrite(__primitive("field value by num", x, i));
            }
          }
        } else {
          // Read the field name = part until we get one that worked.
          var found_field = false;
          for param i in 1..num_fields {
            var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
            var eq:ioLiteral;
            
            // the field name
            var fname:ioLiteral;

            if st == QIO_AGGREGATE_FORMAT_JSON {
              fname = new ioLiteral('"' +
                                    __primitive("field num to name", t, i)
                                    + '"',
                                    true);
            } else {
              fname = new ioLiteral(__primitive("field num to name", t, i),
                                    true);
            }

            reader.readwrite(fname);
  
            // Read : or = if there was no error reading field name.
            if error() == EFORMAT {
              // Try reading again with a different union element.
              clearError();
            } else {
              found_field = true;
              var eq:ioLiteral;
              if st == QIO_AGGREGATE_FORMAT_JSON {
                eq = new ioLiteral(":", true);
              } else {
                eq = new ioLiteral("=", true);
              }
              readIt(eq);

              // We read the 'name = ', so now read the value!
              reader.readwrite(__primitive("field value by num", x, i));
            }
          }
          // Create an error if we never found a field in our union.
          if !found_field {
            setError(EFORMAT:syserr);
          }
        }
      }
    }
    // Note; this is not a multi-method and so must be called
    // with the appropriate *concrete* type of x; that's what
    // happens now with buildDefaultWriteFunction
    // since it has the concrete type and then calls this method.
    proc readThisDefaultImpl(reader, x:?t) where isClassType(t) {
      if !reader.binary() {
        var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
        var start:ioLiteral;
        if st == QIO_AGGREGATE_FORMAT_CHPL {
          start = new ioLiteral("new " + typeToString(t) + "(");
        } else {
          // json and braces type
          start = new ioLiteral("{");
        }
        reader.readwrite(start);
      }
  
      var first = true;
  
      var obj = x; // make obj point to x so ref works
      readThisFieldsDefaultImpl(reader, t, obj, first);
  
      if !reader.binary() {
        var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
        var end:ioLiteral;
        if st == QIO_AGGREGATE_FORMAT_CHPL {
          end = new ioLiteral(")");
        } else {
          // json and braces type
          end = new ioLiteral("}");
        }
        reader.readwrite(end);
      }
    }
    proc readThisDefaultImpl(reader, ref x:?t) where !isClassType(t){
      if !reader.binary() {
        var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
        var start:ioLiteral;
        if st == QIO_AGGREGATE_FORMAT_CHPL {
          start = new ioLiteral("new " + typeToString(t) + "(");
        } else if st == QIO_AGGREGATE_FORMAT_JSON {
          start = new ioLiteral("{");
        } else {
          start = new ioLiteral("(");
        }
        reader.readwrite(start);
      }
  
      var first = true;
  
      readThisFieldsDefaultImpl(reader, t, x, first);
  
      if !reader.binary() {
        var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
        var end:ioLiteral;
        if st == QIO_AGGREGATE_FORMAT_JSON {
          end = new ioLiteral("}");
        } else {
          end = new ioLiteral(")");
        }
        reader.readwrite(end);
      }
    }
  
    /*
  inline proc <~>(w: Writer, x):Writer {
    w.readwrite(x);
    return w;
  }
  inline proc <~>(r: Reader, ref x):Reader {
    r.readwrite(x);
    return r;
  } */
  
  //use IO;
  
  // these are overridden to not be inout
  // since they don't change when read anyway
  // and it's much more convenient to be able to do e.g.
  //   reader & new ioLiteral("=")
  /*
  inline proc <~>(r: Reader, lit:ioLiteral):Reader {
    var litCopy = lit;
    r.readwrite(litCopy);
    return r;
  }
  inline proc <~>(r: Reader, nl:ioNewline):Reader {
    var nlCopy = nl;
    r.readwrite(nlCopy);
    return r;
  }
  
  inline proc Reader.readWriteLiteral(lit:string, ignoreWhiteSpace=true)
  {
    /***
    var iolit = new ioLiteral(lit.c_str(), ignoreWhiteSpace);
    this.readwrite(iolit);
    ***/
    this.readWriteLiteral(lit.c_str(), ignoreWhiteSpace);
  }
  inline proc Reader.readWriteLiteral(lit:c_string, ignoreWhiteSpace=true)
  {
    var iolit = new ioLiteral(lit, ignoreWhiteSpace);
    this.readwrite(iolit);
  }
  inline proc Writer.readWriteLiteral(lit:string, ignoreWhiteSpace=true)
  {
    /***
    var iolit = new ioLiteral(lit.c_str(), ignoreWhiteSpace);
    this.readwrite(iolit);
    ***/
    this.readWriteLiteral(lit.c_str(), ignoreWhiteSpace);
  }
  inline proc Writer.readWriteLiteral(lit:c_string, ignoreWhiteSpace=true)
  {
    var iolit = new ioLiteral(lit, ignoreWhiteSpace);
    this.readwrite(iolit);
  }
  inline proc Reader.readWriteNewline()
  {
    var ionl = new ioNewline();
    this.readwrite(ionl);
  }
  inline proc Writer.readWriteNewline()
  {
    var ionl = new ioNewline();
    this.readwrite(ionl);
  }
  */
  proc halt() {
    __primitive("chpl_error", "halt reached");
  }
  
  proc halt(s:string) {
    halt(s.c_str());
  }

  proc halt(s:c_string) {
    __primitive("chpl_error", "halt reached - " + s);
  }
  
  proc halt(args ...?numArgs) {
    var tmpstring = stringify((...args));
    __primitive("chpl_error", "halt reached - " + tmpstring);
  }
  
  proc warning(s:string) {
    warning(s.c_str());
  }

  proc warning(s:c_string) {
    __primitive("chpl_warning", s);
  }
  
  proc warning(args ...?numArgs) {
    var tmpstring: c_string_copy;
    tmpstring.write((...args));
    warning(tmpstring);
    chpl_free_c_string_copy(tmpstring);
  }
  
  proc _ddata.writeThis(f) {
    compilerWarning("printing _ddata class");
    f.write("<_ddata class cannot be printed>");
  }

  proc chpl_taskID_t.writeThis(f) {
    var tmp : uint(64) = this : uint(64);
    f.write(tmp);
  }
  proc chpl_taskID_t.readThis(f) {
    var tmp : uint(64);
    f.read(tmp);
    this = tmp : chpl_taskID_t;
  }
  
  /*
  class StringWriter: Writer {
    var s: c_string_copy; // Should be initialized to NULL.
    proc StringWriter(x:c_string) {
      this.s = __primitive("string_copy", x);
    }
    proc writePrimitive(x) {
      // TODO: Implement += so it consumes a c_string_copy LHS.
      var aug = x:c_string_copy;
      this.s += aug;      // The update frees this.s before overwriting it.
      chpl_free_c_string_copy(aug);
    }
    proc ~StringWriter() {
      chpl_free_c_string_copy(this.s);
      __primitive("=", this.s, _nullString);
    }
  }*/
 

  // Convert 'x' to a string just the way it would be written out.
  // This version only applies to non-primitive types
  // (primitive types should support :string directly)
  proc _cast(type t, x):string where t == string && ! isPrimitiveType(x.type) {
    return stringify(x);
  }
  
  // Removing this causes a compiler error.
  // unable to resolve AbstractRootLocale.chpl_initOnLocales(tag=iterKind)
  // in _toLeader
  // candidates are AbstractRootLocale.chpl_initOnLocales(param tag: iterKind) 
  /*pragma "dont disable remote value forwarding"
  proc ref c_string.write(args ...?n) {
    compilerError("HERE");
    var sc = new StringWriter(this);
    sc.write((...args));
    // We need to copy this string because the destructor call below frees it
    this = sc.s;
    // This is required to prevent double-deletion.
    __primitive("=", sc.s, _nullString);
    delete sc;
  }*/
 
  /*
  pragma "dont disable remote value forwarding"
  proc ref string.write(args ...?n) {
    var sc = new StringWriter(this.c_str());
    sc.write((...args));
    this = toString(sc.s);
    // This is required to prevent double-deletion.
    __primitive("=", sc.s, _nullString);
    delete sc;
  }*/
 
  proc _getoutputformat(s: c_string):c_string {
    var sn = s.length;
    var afterdot = false;
    var dplaces = 0;
    for i in 1..sn {
      var ss = s.substring(i);
      if ((ss == '#') & afterdot) then dplaces += 1;
      if (ss == '.') then afterdot=true;
      chpl_free_c_string_copy(ss);
    }
    // FIX ME: leak c_string due to concatenation
    return("%" + sn + "." + dplaces + "f");
  }
  
  //
  // When this flag is used during compilation, calls to chpl__testPar
  // will output a message to indicate that a portion of the code has been
  // parallelized.
  //
  
  config param chpl__testParFlag = false;
  var chpl__testParOn = false;
  
  proc chpl__testParStart() {
    chpl__testParOn = true;
  }
  
  proc chpl__testParStop() {
    chpl__testParOn = false;
  }
  
  proc chpl__testPar(args...) {
    if chpl__testParFlag && chpl__testParOn {
      const file : c_string = __primitive("_get_user_file");
      const line = __primitive("_get_user_line");
      writeln("CHPL TEST PAR (", file, ":", line, "): ", (...args));
    }
  }

}
