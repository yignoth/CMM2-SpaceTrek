'
' GAME STATE: WarpMove
'

' Variables
dim integer xQuadCell, yQuadCell, quadMoveCells(QUAD_HEIGHT-1,QUAD_WIDTH-1)

'
'Functions
'

' init
sub _gsWarpMove.init()
local float hlth = ship.warp.getHealth(0)
local integer x,y,sx,sy
local float dmax, d

  if hlth = 0 then
    _printInfo(helmOfficer + "Warp engines have been destroyed.")
    _switchGameState("CommandInput")
  endif

  mapDisplay = MAP_DISPLAY_QUAD
  _setCmdInfo("Set QUADRANT Course~~ <ARROW> move cursor~ <ENTER> engage~ <ESC>   cancel")
  ship.pos.getQuad(0, xQuadCell, yQuadCell)

  ship.pos.getQuad(0,sx,sy)
  dmax = ship.warp.getMaxDist(0)
  for x=0 to QUAD_WIDTH-1
    for y=0 to QUAD_HEIGHT-1
      d = sqr((x-sx)*(x-sx) + (y-sy)*(y-sy))
      quadMoveCells(x,y) = (d <= dmax)
    next
  next
  
end sub


' input
sub _gsWarpMove.input()
local integer px, py

  if keyInputVal = KEY_ESC then
    _switchGameState("CommandInput")
  else
    if keyInputVal = KEY_UP and yQuadCell > 0 then
      if quadMoveCells(xQuadCell,yQuadCell-1) <> 0 then yQuadCell = yQuadCell - 1
    elseif keyInputVal = KEY_DOWN and yQuadCell < QUAD_HEIGHT-1 then
      if quadMoveCells(xQuadCell,yQuadCell+1) <> 0 then yQuadCell = yQuadCell + 1
    elseif keyInputVal = KEY_LEFT and xQuadCell > 0 then
      if quadMoveCells(xQuadCell-1,yQuadCell) <> 0 then xQuadCell = xQuadCell - 1
    elseif keyInputVal = KEY_RIGHT and xQuadCell < QUAD_WIDTH-1 then
      if quadMoveCells(xQuadCell+1,yQuadCell) <> 0 then xQuadCell = xQuadCell + 1
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
local integer px,py,x,y
  quadrant.render()

  for x=0 to QUAD_WIDTH-1
    px = x*QUAD_CELL_SX+4
    for y=0 to QUAD_HEIGHT-1
      py = y*QUAD_CELL_SY+4
      if quadMoveCells(x,y) = 0 then
        line px+1,py+QUAD_CELL_SY-2, px+QUAD_CELL_SX-2,py+1,,CLR_GRAY
        line px+1,py+1, px+QUAD_CELL_SX-2,py+QUAD_CELL_SY-2,,CLR_GRAY
      endif
    next
  next

  ' draw yellow box around current cell
  if blinkPrompt <> 0 then
    px = HUD_MAP_X0 + xQuadCell*QUAD_CELL_SX
    py = HUD_MAP_Y0 + yQuadCell*QUAD_CELL_SY
    box px+1,py+1,QUAD_CELL_SX-1,QUAD_CELL_SY-1,,CLR_YELLOW
  endif
end sub

