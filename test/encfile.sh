function encfile_bcrypt ()
{
	ifile="$1"
	ofile="$2"
	nocompress="$3"

	execprog bcrypt -r -o "$ifile" > "$ofile"
}

function encfile_openssl ()
{

}

function encfile_ccrypt ()
{

}

