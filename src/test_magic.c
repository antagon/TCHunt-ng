#include <stdio.h> // FIXME
#include <stdlib.h>
#include <string.h>
#include <magic.h>
#include <errno.h>

#include "test_magic.h"

#define MAGIC_FLAGS (MAGIC_NO_CHECK_APPTYPE | MAGIC_SYMLINK /*| MAGIC_NO_CHECK_ASCII*/ | MAGIC_NO_CHECK_COMPRESS | MAGIC_NO_CHECK_ELF | MAGIC_NO_CHECK_FORTRAN /*| MAGIC_NO_CHECK_SOFT*/ | MAGIC_NO_CHECK_TAR | MAGIC_NO_CHECK_TOKENS | MAGIC_NO_CHECK_TROFF)

struct testmagic*
testmagic_init (void)
{
	struct testmagic *new_test;

	new_test = calloc (1, sizeof (struct testmagic));

	if ( new_test == NULL )
		return NULL;

	new_test->magic_res = magic_open (MAGIC_FLAGS);

	if ( new_test->magic_res == NULL )
		return NULL;

	if ( magic_load (new_test->magic_res, NULL) == -1 )
		return NULL;

	return new_test;
}

int
testmagic_is_datafile (struct testmagic *testmagic, const char *file)
{
	const char *ftype;

	ftype = magic_file (testmagic->magic_res, file);

	if ( ftype == NULL )
		return -1;

#if 0
	fprintf (stderr, "%s : %s\n", file, ftype);
#endif

	if ( strcmp (ftype, "data") == 0 )
		return 1;

	return 0;
}

void
testmagic_free (struct testmagic *testmagic)
{

}

const char*
testmagic_error (struct testmagic *testmagic)
{
	if ( errno != 0 )
		return strerror (errno);

	return magic_error (testmagic->magic_res);
}

