#!/usr/bin/env bash

set -e

CWD=$(cd $(dirname $0) ; pwd)

if [ "$(git rev-parse --abbrev-ref HEAD)" != "master" ] ; then
    echo "ERROR: current branch is not master"
    exit 1
fi

if [ -z "${VIRTUAL_ENV}" ] ; then
    echo "ERROR: not inside a virtualenv"
    exit 1
fi

repo_root=$CWD/..

echo "Moving to repo root: ${repo_root}"
cd $repo_root

version=$(python setup.py --version)
if [ -z "${version}" ] ; then
    echo "ERROR: Could not parse version number from setup.py"
    exit 1
fi
echo "Version number is: ${version}"

echo "Ensuring requirements are up-to-date..."
pip install -r requirements.txt -r test-requirements.txt -r docs-requirements.txt

echo "Ensuring package is installed..."
python setup.py develop

echo "Running tox..."
tox -e py26,py27,py34,flake8,coverage,docs,doc-test

sha1=$(git rev-parse HEAD)
echo "Tagging latest sha1 (${sha1}) as version ${version}"
git tag -a ${version} -m"release ${version}"

echo "Pushing tags to chapel-lang remote..."
git push https://github.com/chapel-lang/sphinxcontrib-chapeldomain --tags

echo "Cleaning repo..."
git clean -dxf

echo "Building and uploading python package to pypi..."
python setup.py sdist upload

echo "Version ${version} is released."
