'
' GAME STATE: ImpulseMove
'

' Variables
dim integer xSecCell, ySecCell, secMoveCells(SECTOR_WIDTH-1, SECTOR_HEIGHT-1)


' init
sub _gsImpulseMove.init()
local float hlth = ship.impl.getHealth(0)
local float dmax, d
local integer sx, sy, x, y

  mapDisplay = MAP_DISPLAY_SECTOR
  
  if hlth = 0 then
    _printInfo(helmOfficer + "Impulse engines have been destroyed.")
    _switchGameState("CommandInput")
  endif

  _setCmdInfo("Set SECTOR course~~ <ARROW> move cursor~ <ENTER> engage~~ <ESC>   cancel")
  ship.pos.getSec(0, xSecCell, ySecCell)

  ' find out where we can move
  dmax = ship.impl.getMaxDist(0)
  ship.pos.getSec(0, sx, sy)
  for x=0 to SECTOR_WIDTH-1
    for y=0 to SECTOR_HEIGHT-1
      d = sqr((x-sx)*(x-sx) + (y-sy)*(y-sy))
      secMoveCells(x,y) = (d <= dmax)
    next
  next
end sub

' input
sub _gsImpulseMove.input()
  if keyInputVal = KEY_ESC then
    _switchGameState("CommandInput")
  else
    if keyInputVal = KEY_UP and ySecCell > 0 then
      if secMoveCells(xSecCell,ySecCell-1) <> 0 then ySecCell = ySecCell - 1
    elseif keyInputVal = KEY_DOWN and ySecCell < SECTOR_HEIGHT-1 then
      if secMoveCells(xSecCell,ySecCell+1) <> 0 then ySecCell = ySecCell + 1
    elseif keyInputVal = KEY_LEFT and xSecCell > 0 then
      if secMoveCells(xSecCell-1,ySecCell) <> 0 then xSecCell = xSecCell - 1
    elseif keyInputVal = KEY_RIGHT and xSecCell < SECTOR_WIDTH-1 then
      if secMoveCells(xSecCell+1,ySecCell) <> 0 then xSecCell = xSecCell + 1
    elseif keyInputVal = KEY_ENTER then
      local integer px, py
      ship.pos.getSec(0, px, py)
      if xSecCell <> px or ySecCell <> py then
        shipMoveIdx = 0
        shipMoveX = xSecCell
        shipMoveY = ySecCell
        shipMoveTime = SHIP_MOVE_TIME
        shipMoveGetPosFunc = "ship.pos.getSec"
        shipMoveSetPosFunc = "ship.pos.setSec"
        shipNextState = "EnemyFire"
        _switchGameState("MoveShip")
      else
        _switchGameState("CommandInput")
      endif
    endif
  endif
end sub

' logic
sub _gsImpulseMove.logic(gtime%, dt%)
end sub

' render
sub _gsImpulseMove.render()
local integer px,py,x,y
  sector.render()

  for x=0 to SECTOR_WIDTH-1
    px = x*SEC_CELL_SX+4
    for y=0 to SECTOR_HEIGHT-1
      py = y*SEC_CELL_SY+4
      if secMoveCells(x,y) = 0 then
        line px+1,py+SEC_CELL_SY-2, px+SEC_CELL_SX-2,py+1,,CLR_DK_GRAY
        line px+1,py+1, px+SEC_CELL_SX-2,py+SEC_CELL_SY-2,,CLR_DK_GRAY
      endif
    next
  next
  'TODO: Gray out boxes that are to far away to move
  '      This can be precomputed in the init section

  ' render movement outline box
  if blinkPrompt <> 0 then
    _calcSectorPixelCoord(xSecCell, ySecCell, px, py)
    box px,py,SEC_CELL_SX,SEC_CELL_SY,,CLR_YELLOW
  endif
end sub

