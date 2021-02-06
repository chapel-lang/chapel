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

#ifndef PB_MAP_FIELD_HH
#define PB_MAP_FIELD_HH

#include <string>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.h>

#include <field_base.h>

namespace chapel {
  
  using namespace google::protobuf;
  using namespace google::protobuf::io;
  
  class MapFieldGenerator : public FieldGeneratorBase {
   public:
    MapFieldGenerator(const FieldDescriptor* descriptor);
    ~MapFieldGenerator();

    void GenerateMembers(Printer* printer);
  };

}  // namespace chapelespace chapel

#endif  // PB_MAP_FIELD_HH
