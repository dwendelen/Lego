scriptRoot = 'scripts/'
math = require(scriptRoot .. "game_math")

TPS = 6000 --Ticks per second

--  engine.getBrick(size)
--  engine.newObject(model, position, orientation, color)
--  engine.updateObject(object, position, orientation, color)

SIZE_GRID = 0.0080;
HEIGHT_GRID = 0.0032;

MAX_CURSOR_SPEED = 0.1
MAX_CURSOR_DISPLACEMENT_PER_TICK = MAX_CURSOR_SPEED / TPS

print("disp per tick", MAX_CURSOR_DISPLACEMENT_PER_TICK)

COLORS = {
    math.color(1,1,1,1),
    math.color(1,0,0,1),
    math.color(0,1,0,1),
    math.color(0,0,1,1)
}
NB_OF_COLORS = #COLORS

CONTROL_BLOCK = 0
CHANGE_SIZE = 1

local model
local object

local cursor

local mode

function init()
    model = {
        size = math.vec(4, 2, 3),
        model = engine.getBrick(math.vec(4, 2, 3))
    }

    object = {
        id = -1,
        position = math.vec(
            math.roundTo(0, SIZE_GRID),
            math.roundTo(-0.2, HEIGHT_GRID),
            math.roundTo(-0.2, SIZE_GRID)
        ),
        orientation = math.quat(0, 0, 0, 1),
        color = 1
    }

    object.id = engine.newObject(model.model, object.position, object.orientation, COLORS[object.color])

    cursor = math.vec(
        object.position.x,
        object.position.y,
        object.position.z
    )
    mode = CONTROL_BLOCK
end

function update(input)
    for _,button in pairs(input.pressedButtons) do
        local mapping
        if mode == CONTROL_BLOCK then
            mapping = controlBlockButtons
        elseif mode == CHANGE_SIZE then
            mapping = changeSizeButtons
        end

        local func = mapping[button]
        if func ~= nil then
            func()
        end
    end

    for axis, val in pairs(input.axes) do
        if axis == AXIS_LEFTX then
            cursor.x = cursor.x + val * MAX_CURSOR_DISPLACEMENT_PER_TICK;
        elseif axis == AXIS_LEFTY then
            cursor.z = cursor.z + val * MAX_CURSOR_DISPLACEMENT_PER_TICK;
        elseif axis == AXIS_TRIGGERLEFT then
            cursor.y = cursor.y - val * MAX_CURSOR_DISPLACEMENT_PER_TICK;
        elseif axis == AXIS_TRIGGERRIGHT then
            cursor.y = cursor.y + val * MAX_CURSOR_DISPLACEMENT_PER_TICK;
        end
    end

    if mode == CONTROL_BLOCK then
        local newPos = {
            x = math.roundTo(cursor.x, SIZE_GRID),
            y = math.roundTo(cursor.y, HEIGHT_GRID),
            z = math.roundTo(cursor.z, SIZE_GRID)
        }

        if not math.equalsTable(newPos, object.position) then
            object.position = newPos
            updateObject()
        end
    elseif mode == CHANGE_SIZE then
        print("cursorX", cursor.x)
        print("cursorY", cursor.y)
        print("cursorZ", cursor.z)
        cursor = math.vec(
            math.max(1, cursor.x),
            math.max(1, cursor.y),
            math.max(1, cursor.z)
        )
        local newSize = {
            x = math.round(cursor.x/SIZE_GRID),
            y = math.round(cursor.y/HEIGHT_GRID),
            z = math.round(cursor.z/SIZE_GRID)
        }

        if not math.equalsTable(newSize, model.size) then

            model.size = newSize
            model.model = engine.getBrick(newSize)

            engine.updateModel(object.id, model.model)
        end
    end
end

function placeBlock()
    object.id = engine.newObject(model.model, object.position, object.orientation, COLORS[object.color])
end

function changeBrick()
    cursor = math.vec(
        model.size.x * SIZE_GRID,
        model.size.y * HEIGHT_GRID,
        model.size.z * SIZE_GRID
    )
    mode = CHANGE_SIZE
end

function controlBrick()
    cursor = math.vec(
        object.position.x,
        object.position.y,
        object.position.z
    )
    mode = CONTROL_BLOCK
end

function updateObject()
    engine.updateObject(object.id, object.position, object.orientation, COLORS[object.color])
end

function rotateLeft()
    rotateBrick(math.TAU_4)
end

function rotateRight()
    rotateBrick(-math.TAU_4)
end

function rotateBrick(angle)
    object.orientation = math.rotate(object.orientation, math.quatFromAngle(math.vec(0,1,0), angle))
    updateObject();
end

function previousColor()
    if(object.color == 1) then
        object.color = NB_OF_COLORS
    else
        object.color = object.color - 1
    end

    updateObject()
end
function nextColor()
    if(object.color == NB_OF_COLORS) then
        object.color = 1
    else
        object.color = object.color + 1
    end

    updateObject()
end

require (scriptRoot .. "input") -- At the end because the game functions must be defined