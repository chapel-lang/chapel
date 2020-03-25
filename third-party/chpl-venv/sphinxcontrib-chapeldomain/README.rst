Chapel Domain for Sphinx
========================

Chapel_ domain for Sphinx_.

.. _Chapel: http://chapel-lang.org/
.. _Sphinx: http://sphinx-doc.org/

.. image:: https://travis-ci.org/chapel-lang/sphinxcontrib-chapeldomain.svg?branch=master
    :target: https://travis-ci.org/chapel-lang/sphinxcontrib-chapeldomain

.. image:: https://coveralls.io/repos/chapel-lang/sphinxcontrib-chapeldomain/badge.svg?branch=master
    :target: https://coveralls.io/r/chapel-lang/sphinxcontrib-chapeldomain?branch=master

`Package documentation`_ is available on readthedocs.org.

.. _Package documentation: //sphinxcontrib-chapeldomain.readthedocs.org/

Installation
------------

To install::

    pip install sphinxcontrib-chapeldomain

To install from source on github_::

    git clone https://github.com/chapel-lang/sphinxcontrib-chapeldomain
    cd sphinxcontrib-chapeldomain
    python setup.py install

.. _github: https://github.com/chapel-lang/sphinxcontrib-chapeldomain

Getting Started
---------------

This is an example that covers several features of the Chapel domain::

    .. chpl:module:: GMP
        :synopsis: multiple precision integer library

    .. chpl:record:: BigNum

        multiple precision instances

        .. chpl:method:: proc add(a:BigNum, b:BigNum)

            Add two big ints, ``a`` and ``b``, and store the result in ``this``
            instance.

            :arg a: BigNum to be added
            :type a: BigNum

            :arg BigNum b: BigNum to be added

            :returns: nothing, result is stored in current instance

        .. chpl:itermethod:: iter these() ref

            Arbitrary iterator that returns individual digits of this instance.

            :ytype: reference
            :yields: reference to each individual digit of BigNum
