#!/bin/bash

rm -Rf ./DummyDoxygenConfig ./DummyDoxygenConfig.bak
rm -Rf ./xml
doxygen -g DummyDoxygenConfig
echo "GENERATE_XML = YES" >> DummyDoxygenConfig
doxygen DummyDoxygenConfig
rm -Rf ./html
rm -Rf ./latex
rm -Rf ./DummyDoxygenConfig ./DummyDoxygenConfig.bak

# leaves only the xml directory
