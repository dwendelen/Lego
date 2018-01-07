local math = require "math"

local function vec(x, y, z)
    return {
        x = x,
        y = y,
        z = z
    }
end

local function vec_lengthSq(vec)
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z
end

local function vec_length(vec)
    return math.sqrt(vec_lengthSq(vec))
end

local function vec_norm(v)
    local l = vec_length(v)

    if l == 0 then
        return v
    end

    return vec(v.x / l, v.y / l, v.z / l)
end

local function quat(x, y, z, w)
    return {
        x = x,
        y = y,
        z = z,
        w = w
    }
end

local function quatFromAngle(axis, angle)
    if vec_lengthSq(axis) == 0 then
        return quat(0, 0, 0, 1)
    end

    local unit = vec_norm(axis)
    local sinHalfAngle = math.sin(angle * 0.5)

    return quat(
        unit.x * sinHalfAngle,
        unit.y * sinHalfAngle,
        unit.z * sinHalfAngle,
        math.cos(angle * 0.5)
    )
end

local function rotate(orientation, rotation)
    local a = orientation
    local b = rotation

    return quat(
        a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
        a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x,
        a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w,
        a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z
    )
end

local function color(r, g, b, a)
    return {
        r = r,
        g = g,
        b = b,
        a = a
    }
end

local function round(a)
    return math.floor(a + 0.5)
end

local function roundTo(a, b)
    return round(a / b) * b;
end

local function equalsTable(table1, table2)
    if #table1 ~= #table2 then
        return false
    end

    for k,v in pairs(table1) do
        if table2[k] ~= v then
            return false
        end
    end
    return true
end

export_meta = {
    __index = math
}

export = {
    TAU_4 = math.pi / 2,
    vec = vec,
    quat = quat,
    quatFromAngle = quatFromAngle,
    color = color,
    rotate = rotate,
    roundTo = roundTo,
    round = round,
    equalsTable = equalsTable
}

setmetatable(export, {__index = math})

return export