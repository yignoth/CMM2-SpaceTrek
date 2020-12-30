'
' GAME STATE: MoveShip
'

' Constants
const SHIP_MOVE_TIME=500


' Variables
dim integer shipMoveX, shipMoveY
dim integer shipMoveIdx, shipMoveTime
dim integer shipMovePosX, shipMovePosY
dim string shipMoveGetPosFunc, shipMoveSetPosFunc
dim string shipNextState


'
'Functions
'

' init
sub _gsMoveShip.init()
local integer dx, dy, h
local string fn$

    _setCmdPrompt("Course Enganged ")
    _setCmdInfo("")

    ' set ships heading
    call shipMoveGetPosFunc, shipMoveIdx, dx, dy

    dx = shipMoveX - dx
    dy = shipMoveY - dy
    if (abs(dx) > abs(dy)) then
      h = _tern.i(dx > 0, 3, 1)
    else
      h = _tern.i(dy >= 0, 2, 0)
    endif

    ship.pos.setHead(shipMoveIdx, h)
end sub


' input
sub _gsMoveShip.input()
end sub


' logic
sub _gsMoveShip.logic(gtime%, dt%)
local integer px, py, flag = 0

  shipMoveTime = shipMoveTime - dt%

  if shipMoveTime < 0 then
    shipMoveTime = SHIP_MOVE_TIME

    call shipMoveGetPosFunc, shipMoveIdx, px, py

    if px < shipMoveX then
      inc px, 1
    elseif px > shipMoveX then
      inc px, -1
    else
      inc flag
    endif

    if py < shipMoveY then
      inc py, 1
    elseif py > shipMoveY then
      inc py, -1
    else
      inc flag
    endif

    call shipMoveSetPosFunc, 0, px, py

    if flag = 2 then
      _switchGameState(shipNextState)
    endif
  endif
end sub


' render
sub _gsMoveShip.render()
  if mapDisplay = MAP_DISPLAY_SECTOR then
    sector.render()
  elseif mapDisplay = MAP_DISPLAY_QUAD then
    quadrant.render()
  endif
end sub

