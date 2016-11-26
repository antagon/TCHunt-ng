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
		print (("[%02x] %d"):format (idx, cnt))

		chi = chi + (math.pow (cnt - expected, 2) / expected)
	end

	return chi
end

---------------------------------------------------

--local buff = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
--local buff = "\x00\x00\x00"
local buff = string.rep ("\x00", 1024 * 13)

print (("length: %d, chi: %f"):format (
			#buff,
			chi_square (buff)
))

