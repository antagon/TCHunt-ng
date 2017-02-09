function execprog ()
{
	p="$(which "$1" 2>/dev/null)"

	if [ ! -x "$p" ]; then
		(>&2 echo "no executable file '$1'")
		exit 1
	fi

	"$p" ${@:2}
	return $?
}

