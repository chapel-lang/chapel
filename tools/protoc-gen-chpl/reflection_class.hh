#ifndef REFLECTION_CLASS_HH
#define REFLECTION_CLASS_HH

#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/printer.h>

namespace chapel {

  using namespace std;

  using namespace google::protobuf;
  using namespace google::protobuf::io;

  class ReflectionClassGenerator {
   public:
    ReflectionClassGenerator(const FileDescriptor* file);
    ~ReflectionClassGenerator();

    void Generate(Printer* printer);

   private:
    const FileDescriptor* file_;

    string module_name;

    void WriteIntroduction(Printer* printer);
  };

}  // namespace chapel

#endif  // REFLECTION_CLASS_HH
