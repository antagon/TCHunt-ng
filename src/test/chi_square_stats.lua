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

local chi_stats = {
	min = nil,
	max = nil,
	init = false
}

function update_chi_stats (stats, num)
	if not stats.init then
		stats.min = num
		stats.max = num
		stats.init = true
		return
	end

	if num < stats.min then
		stats.min = (stats.min + num) / 2
		return
	end

	if num > stats.max then
		stats.max = (stats.max + num) / 2
		return
	end
end

----------------------------
----------------------------
----------------------------

if #arg < 1 then
	print (("Usage: %s <size>"):format (arg[0]))
	os.exit (1)
end

local rnd_source = "/dev/random"
local read_b = tonumber (arg[1])
local fd = io.open (rnd_source, "r")

if not fd then
	error ("Cannot open '" .. rnd_source .. "'")
	os.exit (1)
end

local loop_cnt = 0

while true do
	local data = fd:read (read_b)

	if not data or #data < read_b then
		print ("Cannot read data")
		break
	end

	update_chi_stats (chi_stats, chi_square (data))

	if loop_cnt == 1000 then
		print (("(%f;%f)"):format (chi_stats.min, chi_stats.max))
		loop_cnt = 0
	end

	loop_cnt = loop_cnt + 1
end

io.close (fd)

os.exit (0)

