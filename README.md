# TCHunt-ng

TCHunt-ng attempts to reveal encrypted files stored on a filesystem. The program is successful in finding **TrueCrypt/VeraCrypt** containers, **EncFS** encrypted files, **PGP/GPG** encrypted messages, **OpenSSH** and **PEM** private keys, password databases, and files made up of random data. The code is based on ideas laid out in the project of *Stephen Judge* named *TCHunt*, hence the name. The original code has aged badly, having unnecessary dependencies and unfixed bugs; a rewrite seemed like a good idea.

TCHunt-ng is a free software licensed under **GPLv3**.

## Methodology

TCHunt-ng performs following tests against content of a file to determine if it is encrypted:

1. Test against a database of well known file-types provided by *libmagic*.
2. The size of the analyzed file has to greater than 19 kiB and of modulo 512. This test is only enabled in *TCHunt* compatibility mode.
3. *Chi-squared test*.

## Usage

	Usage: tchuntng [options] <file>[ ...]

	Options:
	 -r  recursively traverse a directory
	 -s  show a file's classification
	 -T  enable TCHunt compatibility mode
	 -q  quietly treat no results as success
	 -x  don't cross filesystem boundaries
	 -p  preserve access time of files analyzed
	 -v  show version information

## Exit status

TCHunt-ng exits with one of the following exit codes:

* `0` - content of a file is *likely* to be encrypted.
* `1` - a generic error occured.
* `2` - content of a file is not encrypted / no encrypted files were found.
* `3` - interrupted by a signal.

## Requirements

* libmagic >= 5.0

* glibc >= 2.0

## Installation

`make && sudo make install`

## Limitations

TCHunt-ng has no way to tell apart a genuinely encrypted file and a file made up of random data.

## References

https://github.com/stephenjudge/TCHunt

https://github.com/file/file

https://en.wikipedia.org/wiki/Chi-squared_test

http://forensicswiki.org/wiki/TrueCrypt#Detection

