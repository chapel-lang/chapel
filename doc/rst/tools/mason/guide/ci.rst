.. _ci-with-mason:

Setting up Automated Testing
----------------------------

Mason packages can be easily integrated into automated testing pipelines, such
as those used in continuous integration (CI) systems. For the most part, all
that is needed is a working install of Chapel/mason, and then the standard
``mason test`` command can be used to run the tests for a package.

There are two recommended approaches for setting up CI testing of mason
packages:

* Using the published Dockerhub images for Chapel, which come with mason
  pre-installed
* Installing Chapel/mason in the CI environment through binary packages

These docs provide some examples of setting up the CI for a mason package using
both of these approaches in GitHub Actions. However, the same principles can be
applied to other CI systems as well.

Using Containers
~~~~~~~~~~~~~~~~

Using the prebuild Chapel Dockerhub images is a very easy way to get started
with CI testing of mason packages. A very simple CI workflow using the
Dockerhub images might look like the following:

.. code-block:: yaml

    name: CI

    on:
      push:
        branches:
          - main
      pull_request:
      workflow_dispatch:

    jobs:
      tests:
        runs-on: ubuntu-latest
        container:
          image: chapel/chapel:latest
        steps:
          - uses: actions/checkout@v4
          - name: Check the project builds
            run: |
              mason build
          - name: Run mason tests
            run: |
              mason test
          - name: Check that examples build
            run: |
              mason build --examples
          - name: Run chplcheck
            run: |
              chplcheck src/**/*.chpl

This should go in your mason package's repository under
``.github/workflows/ci.yml``. This workflow will run on every pull request and
push to main. It will try and build the project, run the tests, build the
examples, and run chplcheck on all source files. If any of these steps fail,
the CI will be marked as failed.

This is the fastest way to get started with CI testing for mason packages,
since it requires no setup beyond writing the workflow file.


Using setup-chpl
~~~~~~~~~~~~~~~~

While using containers is the easiest way to get started with CI testing, it
does have some limitations. The biggest one is if you are already using a
container for your CI for some other dependency and can't easily switch to
using the Chapel Dockerhub images. You could build a custom container that
includes both your dependencies and Chapel/mason, but that can be a lot of work
to set up and maintain. An alternative to using containers is to use the
``setup-chpl`` GitHub Action to install Chapel and mason in your CI
environment.

.. code-block:: yaml

    name: CI

    on:
      push:
        branches:
          - main
      pull_request:
      workflow_dispatch:

    jobs:
      tests:
        runs-on: ubuntu-latest
        steps:
          - uses: actions/checkout@v4
          - name: Install Chapel and mason
            uses: jabraham17/setup-chpl@v1
            with:
              version: latest
          - name: Check the project builds
            run: |
              mason build
          - name: Run mason tests
            run: |
              mason test
          - name: Check that examples build
            run: |
              mason build --examples
          - name: Run chplcheck
            run: |
              chplcheck src/**/*.chpl

This should go in your mason package's repository under
``.github/workflows/ci.yml``. This workflow will run on every pull request and
push to main. It will try and build the project, run the tests, build the
examples, and run chplcheck on all source files. If any of these steps fail,
the CI will be marked as failed.

For more options to configure the ``setup-chpl`` action, see the `setup-chpl documentation <https://github.com/jabraham17/setup-chpl>`_.

.. warning::

   The ``setup-chpl`` action currently is not kept as up to date with the
   latest Chapel releases, so it may not always have the latest version of
   Chapel available. We recommend using the container-based approach for CI
   testing until the ``setup-chpl`` action is updated to support the latest
   Chapel releases.

More advanced CI setups
~~~~~~~~~~~~~~~~~~~~~~~

The above examples are very basic CI workflows that should work for most mason
packages. However, there are many ways to customize and enhance your CI
workflow beyond the basics. For example, you could:

* Test multiple versions of Chapel

  .. code-block:: yaml

      strategy:
        fail-fast: false
        matrix:
          version: [2.6.0, 2.7.0, latest]
      runs-on: ubuntu-latest
      container:
        image: chapel/chapel:${{ matrix.version }}

* Test against nightly Chapel builds

  .. code-block:: yaml

      container:
        image: chapel/chapel:nightly

* Test multi-locale code by simulating multiple locales

  .. code-block:: yaml

      strategy:
        fail-fast: false
        matrix:
          version: [2.6.0, 2.7.0, latest]
          base_image:
            - chapel/chapel
            - chapel/chapel-gasnet
            - chapel/chapel-gasnet-smp
      runs-on: ubuntu-latest
      container:
        image: ${{ matrix.base_image }}:${{ matrix.version }}

* Test multiple compiler backends and architectures

  .. code-block:: yaml

      strategy:
        fail-fast: false
        matrix:
          backend: [llvm, gnu]
          os: [ubuntu-latest, ubuntu-24.04-arm]
      runs-on: ${{ matrix.os }}
      container:
        image: chapel/chapel:latest
      env:
        CHPL_TARGET_COMPILER: ${{ matrix.backend }}

* Test on MacOS

  .. code-block:: yaml

      runs-on: macos-latest
      steps:
        - uses: Homebrew/actions/setup-homebrew@main
        - name: Install Chapel
          run: |
            brew install chapel
        - uses: actions/checkout@v4
        - name: Your Checks Here!
          run: |
            # ...

* Build and publish documentation for your package using GitHub Pages

  .. code-block:: yaml

      name: doc

      on:
        push:
          branches:
            - main
        pull_request:
        workflow_dispatch:

      # Sets permissions of the GITHUB_TOKEN to allow deployment to GitHub Pages
      permissions:
        contents: read
        pages: write
        id-token: write

      jobs:
        build:
          runs-on: ubuntu-latest
          container:
            image: chapel/chapel:latest
          steps:
            - uses: actions/checkout@v4
            - uses: actions/configure-pages@v5
            - name: Build docs
              run: |
                mason doc
            - uses: actions/upload-pages-artifact@v3
              with:
                path: ./doc

        deploy:
          # TODO: make sure to update OWNER_NAME/REPO_NAME to the
          # actual owner and repo name for your package
          if: github.event_name == 'push' && github.repository == 'OWNER_NAME/REPO_NAME'
          environment:
            name: github-pages
            url: ${{ steps.deployment.outputs.page_url }}
          runs-on: ubuntu-latest
          needs: build
          steps:
            - uses: actions/deploy-pages@v4
              id: deployment
