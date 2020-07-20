#include <string>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include <generator.hh>
#include <helpers.hh>
#include <reflection_class.hh>

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
