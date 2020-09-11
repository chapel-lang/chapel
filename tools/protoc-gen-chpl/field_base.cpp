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

#include <google/protobuf/stubs/logging.h>

#include <field_base.h>
#include <helpers.h>

namespace chapel {
  

  void FieldGeneratorBase::SetCommonFieldVariables(
      map<string, string>* variables) {

    (*variables)["type_name"] = type_name(descriptor_);
    (*variables)["proto_type_name"] = proto_type_name(descriptor_);
    (*variables)["name"] = name();
    (*variables)["descriptor_name"] = descriptor_->name();
    (*variables)["number"] = number();
  }

  void FieldGeneratorBase::SetCommonOneofFieldVariables(
    std::map<string, string>* variables) {
    (*variables)["oneof_name"] = oneof_name(descriptor_->containing_oneof());
    (*variables)["property_name"] = name() + "_" ;
    (*variables)["default_value"] = default_value(descriptor_);
  }

  FieldGeneratorBase::FieldGeneratorBase(const FieldDescriptor* descriptor)
      : descriptor_(descriptor) {
    SetCommonFieldVariables(&variables_);
  }

  FieldGeneratorBase::~FieldGeneratorBase() {
  }

  string FieldGeneratorBase::name() {
    return GetFieldName(descriptor_);
  }
  
  string FieldGeneratorBase::oneof_name(const OneofDescriptor* descriptor) {
    return GetOneofName(descriptor);
  }

  string FieldGeneratorBase::type_name(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_INT64:
        return "int(64)";
      case FieldDescriptor::TYPE_INT32:
        return "int(32)";
      case FieldDescriptor::TYPE_UINT64:
        return "uint(64)";
      case FieldDescriptor::TYPE_UINT32:
        return "uint(32)";
      case FieldDescriptor::TYPE_SINT32:
        return "int(32)";
      case FieldDescriptor::TYPE_SINT64:
        return "int(64)";
      case FieldDescriptor::TYPE_BOOL:
        return "bool";
      case FieldDescriptor::TYPE_BYTES:
        return "bytes";
      case FieldDescriptor::TYPE_STRING:
        return "string";
      case FieldDescriptor::TYPE_FIXED64:
        return "uint(64)";
      case FieldDescriptor::TYPE_FIXED32:
        return "uint(32)";
      case FieldDescriptor::TYPE_FLOAT:
        return "real(32)";
      case FieldDescriptor::TYPE_DOUBLE:
        return "real(64)";
      case FieldDescriptor::TYPE_SFIXED32:
        return "int(32)";
      case FieldDescriptor::TYPE_SFIXED64:
        return "int(64)";
      case FieldDescriptor::TYPE_ENUM:
        return GetEnumName(descriptor->enum_type());
      case FieldDescriptor::TYPE_MESSAGE:
        return GetMessageName(descriptor->message_type());
      default:
        GOOGLE_LOG(FATAL)<< "Unknown field type.";
        return "";
    }
  }

  string FieldGeneratorBase::proto_type_name(const FieldDescriptor* descriptor) {
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_INT64:
        return "int64";
      case FieldDescriptor::TYPE_INT32:
        return "int32";
      case FieldDescriptor::TYPE_UINT64:
        return "uint64";
      case FieldDescriptor::TYPE_UINT32:
        return "uint32";
      case FieldDescriptor::TYPE_SINT32:
        return "sint32";
      case FieldDescriptor::TYPE_SINT64:
        return "sint64";
      case FieldDescriptor::TYPE_BOOL:
        return "bool";
      case FieldDescriptor::TYPE_BYTES:
        return "bytes";
      case FieldDescriptor::TYPE_STRING:
        return "string";
      case FieldDescriptor::TYPE_FIXED64:
        return "fixed64";
      case FieldDescriptor::TYPE_FIXED32:
        return "fixed32";
      case FieldDescriptor::TYPE_FLOAT:
        return "float";
      case FieldDescriptor::TYPE_DOUBLE:
        return "double";
      case FieldDescriptor::TYPE_SFIXED32:
        return "sfixed32";
      case FieldDescriptor::TYPE_SFIXED64:
        return "sfixed64";
      case FieldDescriptor::TYPE_ENUM:
        return "enum";
      case FieldDescriptor::TYPE_MESSAGE:
        if(descriptor->is_map()) {
          return "map";
        }
        return "message";
      default:
        GOOGLE_LOG(FATAL)<< "Unknown field type.";
        return "";
    }
  }

  string FieldGeneratorBase::default_value(const FieldDescriptor* descriptor) {	
    switch (descriptor->type()) {
      case FieldDescriptor::TYPE_MESSAGE:
        return ""; // we use chapel compiler generated default values for messages;
      case FieldDescriptor::TYPE_ENUM:
        return "0:" + GetEnumName(descriptor->enum_type());
      case FieldDescriptor::TYPE_INT64:
        return std::to_string(descriptor->default_value_int64());
      case FieldDescriptor::TYPE_INT32:
        return std::to_string(descriptor->default_value_int32());
      case FieldDescriptor::TYPE_UINT64:
        return std::to_string(descriptor->default_value_uint64());
      case FieldDescriptor::TYPE_UINT32:
        return std::to_string(descriptor->default_value_uint32());
      case FieldDescriptor::TYPE_SINT64:
        return std::to_string(descriptor->default_value_int64());
      case FieldDescriptor::TYPE_SINT32:
        return std::to_string(descriptor->default_value_int32());
      case FieldDescriptor::TYPE_BYTES:
        return "b\"\"";
      case FieldDescriptor::TYPE_STRING:
        return "";
      case FieldDescriptor::TYPE_BOOL:
        return "false";
      case FieldDescriptor::TYPE_FIXED64:
        return std::to_string(descriptor->default_value_uint64());
      case FieldDescriptor::TYPE_FIXED32:
        return std::to_string(descriptor->default_value_uint32());
      case FieldDescriptor::TYPE_FLOAT:
        return std::to_string(descriptor->default_value_float());
      case FieldDescriptor::TYPE_DOUBLE:
        return std::to_string(descriptor->default_value_double());
      case FieldDescriptor::TYPE_SFIXED32:
        return std::to_string(descriptor->default_value_int32());
      case FieldDescriptor::TYPE_SFIXED64:
        return std::to_string(descriptor->default_value_int64());
      default:
        GOOGLE_LOG(FATAL)<< "Unknown field type.";
        return "";
    }
  }

  string FieldGeneratorBase::number() {
    return std::to_string(descriptor_->number());
  }

}  // namespace chapel
