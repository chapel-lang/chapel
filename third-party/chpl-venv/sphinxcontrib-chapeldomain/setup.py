import os.path
import re
from setuptools import setup, find_packages

with open('README.rst', 'r') as fp:
    long_desc = fp.read()

# Grab the version from chapeldomain module.
with open(os.path.join('sphinxcontrib', 'chapeldomain', '__init__.py'), 'r') as fp:
    version_pattern = re.compile(r'^VERSION\s*=\s*\'(?P<version>\d+\.\d+\.\d+)\'$', re.MULTILINE)
    version = version_pattern.search(fp.read()).group('version')

setup(
    name='sphinxcontrib-chapeldomain',
    version=version,
    url='https://github.com/chapel-lang/sphinxcontrib-chapeldomain',
    download_url='https://pypi.python.org/pypi/sphinxcontrib-chapeldomain',
    license='Apache License v2.0',
    author='Chapel Team',
    author_email='chapel-developers@lists.sourceforge.net',
    description='Chapel domain for Sphinx',
    long_description=long_desc,
    zip_safe=False,
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Environment :: Web Environment',
        'Intended Audience :: Developers',
        'Intended Audience :: Education',
        'License :: OSI Approved :: Apache Software License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 3',
        'Framework :: Sphinx :: Extension',
        'Topic :: Documentation',
        'Topic :: Documentation :: Sphinx',
        'Topic :: Text Processing',
        'Topic :: Utilities',
    ],
    platforms='any',
    packages=find_packages(exclude=('test',)),
    include_package_data=True,
    install_requires=[
        'docutils',
        'six',
        'Sphinx>=1.6.0,<1.6.999',
    ],
    namespace_packages=['sphinxcontrib']
)
