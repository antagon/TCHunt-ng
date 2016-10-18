#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <magic.h>

#include "strolldir.h"
#include "test_magic.h"

static void
usage (const char *p)
{
	fprintf (stdout, "Usage: %s <directory>\n\n\
Recursively traverse a directory looking for encrypted files.\n", p);
}

int
main (int argc, char *argv[])
{
	stroller_t dir;
	const struct stroller_flist *files;
	struct stroller_flist_path *path_iter;
	struct testmagic *testmagic;
	int exitno, test_result, rval;

	exitno = EXIT_SUCCESS;
	test_result = 0;

	if ( argc < 2 ){
		usage (argv[0]);
		exitno = EXIT_FAILURE;
		return exitno;
	}

	if ( strolldir_open (&dir, argv[1]) != 0 ){
		fprintf (stderr, "%s: %s\n", argv[0], strerror (errno));
		exitno = EXIT_FAILURE;
		return exitno;
	}

	// Init tests...
	testmagic = testmagic_init ();

	if ( testmagic == NULL ){
		fprintf (stderr, "%s: %s\n", argv[0], testmagic_error (testmagic));
		exitno = EXIT_FAILURE;
		return exitno;
	}

	do {
		if ( strolldir_scan (&dir) != 0 ){
			fprintf (stderr, "%s: '%s': %s\n", argv[0], strolldir_getdir (&dir), strerror (errno));

			if ( errno != EACCES ){
				exitno = EXIT_FAILURE;
				break;
			}
		}

		files = strolldir_getfiles (&dir);

		for ( path_iter = files->head; path_iter != NULL; path_iter = path_iter->next ){
			switch ( testmagic_is_datafile (testmagic, path_iter->path) ){
				case -1:
					fprintf (stderr, "%s: '%s': %s\n", argv[0], strolldir_getdir (&dir), testmagic_error (testmagic));
					break;

				case 0:
					break;

				case 1:
					test_result &= 0x01;
					fprintf (stderr, "%s\n", path_iter->path);
					break;
			}
		}

	} while ( strolldir_nextdir (&dir) );

cleanup:
	strolldir_close (&dir);
	testmagic_free (testmagic);

	return exitno;
}

