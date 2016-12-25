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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <magic.h>
#include <errno.h>

#include "test_magic.h"

#define MAGIC_FLAGS (MAGIC_NO_CHECK_APPTYPE | MAGIC_SYMLINK | MAGIC_NO_CHECK_COMPRESS | MAGIC_NO_CHECK_ELF | MAGIC_NO_CHECK_FORTRAN | MAGIC_NO_CHECK_TAR | MAGIC_NO_CHECK_TOKENS | MAGIC_NO_CHECK_TROFF)

static const char *testmagic_cattype[_TMAGIC_CAT_EOF - 1] = {
	"data", "ciphertext", "key", "password"
};

int
testmagic_init (struct testmagic *testmagic)
{
	testmagic->magic_res = magic_open (MAGIC_FLAGS);

	if ( testmagic->magic_res == NULL )
		return -1;

	if ( magic_load (testmagic->magic_res, NULL) == -1 )
		return -1;

	return 0;
}

#define MATCHES(str, predicate) (strncmp ((str), (predicate), strlen ((predicate))) == 0)

int
testmagic_test (struct testmagic *testmagic, const char *file, const char **cat_type)
{
	const char *ftype;
	int match;

	ftype = magic_file (testmagic->magic_res, file);

	if ( ftype == NULL ){
		if ( errno == ENODATA || errno == ENOENT || errno == EIO )
			return 0;
		return -1;
	}

	match = 0;

	if ( MATCHES (ftype, "data") )
		match = TMAGIC_CAT_DATA;
	else if ( MATCHES (ftype, "PGP public key block") )
		match = TMAGIC_CAT_KEY;
	else if ( MATCHES (ftype, "PGP message") )
		match = TMAGIC_CAT_ENCDATA;
	else if ( MATCHES (ftype, "PGP\011Secret Key") )
		match = TMAGIC_CAT_KEY;
	else if ( MATCHES (ftype, "GPG symmetrically encrypted data") )
		match = TMAGIC_CAT_ENCDATA;
	else if ( MATCHES (ftype, "GPG encrypted data") )
		match = TMAGIC_CAT_ENCDATA;
	else if	( MATCHES (ftype, "PGP symmetric key encrypted data") )
		match = TMAGIC_CAT_ENCDATA;
	else if ( MATCHES (ftype, "PGP encrypted data") )
		match = TMAGIC_CAT_ENCDATA;
	else if ( MATCHES (ftype, "PGP RSA encrypted session key") )
		match = TMAGIC_CAT_ENCDATA;
	else if ( MATCHES (ftype, "OpenSSH") )
		match = TMAGIC_CAT_KEY;
	else if ( MATCHES (ftype, "PEM RSA private key") )
		match = TMAGIC_CAT_KEY;
	else if ( MATCHES (ftype, "PEM DSA private key") )
		match = TMAGIC_CAT_KEY;
	else if ( MATCHES (ftype, "PEM EC private key") )
		match = TMAGIC_CAT_KEY;
	else if ( MATCHES (ftype, "Keepass password database") )
		match = TMAGIC_CAT_PASS;
	else if ( MATCHES (ftype, "Password Safe V3 database") )
		match = TMAGIC_CAT_PASS;
	else if ( MATCHES (ftype, "cvs password text file") )
		match = TMAGIC_CAT_PASS;

	if ( match > 0 && cat_type != NULL )
		*cat_type = testmagic_cattype[match - 1];

#if 0
	fprintf (stderr, "TESTMAGIC_DBG: %s (%s)\n", file, ftype);
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

