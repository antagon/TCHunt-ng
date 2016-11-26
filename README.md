# TCHunt-ng

TCHunt-ng attempts to reveal encrypted files stored on a filesystem. The program is successful in revealing **TrueCrypt/VeraCrypt** containers, **EncFS** encrypted files, **PGP/GPG** messages (and keys) and most likely other files. The code is based on ideas laid out in the project of *Stephen Judge* named *TCHunt*, hence the name. The original code has aged badly, having unnecessary dependencies and unfixed bugs; a rewrite that would allow easier portability seemed like a good idea.

TCHunt-ng is a free software licensed under **GPLv3**.

## Methodology

TCHunt-ng performs following tests against content of a file to determine if it is encrypted:

1. Test against a database of well known file-types provided by *libmagic*.
2. *Chi-squared test*.

## Usage

    Usage: tchuntng [options] <file>

    Options:
     -r  recursively traverse a directory
     -v  show version information

## Exit status

TCHunt-ng exits with one of the following exit codes:

* `0` - content of a file is *most likely* encrypted.
* `1` - a generic error occured.
* `2` - no encrypted files were found.

## Requirements

* libmagic >= 5.0

* glibc >= 2.0

## Installation

`make && sudo make install`

## Limitations

Small files (less than 256 bytes) are not tested using *Chi-squared test*. Files which are actually encrypted will therefore remain undetected, unless detected by their file-type.

## References

https://github.com/stephenjudge/TCHunt

https://en.wikipedia.org/wiki/Chi-squared_test

