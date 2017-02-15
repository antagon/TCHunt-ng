# TCHunt-ng

TCHunt-ng attempts to reveal encrypted files stored on a filesystem. The program is successful in finding ***TrueCrypt***, ***VeraCrypt***, ***CipherShed*** containers; ***LUKS***, ***EncFS***, ***PGP/GPG*** encrypted files; ***OpenSSH*** and ***PEM*** private keys; password databases; files made up of random data. The code is based on ideas laid out in the project of Stephen Judge named *TCHunt*, hence the name. The original code has aged badly, having unnecessary dependencies and unfixed bugs. A rewrite seemed like a good idea.

TCHunt-ng is a free software licensed under **GPLv3**.

## Methodology

TCHunt-ng performs following tests against content of a file to determine if it is of interest:

1. Test against a database of well known file-types provided by *libmagic*.
2. Test the size of a file to be greater than 19 kiB and of modulo 512. The test is performed only in *TCHunt compatibility mode*.
3. *Chi-squared test*.

## Usage

	Usage: tchuntng [options] <file>...

	Options:
	 -p  preserve access time of files analyzed
	 -q  quietly treat no result as success
	 -s  show a file's classification
	 -T  enable TCHunt compatibility mode
	 -v  show version information
	 -h  show usage information

## Environment

The behavior of TCHunt-ng is affected by the following environment variables.

The environment variable MAGIC can be used to override default magic file name. Please note that doing so may affect other programs that rely on *libmagic*.

## Exit status

TCHunt-ng exits with one of the following exit codes:

* `0` - content of a file is *likely* to be encrypted.
* `1` - a generic error occured.
* `2` - content of a file is not encrypted.
* `3` - interrupted by a signal.

## Dependencies

* libmagic >= 5.0

* glibc >= 2.0

## Installation

On Ubuntu:

	sudo apt-get install libmagic-dev

On Fedora:

	sudo dnf install file-devel

Compile the source code and install the executable:

	make && sudo make install

## Limitations

TCHunt-ng has no way to tell apart a genuinely encrypted file and a file made up of random data. Files smaller than 32 bytes, unless recognized by its type, are ignored.

## References

https://github.com/stephenjudge/TCHunt

https://github.com/file/file

https://en.wikipedia.org/wiki/Chi-squared_test

http://forensicswiki.org/wiki/TrueCrypt#Detection

