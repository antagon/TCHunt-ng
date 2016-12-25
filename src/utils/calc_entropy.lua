require ("math")

math.log2 = function (num)
	return math.log (num) / math.log (2)
end

function map_char_counts (buff)
	local map = {}

	for i = 1, #buff do
		local b = buff:byte (i)

		if map[b] then
			map[b] = map[b] + 1
		else
			map[b] = 1
		end
	end

	return map
end

function shannon_entropy (buff)
	local res = 0.0

	for idx, cnt in pairs (map_char_counts (buff)) do
		freq = (cnt / #buff)
		res = res + (freq * math.log2 (freq))
	end

	return math.abs (res)
end

function chi_square (buff)
	local expected = #buff / 256
	local chi = 0.0

	for idx, cnt in pairs (map_char_counts (buff)) do
		chi = chi + (math.pow (cnt - expected, 2) / expected)
	end

	return chi
end

---------------------------------------------------

local file = arg[1]
local len = arg[2]

if #arg == 1 then
	if tonumber (file) ~= nil then
		len = file
		file = nil
	end
end

if not file then
	file = "/dev/urandom"
end

if not len or len == 0 then
	len = math.pow (2, 14)
end

local fd, errmsg = io.open (file, "rb")

if not fd then
	io.stderr:write (arg[0] .. ": cannot open file: " .. errmsg .. "\n")
	os.exit (1)
end

local buff = fd:read (tonumber (len))

if not buff then
	os.exit (2)
end

io.stdout:write (("file: '%s', length: %d, entropy: %f, chidist: %f\n"):format (
			file, #buff, shannon_entropy (buff), chi_square (buff)
))

os.exit (0)

