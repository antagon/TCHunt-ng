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
	local max = 14
	local exp = math.log (num) / math.log (2)

	if exp < min or exp > max then
		return -1
	end

	return exp - min
end

local model = {
	[0] = { len = math.pow (2, 5), min = 0.0, max = 0.0 },
	[1] = { len = math.pow (2, 6), min = 0.0, max = 0.0 },
	[2] = { len = math.pow (2, 7), min = 0.0, max = 0.0 },
	[3] = { len = math.pow (2, 8), min = 0.0, max = 0.0 },
	[4] = { len = math.pow (2, 9), min = 0.0, max = 0.0 },
	[5] = { len = math.pow (2, 10), min = 0.0, max = 0.0 },
	[6] = { len = math.pow (2, 11), min = 0.0, max = 0.0 },
	[7] = { len = math.pow (2, 12), min = 0.0, max = 0.0 },
	[8] = { len = math.pow (2, 13), min = 0.0, max = 0.0 },
	[9] = { len = math.pow (2, 14), min = 0.0, max = 0.0 }
}

math.randomseed (os.time ())
local data_len = math.random (math.pow (2, 5), math.pow (2, 14))
local closest = derive_closest (data_len)
local index = derive_cindex (closest)

print (("Data length: %d\nClosest to: %d\nArray index: %d => { %d, %f, %f }"):format (data_len, closest, index, model[index].len, model[index].min, model[index].max))

