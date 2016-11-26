# TCHunt-ng

TCHunt-ng attempts to reveal *possibly* encrypted files found on a filesystem. The program is successful in revealing **TrueCrypt/VeraCrypt** containers, **EncFS** encrypted files, **PGP/GPG** messages (and keys). The code is based on ideas laid out in the project of *Stephen Judge* named *TCHunt*. The original code has aged badly, having unnecessary dependencies and unfixed bugs; a rewrite that would allow easier portability seemed like a good idea.

TCHunt-ng is free software licensed under **GPLv3**.

## Methodology

TCHunt-ng performs following tests against a content of a file to determine if it is encrypted:

1. Test against a database of well known file-types provided by *libmagic*.
2. *Chi-square distribution* test.

## Usage

    Usage: tchuntng [options] <file>

    Options:
     -r  recursively traverse a directory
     -v  show version information

## Exit codes

TCHunt-ng exits with one of the following exit codes you can use to assume the end result of its operation:

* `0` a content of a file is *most likely* encrypted.
* `1` a generic error occured.
* `2` no encrypted files were found.

## Limitations

Particularly small files (less than 256 bytes) are not tested in *Chi-square distribution* test, therefore some files that are actually encrypted will be unreported.

## References

https://github.com/stephenjudge

https://github.com/stephenjudge/TCHunt

