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

#ifndef PB_PRIMITIVE_FIELD_HH
#define PB_PRIMITIVE_FIELD_HH

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>

#include <field_base.h>

namespace chapel {
  
  using namespace google::protobuf;
  using namespace google::protobuf::io;
  
  class PrimitiveFieldGenerator : public FieldGeneratorBase {
   public:
    PrimitiveFieldGenerator(const FieldDescriptor* descriptor);
    ~PrimitiveFieldGenerator();

    void GenerateMembers(Printer* printer);
  };

  class PrimitiveOneofFieldGenerator : public FieldGeneratorBase {
   public:
    PrimitiveOneofFieldGenerator(const FieldDescriptor* descriptor);
    ~PrimitiveOneofFieldGenerator();

    void GenerateMembers(Printer* printer);
  };

}  // namespace chapel

#endif /* PB_PRIMITIVE_FIELD_HH */
