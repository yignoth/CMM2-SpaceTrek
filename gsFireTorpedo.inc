'
' GAME STATE: FireTorpedo
'

' Constants


' Variables
dim float __xTorp, __yTorp, __dxTorp, __dyTorp
dim integer __x0Torp, __y0Torp


'
' FT: select target
'
sub _gsFireTorpedo.init()
  mapDisplay = MAP_DISPLAY_SECTOR
  cmdText$ = ""

  if numEnemies = 0 then
    _printInfo(weaponsOfficer + "No enemies detected in sector.")
    _switchGameState("CommandInput")
  elseif ship.torp.getValue(0) = 0 then
    _printInfo(weaponsOfficer + "No torpedoes in armory.")
    _switchGameState("CommandInput")
  else
    local integer i, px, py
    _setCmdInfo("Select enemy target.~~ <ARROW> move cursor~ <SPACE> target enemy~ <ENTER> fire torpedo~~ <ESC> cancel")
    secTargetIdx = 1

    for i=0 to numEnemies
      ship.pos.getSec(i, px, py)
      secTarget(0,i) = HUD_MAP_X0 + px*SEC_CELL_SX
      secTarget(1,i) = HUD_MAP_Y0 + py*SEC_CELL_SY
      secTarget(2,i) = -1
    next
  endif
end sub

sub _gsFireTorpedo.input()
  if keyInputVal = KEY_ESC then
    _printInfo(weaponsOfficer + "Torpedo order canceled")
    _switchGameState("CommandInput")
  elseif keyInputVal = KEY_ENTER then
    local integer i
    numSecTargets = 0
    for i=1 to numEnemies
      if secTarget(2,i) >= 0 then
        inc numSecTargets
        _swapArrayPlaces2(i,numSecTargets,secTarget(),2)
      endif
    next
    if numSecTargets = 0 then
      _printInfo(weaponsOfficer + "No enemies targeted")
      _switchGameState("CommandInput")
    else
      _switchGameState("FireTorpedo2")
    endif
  elseif keyInputVal = KEY_UP or keyInputVal = KEY_LEFT then
    inc secTargetIdx, -1
    if (secTargetIdx < 1) then secTargetIdx = numEnemies
  elseif keyinputVal = KEY_DOWN or keyInputVal = KEY_RIGHT then
    inc secTargetIdx
    if (secTargetIdx > numEnemies) then secTargetIdx = 1
  elseif keyInputVal = KEY_SPACE then
    local integer i
    for i=0 to numEnemies
      secTarget(2,i) = -1
    next
    secTarget(2,secTargetIdx) = _tern.i(secTarget(2,secTargetIdx)=-1,secTargetIdx,-1)
  endif
end sub

sub _gsFireTorpedo.logic(gtime%, dt%)
end sub

sub _gsFireTorpedo.render()
local integer i, cnt
  sector.render()

  for i=1 to numEnemies
    if secTarget(2,i) > 0 then
      box secTarget(0,i),secTarget(1,i), SEC_CELL_SX,SEC_CELL_SY,, CLR_RED
    endif
  next

  if blinkPrompt <> 0 then
    box secTarget(0,secTargetIdx)+1,secTarget(1,secTargetIdx)+1, SEC_CELL_SX-2,SEC_CELL_SY-2,, CLR_YELLOW
  endif
end sub


'
' FT2: fire torpedo
'
sub _gsFireTorpedo2.init()
local integer sx(3) = (7,4,7,10), sy(3) = (4,7,10,7)

  _printInfo(weaponsOfficer + "Firing torpedo!")
  _setCmdPrompt("")
  _setCmdInfo("")
  secTargetIdx = 1
  phasorsFiring = 0

  ' reduce number of torpedos
  ship.torp.setValue(0, ship.torp.getValue(0) - 1)

  ' setup torpedo animation
  local integer h = ship.pos.getHead(0)
  __x0Torp = secTarget(0,0) + sx(h)
  __y0Torp = secTarget(1,0) + sy(h)
  __dxTorp = secTarget(0,1) + SEC_CELL_SX\2 - __x0Torp
  __dyTorp = secTarget(1,1) + SEC_CELL_SY\2 - __y0Torp
end sub

sub _gsFireTorpedo2.input()
end sub

sub _gsFireTorpedo2.logic(gtime%, dt%)
  phasorsFiring = min(phasorsFiring + dt%, 1000)
  __xTorp = __x0Torp + __dxTorp * phasorsFiring / 1000.0
  __yTorp = __y0Torp + __dyTorp * phasorsFiring / 1000.0
  if phasorsFiring = 1000 then
    _switchGameState("FireTorpedo3")    
  endif
end sub

sub _gsFireTorpedo2.render()
  sector.render()

  local integer c = map(fix(rnd()*256))
  box fix(__xTorp)-1,fix(__yTorp)-1,3,3,,c,c
end sub


'
' FT3: settle up
'
sub _gsFireTorpedo3.init()
local integer i, idx, df%
local string msg, sys$

  _setCmdPrompt("")
  _setCmdInfo("Hit <ENTER> or~    <ESC> to continue")
  __pha_num_dest = 0

  ' enemy outcomes
  for i=1 to numSecTargets
    idx = secTarget(2, i)

    msg = weaponsOfficer + "Enemy-" + str$(idx)

    ship.doTorpedoDamage(0, idx, sys$, df%)
    if df% <> 0 then
      _printInfo(msg + " has been destroyed!")
      __pha_destA(__pha_num_dest) = idx
      inc __pha_num_dest
    else
      _printInfo(msg + "'s " + sys$ + " has been hit.")
    endif
  next

  ' remove destroyed
  for i=0 to __pha_num_dest-1
    ship.destroyed(__pha_destA(i))
    inc enemiesKilled
  next

  _switchGameState("CommandInput")
end sub

sub _gsFireTorpedo3.input()
end sub

sub _gsFireTorpedo3.logic(gtime%, dt%)
end sub

sub _gsFireTorpedo3.render()
  sector.render()
end sub

