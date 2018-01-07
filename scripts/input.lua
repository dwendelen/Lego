BUTTON_A = 0
BUTTON_B = 1
BUTTON_X = 2
BUTTON_Y = 3
BUTTON_BACK = 4
BUTTON_GUIDE = 5
BUTTON_START = 6
BUTTON_LEFTSTICK = 7
BUTTON_RIGHTSTICK = 8
BUTTON_LEFTSHOULDER = 9
BUTTON_RIGHTSHOULDER = 10
BUTTON_DPAD_UP = 11
BUTTON_DPAD_DOWN = 12
BUTTON_DPAD_LEFT = 13
BUTTON_DPAD_RIGHT = 14

AXIS_LEFTX = 0
AXIS_LEFTY = 1
AXIS_RIGHTX = 2
AXIS_RIGHTY = 3
AXIS_TRIGGERLEFT = 4
AXIS_TRIGGERRIGHT = 5

commonButtons = {
    [BUTTON_START] = engine.quit,
}

controlBlockButtons = {
    [BUTTON_A] = placeBlock,
    [BUTTON_X] = changeBrick,
    [BUTTON_LEFTSHOULDER] = rotateLeft,
    [BUTTON_RIGHTSHOULDER] = rotateRight
}

changeSizeButtons = {
    [BUTTON_A] = controlBrick,
    [BUTTON_X] = controlBrick,
    [BUTTON_LEFTSHOULDER] = previousColor,
    [BUTTON_RIGHTSHOULDER] = nextColor
}

commonButtons.__index = commonButtons
setmetatable(controlBlockButtons, commonButtons)
setmetatable(changeSizeButtons, commonButtons)