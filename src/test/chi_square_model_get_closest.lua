-- http://lua-users.org/wiki/SimpleRound
function round(num, idp)
	local mult = 10^(idp or 0)
	return math.floor(num * mult + 0.5) / mult
end

function derive_closest (len)
	return math.pow (2, round (math.log (len) / math.log (2)))
end

local data_len = math.random (math.pow (2, 5), math.pow (2, 15))

print (("Data length: %d\nClosest to: %d"):format (data_len, derive_closest (data_len)))

