#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <magic.h>
#include <unistd.h>

#include "strolldir.h"
#include "test_magic.h"
#include "test_chidist.h"

#define TCHUNTNG_VERSION "1.0"

static void
usage (const char *p)
{
	fprintf (stdout, "Usage: %s [options] <file>\n\n\
Options:\n\
 -r  recursively traverse a directory\n\
 -v  show version information\n", p);
}

static void
version_info (void)
{
	fprintf (stdout, "TCHunt-ng %s\n", TCHUNTNG_VERSION);
}

static int
scan_dir (const char *p, const char *dirname)
{
	stroller_t dir;
	struct stroller_flist_path *path_iter;
	const struct stroller_flist *files;
	struct testmagic *testmagic;
	int exitno;

	exitno = EXIT_SUCCESS;
	testmagic = NULL;

	if ( strolldir_open (&dir, dirname) != 0 ){
		fprintf (stderr, "%s: %s\n", p, strerror (errno));
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	testmagic = testmagic_init ();

	if ( testmagic == NULL ){
		fprintf (stderr, "%s: %s\n", p, testmagic_error (testmagic));
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	do {
		if ( strolldir_scan (&dir) != 0 ){
			fprintf (stderr, "%s: '%s': %s\n", p, strolldir_getdir (&dir), strerror (errno));

			// Terminate if the error is something severe rather than access
			// permissions violation.
			if ( errno != EACCES ){
				exitno = EXIT_FAILURE;
				goto cleanup;
			}
		}

		files = strolldir_getfiles (&dir);

		for ( path_iter = files->head; path_iter != NULL; path_iter = path_iter->next ){

			switch ( testmagic_test (testmagic, path_iter->path) ){
				case -1:
					fprintf (stderr, "%s: '%s': %s\n", p, path_iter->path, strerror (errno));
					continue;

				case 0:
					// Read a next file...
					continue;

				case 1:
					// Follow up with other tests...
					break;

				case 2:
					goto test_success;
			}

			switch ( testchidist_x2 (path_iter->path) ){
				case -1:
					fprintf (stderr, "%s: '%s': %s\n", p, path_iter->path, strerror (errno));
					continue;

				case 0:
					// Read a next file...
					continue;

				case 1:
					break;
			}

test_success:
			fprintf (stdout, "%s\n", path_iter->path);
		}

	} while ( strolldir_nextdir (&dir) );

cleanup:
	strolldir_close (&dir);
	testmagic_free (testmagic);

	return exitno;
}

static int
scan_file (const char *p, const char *filename)
{
	struct testmagic *testmagic;
	int exitno;

	exitno = EXIT_SUCCESS;
	testmagic = testmagic_init ();

	if ( testmagic == NULL ){
		fprintf (stderr, "%s: %s\n", p, testmagic_error (testmagic));
		exitno = EXIT_FAILURE;
		goto cleanup;
	}

	switch ( testmagic_test (testmagic, filename) ){
		case -1:
			fprintf (stderr, "%s: '%s': %s\n", p, filename, strerror (errno));
			goto cleanup;

		case 0:
			goto cleanup;

		case 1:
			// Follow up with other tests...
			break;

		case 2:
			goto test_success;
	}

	switch ( testchidist_x2 (filename) ){
		case -1:
			fprintf (stderr, "%s: '%s': %s\n", p, filename, strerror (errno));
			goto cleanup;

		case 0:
			// Read a next file...
			goto cleanup;

		case 1:
			break;
	}

test_success:
	fprintf (stdout, "%s\n", filename);

cleanup:
	testmagic_free (testmagic);
	
	return exitno;
}

int
main (int argc, char *argv[])
{
	struct stat file_stat;
	int exitno, c, recursive, is_dir;

	exitno = EXIT_SUCCESS;
	recursive = 0;
	is_dir = 0;

	while ( (c = getopt (argc, argv, "rv")) != -1 ){
		switch ( c ){
			case 'r':
				recursive = 1;
				break;

			case 'v':
				version_info ();
				exitno = EXIT_SUCCESS;
				return exitno;

			case '?':
				exitno = EXIT_FAILURE;
				return exitno;
		}
	}

	if ( (argc - optind) == 0 ){
		usage (argv[0]);
		exitno = EXIT_FAILURE;
		return exitno;
	}

	if ( stat (argv[optind], &file_stat) == -1 ){
		fprintf (stderr, "%s: %s: %s\n", argv[0], argv[optind], strerror (errno));
		exitno = EXIT_FAILURE;
		return exitno;
	}

	switch ( file_stat.st_mode & S_IFMT ){
		case S_IFDIR:
			is_dir = 1;
			break;
	}

	if ( is_dir ){

		if ( ! recursive ){
			fprintf (stderr, "%s: ommitting directory '%s'\n", argv[0], argv[optind]);
			exitno = EXIT_FAILURE;
			return exitno;
		}

		exitno = scan_dir (argv[0], argv[optind]);
	} else {
		exitno = scan_file (argv[0], argv[optind]);
	}

	return exitno;
}

