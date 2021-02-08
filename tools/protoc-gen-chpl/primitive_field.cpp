/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include <primitive_field.h>

namespace chapel {

  PrimitiveFieldGenerator::PrimitiveFieldGenerator(const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
  }

  PrimitiveFieldGenerator::~PrimitiveFieldGenerator() {
  }

  void PrimitiveFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(variables_, "var $name$: $type_name$;\n");
  }
  
  PrimitiveOneofFieldGenerator::PrimitiveOneofFieldGenerator(const FieldDescriptor* descriptor)
      : FieldGeneratorBase(descriptor) {
    SetCommonOneofFieldVariables(&variables_);
  }

  PrimitiveOneofFieldGenerator::~PrimitiveOneofFieldGenerator() {
  }

  void PrimitiveOneofFieldGenerator::GenerateMembers(Printer* printer) {
    printer->Print(variables_,
      "var $property_name$: $type_name$;\n"
      "proc $name$ {\n"
      "  if $oneof_name$SetVal == $oneof_name$.$name$ then return $property_name$;\n"
      "  else return $default_value$;\n"
      "}\n"
      "proc ref $name$ ref {\n"
      "  $oneof_name$SetVal = $oneof_name$.$name$;\n"
      "  return $property_name$;\n"
      "}\n");
  }

}  // namespace chapel
