#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <unordered_map>

int main(int argc, char** argv) {
  const char* inputFile = "moby.txt";
  int repeat = 10;
  std::string timingArg = "--timing";
  bool printTiming = false;
  for (int i = 1; i < argc; i++) {
    if (argv[i] == timingArg)
      printTiming = true;
    else
      inputFile = argv[i];
  }

  std::unordered_map<std::string, std::string> map;

  std::ifstream file(inputFile);
  if (file.is_open()) {

    auto start = std::chrono::steady_clock::now();
    std::string line;
    std::string word;
    while (std::getline(file, line)) {
      for (int i = 0; i < repeat; i++) {
        std::istringstream iss(line, std::istringstream::in);
        while( iss >> word )
        {
          map.insert(std::make_pair(word, word));
        }
      }
    }
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    if (printTiming)
      std::cout << "unordered map elapsed time: " << elapsed.count() << " s\n";

    file.close();
  }
}
