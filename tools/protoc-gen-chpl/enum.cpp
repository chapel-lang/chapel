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

#include <enum.h>
#include <helpers.h>

namespace chapel {

  EnumGenerator::EnumGenerator(const EnumDescriptor* descriptor) :
      descriptor_(descriptor) {
  }

  EnumGenerator::~EnumGenerator() {
  }

  void EnumGenerator::Generate(Printer* printer) {
    printer->Print("enum $name$ {\n",
                   "name", GetEnumName(descriptor_));
    printer->Indent();
    
    for (int i = 0; i < descriptor_->value_count(); i++) {
      printer->Print("$name$ = $number$,\n",
                     "name", descriptor_->value(i)->name(),
                     "number", std::to_string(descriptor_->value(i)->number()));
    }

    printer->Outdent();
    printer->Print("}\n"); 
              
  }

}  // namespace chapel
