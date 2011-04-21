#Run this script from the base directory

find . -name "*.hpp" -exec headache -h scripts/preamble.txt -c scripts/headache.cfg {} +
find . -name "*.cpp" -exec headache -h scripts/preamble.txt -c scripts/headache.cfg {} +
find . -name "*.c" -exec headache -h scripts/preamble.txt -c scripts/headache.cfg {} +
find . -name "*.h" -exec headache -h scripts/preamble.txt -c scripts/headache.cfg {} +
find . -name "Makefile.am" -exec headache -h scripts/preamble.txt -c scripts/headache.cfg {} +

