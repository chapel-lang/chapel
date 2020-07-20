#ifndef PB_GENERATOR_HH
#define PB_GENERATOR_HH

#include <string>

#include <google/protobuf/compiler/code_generator.h>
#include <google/protobuf/descriptor.h>

namespace chapel {

  using namespace std;

  using namespace google::protobuf::compiler;
  using namespace google::protobuf;

  class Generator : public CodeGenerator {
  public:
    Generator();
    ~Generator();
    bool Generate(
      const FileDescriptor *file,
      const string &parameter,         
      GeneratorContext *generator_context,     
      string *error)
    const override;
  };

} // namespace chapel

#endif /* PB_GENERATOR_HH */
