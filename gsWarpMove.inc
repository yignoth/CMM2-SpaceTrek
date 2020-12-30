'
' GAME STATE: WarpMove
'

' Variables
dim integer xQuadCell, yQuadCell

'
'Functions
'

' init
sub _gsWarpMove.init()
  mapDisplay = MAP_DISPLAY_QUAD
  _setCmdInfo("Set QUADRANT Course~~ <ARROW> move cursor~ <ENTER> engage~ <ESC>   cancel")
  ship.pos.getQuad(0, xQuadCell, yQuadCell)
end sub


' input
sub _gsWarpMove.input()
local integer px, py

  if keyInputVal = KEY_ESC then
    _switchGameState("CommandInput")
  else
    if keyInputVal = KEY_UP and yQuadCell > 0 then
      yQuadCell = yQuadCell - 1
    elseif keyInputVal = KEY_DOWN and yQuadCell < QUAD_HEIGHT-1 then
      yQuadCell = yQuadCell + 1
    elseif keyInputVal = KEY_LEFT and xQuadCell > 0 then
      xQuadCell = xQuadCell - 1
    elseif keyInputVal = KEY_RIGHT and xQuadCell < QUAD_WIDTH-1 then
      xQuadCell = xQuadCell + 1
    elseif keyInputVal = KEY_ENTER then
      ship.pos.getQuad(0, px, py)
      if xQuadCell <> px or yQuadCell <> py then
        shipMoveIdx = 0
        shipMoveX = xQuadCell
        shipMoveY = yQuadCell
        shipMoveTime = SHIP_MOVE_TIME
        shipMoveGetPosFunc = "ship.pos.getQuad"
        shipMoveSetPosFunc = "ship.pos.setQuad"
        shipNextState = "EnterSector"
        _switchGameState("MoveShip")
      else
        _switchGameState("CommandInput")
      endif
    endif
  endif
end sub


' logic
sub _gsWarpMove.logic(gtime%, dt%)
end sub


' render
sub _gsWarpMove.render()
local integer px,py
  quadrant.render()

  'TODO: Gray out boxes that are to far away to move
  '      This can be precomputed in the init section

  ' draw yellow box around current cell
  if blinkPrompt <> 0 then
    px = HUD_MAP_X0 + xQuadCell*QUAD_CELL_SX
    py = HUD_MAP_Y0 + yQuadCell*QUAD_CELL_SY
    box px+1,py+1,QUAD_CELL_SX-1,QUAD_CELL_SY-1,,CLR_YELLOW
  endif
end sub

