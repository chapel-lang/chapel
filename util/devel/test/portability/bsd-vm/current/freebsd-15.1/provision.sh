#!/bin/sh

set -ex
cd "$(dirname "$0")/../../.."
provision-scripts/freebsd-pkg-deps.sh
provision-scripts/freebsd-pkg-llvm.sh
