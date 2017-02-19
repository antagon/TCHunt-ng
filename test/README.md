# TCHunt-ng Test Suite

This is a test suite for TCHunt-ng capable of detecting feature regressions. It
works on a set of pre-generated files and files generated on demand.

## Dependencies

Print out the dependency list:

	$ make deps

## Usage

	$ make

## Configuration

Some of the default behavior can be overriden by specifying the proper variable
name with a new value before invocation of `make(1)`.

	$ make TCHUNTNG_BIN=../src/tchuntng-patched

To see the what is configurable, please open the Makefile in a text editor.

