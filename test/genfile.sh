# Generate a plain text file.
function genfile_plaintext ()
{
	name="$1"
	text="$2"

	echo -e "$text" > "$name"

	return $?
}

# Generate a file filled with random data.
function genfile_random ()
{
	name="$1"
	size="$2"

	execprog dd if=/dev/urandom of="$name" bs=1 count="$size"

	return $?
}

# Generate TrueCrypt container.
function genfile_truecrypt ()
{
	name="$1"
	size="$2"
}

# Generate LUKS encrypted file.
function genfile_luks ()
{
	name="$1"
	size="$2"

	execprog fallocate -l "$size"M "$name"
	echo -n "nopassword" | execprog cryptsetup -q luksFormat "$name" -

	return $?
}

# Generate PGP/GPG keys (public/private).
function genfile_pgpkeys ()
{
# Keys are included in git repository, no need to generate them.
	return 0
}

function genfile ()
{
	memo="$1"
	which="$2"

	echo -ne "$memo"

	eval "genfile_$which" ${@:3}

	if [ $? -ne 0 ]; then
		echo -e " [FAIL]"
		exit 1
	fi

	echo -e " [OK]"
}

