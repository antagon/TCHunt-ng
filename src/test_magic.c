/*
 * TCHunt-ng - reveal (possibly) encrypted files stored on a filesystem.
 * Copyright (C) 2016, 2017  CodeWard.org
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#if 0
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <magic.h>
#include <errno.h>

#include "test_magic.h"

static const char *testmagic_cattype[_TMAGIC_CAT_EOF - 1] = {
	"data", "data", "key", "password"
};

static struct testmagic_filetype
{
	const char *str_id;
	int cat;
} testmagic_filetypedef[] = {
	{ "data", TMAGIC_CAT_DATA },
	{ "PGP public key block", TMAGIC_CAT_KEY },
	{ "PGP message", TMAGIC_CAT_ENCDATA },
	{ "PGP\011Secret Key", TMAGIC_CAT_KEY },
	{ "GPG symmetrically encrypted data", TMAGIC_CAT_ENCDATA },
	{ "GPG encrypted data", TMAGIC_CAT_ENCDATA },
	{ "PGP symmetric key encrypted data", TMAGIC_CAT_ENCDATA },
	{ "PGP encrypted data", TMAGIC_CAT_ENCDATA },
	{ "PGP RSA encrypted session key", TMAGIC_CAT_ENCDATA },
	{ "OpenSSH", TMAGIC_CAT_KEY },
	{ "PEM RSA private key", TMAGIC_CAT_KEY },
	{ "PEM DSA private key", TMAGIC_CAT_KEY },
	{ "PEM EC private key", TMAGIC_CAT_KEY },
	{ "Keepass password database", TMAGIC_CAT_PASS },
	{ "Password Safe V3 database", TMAGIC_CAT_PASS },
	{ "cvs password text file", TMAGIC_CAT_PASS },
	{ "LUKS encrypted file", TMAGIC_CAT_ENCDATA },
	{ "Chiasmus encrypted data", TMAGIC_CAT_ENCDATA },
	{ "Chiasmus key", TMAGIC_CAT_KEY },
	{ "mcrypt", TMAGIC_CAT_ENCDATA },
	{ "GNOME keyring", TMAGIC_CAT_PASS },
	{ "Mac OS X Keychain File", TMAGIC_CAT_PASS },
	{ "Vim encrypted file data", TMAGIC_CAT_ENCDATA },
	{ "Encore", TMAGIC_CAT_DATA }
};

int
testmagic_init (struct testmagic *testmagic, int flags)
{
	testmagic->magic_res = magic_open (flags);

	if ( testmagic->magic_res == NULL )
		return -1;

	if ( magic_load (testmagic->magic_res, NULL) == -1 )
		return -1;


#ifdef MAGIC_PARAM_NAME_MAX
	/* Increate the default value as some files fail to be checked
	 * successfully. */
	size_t val;

	val = 99;

	if ( magic_setparam (testmagic->magic_res, MAGIC_PARAM_NAME_MAX, &val) == -1 )
		return -1;
#endif

	return 0;
}

#define MATCHES(str, predicate) (strncmp ((str), (predicate), strlen ((predicate))) == 0)

int
testmagic_test (struct testmagic *testmagic, const char *file, const char **cat_type)
{
	const char *ftype;
	int match, i;

	ftype = magic_file (testmagic->magic_res, file);

	if ( ftype == NULL )
		return -1;

	match = 0;

	for ( i = 0; i < (sizeof (testmagic_filetypedef) / sizeof (testmagic_filetypedef[0])); i++ ){
		if ( MATCHES (ftype, testmagic_filetypedef[i].str_id) ){
			match = testmagic_filetypedef[i].cat;
			break;
		}
	}

	if ( match > 0 && cat_type != NULL )
		*cat_type = testmagic_cattype[match - 1];

#if 0
	fprintf (stderr, "TESTMAGIC_DBG: %s (%s) --> %d\n", file, ftype, match);
#endif

	return match;
}

void
testmagic_free (struct testmagic *testmagic)
{
	if ( testmagic == NULL )
		return;

	if ( testmagic->magic_res == NULL )
		return;

	magic_close (testmagic->magic_res);
}

const char*
testmagic_error (struct testmagic *testmagic)
{
	if ( testmagic == NULL )
		return NULL;

	if ( testmagic->magic_res == NULL )
		return NULL;

	return magic_error (testmagic->magic_res);
}

