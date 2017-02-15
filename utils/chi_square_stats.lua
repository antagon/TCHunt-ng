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

function chi_square (buff)
	local expected = #buff / 256
	local chi = 0.0

	for idx, cnt in pairs (map_char_counts (buff)) do
		--print (("[%02x] %d"):format (idx, cnt))
		chi = chi + (math.pow (cnt - expected, 2) / expected)
	end

	return chi
end

function update_chi_stats (stats, num)
	if not stats.init then
		stats.min = num
		stats.max = num
		stats.init = true
		return true
	end

	if num < stats.min then
		stats.min = num
		return true
	end

	if num > stats.max then
		stats.max = num
		return true
	end

	return false
end

----------------------------
----------------------------
----------------------------
local chunks = {
	{ len = 32, stats = { min = 196.000000, max = 706.750000, init = true }},
	{ len = 64, stats = { min = 144.000000, max = 500.250000, init = true }},
	{ len = 128, stats = { min = 85.000000, max = 389.249832, init = true }},
	{ len = 256, stats = { min = 76.000000, max = 343.000000, init = true }},
	{ len = 512, stats = { min = 104.000000, max = 330.000000, init = true }},
	{ len = 1024, stats = { min = 130.500000, max = 393.000000, init = true }},
	{ len = 2048, stats = { min = 140.750000, max = 402.750000, init = true }},
	{ len = 4096, stats = { min = 149.000000, max = 409.250000, init = true }},
	{ len = 8192, stats = { min = 146.312500, max = 400.125000, init = true }},
	{ len = 16384, stats = { min = 154.156250, max = 403.250000, init = true }}
}
local rnd_source = "/dev/urandom"
local poolsize_b = math.pow (1024, 3) * 100
local read_b = math.pow (2, 24)

local fd = io.open (rnd_source, "r")

if not fd then
	error ("Cannot open '" .. rnd_source .. "'")
	os.exit (1)
end

for i = 0, (poolsize_b / read_b) do
	print (("%s %d/%d"):format (os.date (), i, (poolsize_b / read_b)))

	local data = fd:read (read_b)

	if not data or #data < read_b then
		print ("Cannot read data")
		break
	end

	for _, chunk in ipairs (chunks) do
		local printme = false

		for i = 1, read_b, chunk.len do
			if update_chi_stats (chunk.stats, chi_square (data:sub (i, i + chunk.len - 1))) then
				printme = true
			end
		end

		if printme then
			print (("{ %d, %f, %f },"):format (chunk.len, chunk.stats.min, chunk.stats.max))
		end
	end
end

print ("DONE!")

for _, chunk in ipairs (chunks) do
	print (("{ %d, %f, %f },"):format (chunk.len, chunk.stats.min, chunk.stats.max))
end

io.close (fd)

os.exit (0)

