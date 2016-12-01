-- http://lua-users.org/wiki/SimpleRound
function round(num, idp)
	local mult = 10^(idp or 0)
	return math.floor(num * mult + 0.5) / mult
end

function derive_closest (len)
	return math.pow (2, round (math.log (len) / math.log (2)))
end

function derive_cindex (num)
	-- Powers of 2
	local min = 5
	local max = 15
	local exp = math.log (num) / math.log (2)

	if exp < min or exp > max then
		return -1
	end

	return exp - min
end

local model = {
	{ len = math.pow (2, 5), min = nil, max = nil },
	{ len = math.pow (2, 6), min = nil, max = nil },
	{ len = math.pow (2, 7), min = nil, max = nil },
	{ len = math.pow (2, 8), min = nil, max = nil },
	{ len = math.pow (2, 9), min = nil, max = nil },
	{ len = math.pow (2, 10), min = nil, max = nil },
	{ len = math.pow (2, 11), min = nil, max = nil },
	{ len = math.pow (2, 12), min = nil, max = nil },
	{ len = math.pow (2, 13), min = nil, max = nil },
	{ len = math.pow (2, 14), min = nil, max = nil },
	{ len = math.pow (2, 15), min = nil, max = nil }
}

local data_len = math.random (math.pow (2, 5), math.pow (2, 15))
local closest = derive_closest (data_len)
local index = derive_cindex (closest)

print (("Data length: %d\nClosest to: %d\nArray index: %d"):format (data_len, closest, index))

