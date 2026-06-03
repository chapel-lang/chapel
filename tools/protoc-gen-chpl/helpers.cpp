/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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
#include <string_view>

#include "helpers.h"
#include "primitive_field.h"
#include "repeated_primitive_field.h"
#include "enum_field.h"
#include "repeated_enum_field.h"
#include "message_field.h"
#include "repeated_message_field.h"
#include "map_field.h"
#include "field_base.h"

namespace chapel {

  std::string_view StripDotProto(std::string_view proto_file) {
    int lastindex = proto_file.find_last_of(".");
    return proto_file.substr(0, lastindex);
  }

  std::string InvalidCharsToUnderscores(std::string_view input) {
    std::string result;
    for (int i = 0; i < input.size(); i++) {
      if (isalnum(input[i]) || input[i] == '_') {
        result += input[i];
      } else {
        result += '_';
      }
    }
    return result;
  }

  std::string GetFileNameBase(const FileDescriptor* descriptor) {
      auto proto_file = descriptor->name();
      int lastslash = proto_file.find_last_of("/");
      auto base = proto_file.substr(lastslash + 1);
      return InvalidCharsToUnderscores(StripDotProto(base));
  }

  bool ValidateInputFileName(const FileDescriptor* descriptor) {
    auto proto_file = descriptor->name();
    int lastindex = proto_file.find_last_of(".");
    if (proto_file.substr(lastindex+1) != "proto") {
      return false;
    }
    return true;
  }

  std::string GetPackageName(const FileDescriptor* descriptor) {
    return InvalidCharsToUnderscores(descriptor->package());
  }

  std::string GetModuleName(const FileDescriptor* descriptor) {
    std::string package_name = GetPackageName(descriptor);
    if (!package_name.empty()) {
      return package_name;
    }
    std::string proto_filename = GetFileNameBase(descriptor);
    return proto_filename;
  }

  std::string GetOutputFile(const FileDescriptor* descriptor, std::string& error) {
    std::string file_extension = ".chpl";

    bool valid_input_filename = ValidateInputFileName(descriptor);
    if (!valid_input_filename) {
      error = "Input file should be a .proto file";
      return "";
    }

    std::string base_filename = GetModuleName(descriptor);
    return base_filename + file_extension;
  }

  std::string GetFieldName(const FieldDescriptor* descriptor) {
      return std::string(descriptor->name());
  }

  std::string GetNestedTypeName(const Descriptor* descriptor, std::string_view name) {
    if (descriptor != NULL) {
      return std::string(descriptor->name()) + "_" + std::string(name);
    }
    return std::string(name);
  }

  std::string GetMessageName(const Descriptor* descriptor) {
    return GetNestedTypeName(descriptor->containing_type(), descriptor->name());
  }

  std::string GetEnumName(const EnumDescriptor* descriptor) {
    return GetNestedTypeName(descriptor->containing_type(), descriptor->name());
  }

  std::string GetOneofName(const OneofDescriptor* descriptor) {
    return GetNestedTypeName(descriptor->containing_type(), descriptor->name());
  }

  FieldGeneratorBase* CreateFieldGenerator(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_MESSAGE:
        if (descriptor->is_repeated()) {
          if (descriptor->is_map()) {
            return new MapFieldGenerator(descriptor);
          } else {
            return new RepeatedMessageFieldGenerator(descriptor);
          }
        } else {
          if (descriptor->real_containing_oneof()) {
            return new MessageOneofFieldGenerator(descriptor);
          } else {
            return new MessageFieldGenerator(descriptor);
          }
        }
      case FieldDescriptor::TYPE_ENUM:
        if (descriptor->is_repeated()) {
          return new RepeatedEnumFieldGenerator(descriptor);
        } else {
          if (descriptor->real_containing_oneof()) {
            return new EnumOneofFieldGenerator(descriptor);
          } else {
            return new EnumFieldGenerator(descriptor);
          }
        }
      default:
        if (descriptor->is_repeated()) {
          return new RepeatedPrimitiveFieldGenerator(descriptor);
        } else {
          if (descriptor->real_containing_oneof()) {
            return new PrimitiveOneofFieldGenerator(descriptor);
          } else {
            return new PrimitiveFieldGenerator(descriptor);
          }
        }
    }
  }

} // namespace chapel
