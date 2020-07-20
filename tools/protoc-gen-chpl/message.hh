#ifndef PB_MESSAGE_HH
#define PB_MESSAGE_HH

#include <string>
#include <vector>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/wire_format.h>

namespace chapel {

  using namespace std;
  using namespace google::protobuf;
  using namespace google::protobuf::compiler;
  using namespace google::protobuf::io;

  class FieldGeneratorBase;

  class MessageGenerator {
   public:
    MessageGenerator(const Descriptor* descriptor);
    ~MessageGenerator();

    void Generate(Printer* printer);

   private:
    const Descriptor* descriptor_;

    FieldGeneratorBase* CreateFieldGeneratorInternal(
    const FieldDescriptor* descriptor);

    string record_name();
  };

}  // namespace chapel

#endif  // PB_MESSAGE_HH
