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

#ifndef PB_FIELD_BASE_HH
#define PB_FIELD_BASE_HH

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace chapel {

  using namespace google::protobuf;
  using namespace google::protobuf::io;
  using namespace google::protobuf::internal;

  class FieldGeneratorBase {
   public:
    FieldGeneratorBase(const FieldDescriptor* descriptor);
    virtual ~FieldGeneratorBase();

    virtual void GenerateMembers(Printer* printer) = 0;
    std::string name();
    std::string type_name(const FieldDescriptor* descriptor);
    std::string proto_type_name(const FieldDescriptor* descriptor);
    std::string default_value(const FieldDescriptor* descriptor);
    std::string number();

    const FieldDescriptor* descriptor_;
    std::map<std::string, std::string> variables_;

   protected:
     void SetCommonOneofFieldVariables(std::map<std::string, std::string>* variables);
     std::string oneof_name(const OneofDescriptor* descriptor);

   private:
     void SetCommonFieldVariables(std::map<std::string, std::string>* variables);
  };

}  // namespace chapel

#endif  // PB_FIELD_BASE_HH
