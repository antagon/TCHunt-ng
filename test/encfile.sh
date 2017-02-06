function encfile_bcrypt ()
{
	ifile="$1"
	ofile="$2"
	nocompress="$3"

	echo -ne "password123\npassword123\n" | execprog bcrypt -r -o $(test $nocompress && echo "-c") -s 4 "$ifile" > "$ofile"
}

function encfile_openssl ()
{
	return 1
}

function encfile_ccrypt ()
{
	return 1
}

function encfile ()
{
	memo="$1"
	which="$2"

	echo -ne "$memo"

	eval "encfile_$which" ${@:3}

	if [ $? -ne 0 ]; then
		echo -e " [FAIL]"
		exit 1
	fi

	echo -e " [OK]"
}

