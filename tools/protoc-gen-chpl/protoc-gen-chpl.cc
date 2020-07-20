#include <google/protobuf/compiler/plugin.h>

#include <generator.hh>

int main(int argc, char *argv[]) {
  chapel::Generator generator;
  return PluginMain(argc, argv, &generator);
}
