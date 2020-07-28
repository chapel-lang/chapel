/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include <string>

#include <google/protobuf/stubs/strutil.h>

#include <helpers.h>
#include <primitive_field.h>
#include <repeated_primitive_field.h>
#include <enum_field.h>
#include <repeated_enum_field.h>
#include <field_base.h>

namespace chapel {
  
  string StripDotProto(const std::string& proto_file) {
    int lastindex = proto_file.find_last_of(".");
    return proto_file.substr(0, lastindex);
  }

  string InvalidCharsToUnderscores(const string& input) {
    string result;
    for (int i = 0; i < input.size(); i++) {
      if ( isalnum(input[i]) || input[i] == '_') {
        result += input[i];
      } else {
        result += '_';
      }
    }
    return result;
  }

  string GetFileNameBase(const FileDescriptor* descriptor) {
      string proto_file = descriptor->name();
      int lastslash = proto_file.find_last_of("/");
      string base = proto_file.substr(lastslash + 1);
      return InvalidCharsToUnderscores(StripDotProto(base));
  }

  bool ValidateInputFileName(const FileDescriptor* descriptor) {
    string proto_file = descriptor->name();
    int lastindex = proto_file.find_last_of(".");
    if (proto_file.substr(lastindex+1) != "proto") {
      return false;
    }
    return true;
  }

  string GetPackageName(const FileDescriptor* descriptor) {
    return InvalidCharsToUnderscores(descriptor->package());
  }

  string GetModuleName(const FileDescriptor* descriptor) {
    string package_name = GetPackageName(descriptor);
    if (!package_name.empty()) {
      return package_name;
    }
    string proto_filename = GetFileNameBase(descriptor);
    return proto_filename;
  }

  string GetOutputFile(const FileDescriptor* descriptor, string* error) {
    string file_extension = ".chpl";

    bool valid_input_filename = ValidateInputFileName(descriptor);
    if (!valid_input_filename) {
      *error = "Input file should be a .proto file";
      return "";
    }

    string base_filename = GetModuleName(descriptor);
    return base_filename + file_extension;
  }

  string GetFieldName(const FieldDescriptor* descriptor) {
      return descriptor->name();
  }

  FieldGeneratorBase* CreateFieldGenerator(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_ENUM:
        if (descriptor->is_repeated()) {
          return new RepeatedEnumFieldGenerator(descriptor);
        } else {
          return new EnumFieldGenerator(descriptor);
        }
      default:
        if (descriptor->is_repeated()) {
          return new RepeatedPrimitiveFieldGenerator(descriptor);
        } else {
          return new PrimitiveFieldGenerator(descriptor);
        }  
    }
  }

} // namespace chapel
