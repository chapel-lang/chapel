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

#ifndef PB_HELPERS_HH
#define PB_HELPERS_HH

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/compiler/code_generator.h>

namespace chapel {
  
  using namespace std;
  
  using namespace google::protobuf;
  using namespace google::protobuf::compiler;

  class FieldGeneratorBase;

  string GetOutputFile(const FileDescriptor* descriptor, string*error);
  
  string GetFieldName(const FieldDescriptor* descriptor);

  FieldGeneratorBase* CreateFieldGenerator(const FieldDescriptor* descriptor);

  string GetModuleName(const FileDescriptor* descriptor);
  
  string GetMessageName(const Descriptor* descriptor);

  string GetEnumName(const EnumDescriptor* descriptor);
  
  string GetPackageName(const FileDescriptor* descriptor);

  string GetOneofName(const OneofDescriptor* descriptor);
  
  inline bool IsMapEntryMessage(const Descriptor* descriptor) {
    return descriptor->options().map_entry();
  }

} // namespace chapel

#endif /* PB_HELPERS_HH */
