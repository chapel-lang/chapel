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

#include <google/protobuf/stubs/strutil.h>
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

  FieldGeneratorBase::FieldGeneratorBase(const FieldDescriptor* descriptor)
      : descriptor_(descriptor) {
    SetCommonFieldVariables(&variables_);
  }

  FieldGeneratorBase::~FieldGeneratorBase() {
  }

  string FieldGeneratorBase::name() {
    return GetFieldName(descriptor_);
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
        return descriptor->enum_type()->name();
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
      default:
        GOOGLE_LOG(FATAL)<< "Unknown field type.";
        return "";
    }
  }

  string FieldGeneratorBase::number() {
    return StrCat(descriptor_->number());
  }

}  // namespace chapel
