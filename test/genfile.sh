source execprog.sh

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

	execprog dd if=/dev/urandom of="$name" bs=1 count="$size" 2>/dev/null

	return $?
}

# Generate VeraCrypt container.
function genfile_veracrypt ()
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

	return 0
}

if [ $# -lt 2 ]; then
	echo "Usage: $0 <memo> <type>"
	exit 1
fi

genfile "$1" "$2" ${@:3}

exit $?


