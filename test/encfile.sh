source execprog.sh

function encfile_bcrypt ()
{
	ifile="$1"
	ofile="$2"
	nocompress="$3"

	echo -ne "password123\npassword123\n" | execprog bcrypt -r -o $(test $nocompress && echo "-c") -s 4 "$ifile" > "$ofile"
	return $?
}

function encfile_openssl ()
{
	ifile="$1"
	ofile="$2"
	cipher="$3"
	armor="$4"

	execprog openssl "$cipher" $(test $armor && echo "-a") -salt -pass "pass:password123" -in "$ifile" -out "$ofile"
	return $?
}

function encfile_ccrypt ()
{
	ifile="$1"
	ofile="$2"

	execprog cat "$ifile" | execprog ccrypt --encrypt --quiet --brave --key "password123" > "$ofile"
	return $?
}

function encfile ()
{
	memo="$1"
	which="$2"

	echo -ne "$memo"

	eval "encfile_$which" ${@:3}

	if [ $? -ne 0 ]; then
		echo -e " [FAIL]"
		return 1
	fi

	echo -e " [OK]"
	return 0
}

if [ $# -lt 2 ]; then
	echo "Usage: $0 <memo> <type>"
	exit 1
fi

encfile "$1" "$2" ${@:3}

exit $?

