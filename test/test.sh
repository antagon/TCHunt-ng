source execprog.sh
source genfile.sh
source testfile.sh

##########
# Config #
##########
outdir="./samples"
plain_name="$outdir/message.txt"
rand_names=()
luks_name="$outdir/lukscontainer"
pgpkeyring_name="$outdir/gpg_keyring.gpg"
pgppubkey_name="$outdir/gpg_pubkey"
pgpseckey_name="$outdir/gpg_secretkey"

function cleanup ()
{
	echo "Cleaning up..."

	rm -f "$plain_name"\
		"$luks_name"\
		${rand_names[*]}
}

trap cleanup EXIT

if [ ! -d "$outdir" ]; then
	mkdir -p "$outdir"
fi

genfile "Generating a plain text file '$plain_name' ..." "plaintext" "$plain_name" "There is no perfection only life."

# TODO: store random generated filenames in array.
for i in $(seq 1 14); do
	tmpname="$(mktemp -p "$outdir")"
        rand_names+=("$tmpname")
	size="$(echo "2 ^ $i" | bc)"

	genfile "Generating a random file '$tmpname' ..." "random" "$tmpname" "$size" 2>/dev/null
done

genfile "Generating a LUKS container '$luks_name' ..." "luks" "$luks_name" "14"
genfile "Generating a GPG key pair ..." "pgpkeys" "$pgpkeyring_name" "$pgppubkey_name" "$pgpseckey_name"

#
## Test files
#
testfile "Testing '$plain_name' ... " $TESTFILE_NOMATCH -p "$plain_name"

for f in ${rand_names[*]}; do
	testfile "Testing '$f' ... " $TESTFILE_MATCH -p "$f"
done

testfile "Testing '$luks_name' ... " $TESTFILE_MATCH -p "$luks_name"
testfile "Testing '$pgpkeyring_name' ... " $TESTFILE_MATCH -p "$pgpkeyring_name"
testfile "Testing '$pgppubkey_name'.asc ... " $TESTFILE_MATCH -p "$pgppubkey_name".asc
testfile "Testing '$pgpseckey_name'.asc ... " $TESTFILE_MATCH -p "$pgpseckey_name".asc
testfile "Testing '$pgppubkey_name'.dat ... " $TESTFILE_MATCH -p "$pgppubkey_name".dat
testfile "Testing '$pgpseckey_name'.dat ... " $TESTFILE_MATCH -p "$pgpseckey_name".dat

exit 0

