How to test ANNOTATIONS.yaml changes locally
============================================

Annotations are automatically checked as part of the pre-merge Travis smoke
test, but you can check them locally with::

    $CHPL_HOME/util/test/check_annotations.py

If that fails with an error message like ``ImportError: No module named
yaml`` it means the python YAML module isn't installed. You can either
resolve this by installing it yourself with ``python3 -m pip install
PyYAML`` or by using the version bundled with the testing system::

    cd $CHPL_HOME && make test-venv
    $CHPL_HOME/util/test/run-in-test-venv.bash $CHPL_HOME/util/test/check_annotations.py
