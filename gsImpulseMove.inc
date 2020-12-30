'
' GAME STATE: ImpulseMove
'

' Variables
dim integer xSecCell, ySecCell


' init
sub _gsImpulseMove.init()
  mapDisplay = MAP_DISPLAY_SECTOR
  _setCmdInfo("Set SECTOR course~~ <ARROW> move cursor~ <ENTER> engage~~ <ESC>   cancel")

  ship.pos.getSec(0, xSecCell, ySecCell)
end sub

' input
sub _gsImpulseMove.input()
  if keyInputVal = KEY_ESC then
    _switchGameState("CommandInput")
  else
    if keyInputVal = KEY_UP and ySecCell > 0 then
      ySecCell = ySecCell - 1
    elseif keyInputVal = KEY_DOWN and ySecCell < SECTOR_HEIGHT-1 then
      ySecCell = ySecCell + 1
    elseif keyInputVal = KEY_LEFT and xSecCell > 0 then
      xSecCell = xSecCell - 1
    elseif keyInputVal = KEY_RIGHT and xSecCell < SECTOR_WIDTH-1 then
      xSecCell = xSecCell + 1
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
        shipNextState = "CommandInput"
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
local integer px,py
  sector.render()

  'TODO: Gray out boxes that are to far away to move
  '      This can be precomputed in the init section

  ' render movement outline box
  if blinkPrompt <> 0 then
    _calcSectorPixelCoord(xSecCell, ySecCell, px, py)
    box px,py,SEC_CELL_SX,SEC_CELL_SY,,CLR_YELLOW
  endif
end sub

