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

#include <message_field.h>

namespace chapel {

  MessageFieldGenerator::MessageFieldGenerator(const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
  }

  MessageFieldGenerator::~MessageFieldGenerator() {
  }

  void MessageFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(
      variables_,
      "var $name$: $type_name$;\n"
    );
  }

  MessageOneofFieldGenerator::MessageOneofFieldGenerator(const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
    SetCommonOneofFieldVariables(&variables_);
  }

  MessageOneofFieldGenerator::~MessageOneofFieldGenerator() {
  }

  void MessageOneofFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(variables_,
      "var $property_name$: $type_name$;\n"
      "proc $name$ {\n"
      "  var defaultValue: $type_name$;\n"
      "  if $oneof_name$SetVal == $oneof_name$.$name$ then return $property_name$;\n"
      "  else return defaultValue;\n"
      "}\n"
      "proc ref $name$ ref {\n"
      "  $oneof_name$SetVal = $oneof_name$.$name$;\n"
      "  return $property_name$;\n"
      "}\n");
  }

}  // namespace chapel
