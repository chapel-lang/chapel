from setuptools import setup, find_packages

with open('README.rst', 'r') as fp:
    long_desc = fp.read()

setup(
    name='chpllexer',
    version='2.7.2+0',
    license='Apache License v2.0',
    author='Chapel Team',
    description='Chapel lexer for Pygments',
    long_description=long_desc,
    packages=find_packages(include=['chpllexer', 'chpllexer.*']),
    entry_points=
    """
    [pygments.lexers]
    chpllexer = chpllexer.lexer:chpllexer
    """,
)
