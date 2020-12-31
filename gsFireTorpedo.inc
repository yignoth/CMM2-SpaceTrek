'
' GAME STATE: FireTorpedo
'

' Constants


' Variables
dim float __xTorp(3), __yTorp(3), __dxTorp(3), __dyTorp(3)
dim integer __x0Torp, __y0Torp, __tgtDest%(3), __torpOn(3)
dim string __tgtSys$(3)


'
' FT: select target
'
sub _gsFireTorpedo.init()
local float hlth = ship.torp.getHealth(0)

  mapDisplay = MAP_DISPLAY_SECTOR
  cmdText$ = ""

  if numEnemies = 0 then
    _printInfo(weaponsOfficer + "No enemies detected in sector.")
    _switchGameState("CommandInput")
  elseif hlth = 0 then
    _printInfo(weaponsOfficer + "Torpedo tubes have been destroyed.")
    _switchGameState("CommandInput")
  elseif ship.torp.getValue(0) = 0 then
    _printInfo(weaponsOfficer + "No torpedoes in armory.")
    _switchGameState("CommandInput")
  else
    local integer i, px, py
    _setCmdInfo("Select enemy target.~~ <ARROW> move cursor~ <SPACE> target enemy~ <ENTER> fire torpedo~~ <ESC> cancel")
    secTargetIdx = 1
    numSecTargets = 0

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
    if numSecTargets = 0 then
      _printInfo(weaponsOfficer + "No enemies targeted")
      _switchGameState("CommandInput")
    else
      local integer i
      for i=1 to numEnemies
        if secTarget(2,i) >= 0 then
          _swapArrayPlaces2(i,numSecTargets,secTarget(),2)
        endif
      next
      _switchGameState("FireTorpedo2")
    endif
  elseif keyInputVal = KEY_UP or keyInputVal = KEY_LEFT then
    inc secTargetIdx, -1
    if (secTargetIdx < 1) then secTargetIdx = numEnemies
  elseif keyinputVal = KEY_DOWN or keyInputVal = KEY_RIGHT then
    inc secTargetIdx
    if (secTargetIdx > numEnemies) then secTargetIdx = 1
  elseif keyInputVal = KEY_SPACE then
    if secTarget(2,secTargetIdx) > 0 then
      secTarget(2,secTargetIdx) = -1
      inc numSecTargets, -1
    elseif ship.torp.getValue(0) < numSecTargets+1 then
      _printInfo(weaponsOfficer + "Not enough torpedoes.")
    else
      secTarget(2,secTargetIdx) = secTargetIdx
      inc numSecTargets
    endif
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
local integer i, h, idx, sx(3) = (7,4,7,10), sy(3) = (4,7,10,7)

  _printInfo(weaponsOfficer + "Firing torpedo!")
  _setCmdPrompt("")
  _setCmdInfo("")
  phasorsFiring = 0

  ' reduce number of torpedos
  ship.torp.setValue(0, ship.torp.getValue(0) - 1)

  ' need to sort target indices from high to low, so that deleting the
  ' enemies if destroyed doesn't mess up targeted indicies
  __firePhasors.sortTargets()

  ' do damage to enemies
  for i=1 to numSecTargets
    idx = secTarget(2, i)
    ship.doTorpedoDamage(0, idx, __tgtSys$(i), __tgtDest%(i))
  next

  ' setup torpedo animation
  h = ship.pos.getHead(0)
  __x0Torp = secTarget(0,0) + sx(h)
  __y0Torp = secTarget(1,0) + sy(h)
  for i = 1 to numSecTargets
    __dxTorp(i) = secTarget(0,i) + SEC_CELL_SX\2 - __x0Torp
    __dyTorp(i) = secTarget(1,i) + SEC_CELL_SY\2 - __y0Torp
    __torpOn(i) = (1-i)*500
  next
end sub

sub _gsFireTorpedo2.input()
end sub

sub _gsFireTorpedo2.logic(gtime%, dt%)
local integer i
local float frac
  phasorsFiring = phasorsFiring + dt%
  if phasorsFiring > 1500+500*numSecTargets then
    _switchGameState("FireTorpedo3")    
  else
    for i = 1 to numSecTargets
      inc __torpOn(i), dt%
      frac = max(0, __torpOn(i) / 1000.0)
      __xTorp(i) = __x0Torp + __dxTorp(i) * frac
      __yTorp(i) = __y0Torp + __dyTorp(i) * frac
      
      ' check for death
      if frac >= 1 and __tgtDest(i) <> 0 and __torpOn(i) > 0 then
        ship.destroyed(secTarget(2,i))
        __torpOn(i) = -10000000
      endif
    next
  endif
end sub

sub _gsFireTorpedo2.render()
local integer i, c
  sector.render()

  c = map(fix(rnd()*256))
  for i=1 to numSecTargets
    if __torpOn(i) > 0 and __torpOn(i) < 1000 then
      box fix(__xTorp(i))-1,fix(__yTorp(i))-1,3,3,,c,c
    endif
  next
end sub


'
' FT3: settle up
'
sub _gsFireTorpedo3.init()
local integer i, j, a, b, idx, df%
local string msg, sys$

  _setCmdPrompt("")
  _setCmdInfo("Hit <ENTER> or~    <ESC> to continue")

  ' enemy outcomes
  for i=1 to numSecTargets
    idx = secTarget(2, i)
    msg = weaponsOfficer + "Enemy-" + str$(idx)

    if __tgtDest%(i) <> 0 then
      _printInfo(msg + " has been destroyed!")
    else
      _printInfo(msg + "'s " + __tgtSys$(i) + " has been hit.")
    endif
  next

  _switchGameState("EnemyFire")
end sub

sub _gsFireTorpedo3.input()
end sub

sub _gsFireTorpedo3.logic(gtime%, dt%)
end sub

sub _gsFireTorpedo3.render()
  sector.render()
end sub

