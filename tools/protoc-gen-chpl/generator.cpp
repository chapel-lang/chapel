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

#include <string>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <generator.h>
#include <helpers.h>
#include <reflection_class.h>

namespace chapel {

  using namespace google::protobuf::io;

  Generator::Generator() {}
  Generator::~Generator() {}

  void GenerateFile(const FileDescriptor* file, Printer* printer) {
    ReflectionClassGenerator reflectionClassGenerator(file);
    reflectionClassGenerator.Generate(printer);
  }

  bool Generator::
  Generate(
      const FileDescriptor *file, const string &parameter,
      GeneratorContext *generator_context, string *error) const {
        
        string filename_error = "";
        string filename = GetOutputFile(file, &filename_error);

        if (filename.empty()) {
          *error = filename_error;
          return false;
        }
        unique_ptr< ZeroCopyOutputStream> output(
            generator_context->Open(filename));
        Printer printer(output.get(), '$');
          
        GenerateFile(file, &printer);

        return true;
  }
}
