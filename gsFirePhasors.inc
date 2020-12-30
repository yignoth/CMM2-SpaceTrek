'
' GAME STATE: FirePhasors
'

' Constants


' Variables
dim integer phasorEnergy, numSecTargets, secTargetIdx, phasorsFiring
dim integer secTarget(3,3)
dim integer __pha_destA(3), __pha_num_dest = 0



'
' FP: set energy level
'
sub _gsFirePhasors.init()
  mapDisplay = MAP_DISPLAY_SECTOR
  cmdText$ = ""

  if numEnemies = 0 then
    _printInfo(weaponsOfficer + "No enemies detected in sector.")
    _switchGameState("CommandInput")
  else
    local string pe$ = str$(ship.phas.getMaxEnergy(0))
    _setCmdInfo("Set phasor energy.~Max energy: " + pe$ + "~~ <ESC> cancel")
    _setCmdPrompt("Energy:")
  endif
end sub

sub _gsFirePhasors.input()
  if keyInputVal = KEY_ESC then
    _printInfo(weaponsOfficer + "Phasor order canceled")
    _switchGameState("CommandInput")
  elseif keyInputVal = KEY_BS then
    if cmdText$ <> "" then cmdText$ = left$(cmdText$, len(cmdText$)-1)
  elseif keyInputVal = KEY_ENTER then
    if cmdText$ = "" then
      _switchGameState("CommandInput")
    else
      phasorEnergy = val(cmdText$)
      if phasorEnergy = 0 then
        _switchGameState("CommandInput")
      else
        local integer pe = ship.phas.getMaxEnergy(0)
        if phasorEnergy > pe then
          _printInfo(weaponsOfficer + "Phasors cannot take that much energy.")
          _switchGameState("FirePhasors")
        else
          _switchGameState("FirePhasors2")
        endif
      endif
    endif    
  elseif keyInputVal >= KEY_0 and keyInputVal <= KEY_9 then
    if len(cmdText$) < 4 then
      cmdText$ = cmdText$ + keyInput$
    endif
  endif
end sub

sub _gsFirePhasors.logic(gtime%, dt%)
end sub

sub _gsFirePhasors.render()
  sector.render()
  _renderCmd()
end sub


'
' FP2: target enemies
'
sub _gsFirePhasors2.init()
local integer i, px, py, h

  _setCmdPrompt("Set Target(s)")
  _setCmdInfo("Select enemy targets.~~ <ARROW> move cursor~ <SPACE> target enemy~ <ENTER> fire phasors~~ <ESC> cancel")
  _setCmdPrompt("")
  cmdText$ = ""
  secTargetIdx = 1

  for i=0 to numEnemies
    ship.pos.getSec(i, px, py)
    h = ship.pos.getHead(i)
    secTarget(0,i) = HUD_MAP_X0 + px*SEC_CELL_SX
    secTarget(1,i) = HUD_MAP_Y0 + py*SEC_CELL_SY
    secTarget(2,i) = -1
    secTarget(3,i) = h
  next
end sub

sub _gsFirePhasors2.input()
  if keyInputVal = KEY_ESC then
    _printInfo(weaponsOfficer + "Phasor order canceled")
    _switchGameState("CommandInput")
  elseif keyInputVal = KEY_ENTER then
    local integer i
    numSecTargets = 0
    for i=1 to numEnemies
      if secTarget(2,i) >= 0 then
        inc numSecTargets
        _swapArrayPlaces2(i,numSecTargets,secTarget(),3)
      endif
    next
    if numSecTargets = 0 then
      _printInfo(weaponsOfficer + "No enemies targeted")
      _switchGameState("CommandInput")
    else
      _switchGameState("FirePhasors3")
    endif
  elseif keyInputVal = KEY_UP or keyInputVal = KEY_LEFT then
    inc secTargetIdx, -1
    if (secTargetIdx < 1) then secTargetIdx = numEnemies
  elseif keyinputVal = KEY_DOWN or keyInputVal = KEY_RIGHT then
    inc secTargetIdx
    if (secTargetIdx > numEnemies) then secTargetIdx = 1
  elseif keyInputVal = KEY_SPACE then
    secTarget(2,secTargetIdx) = _tern.i(secTarget(2,secTargetIdx)=-1,secTargetIdx,-1)
  endif
end sub

sub _gsFirePhasors2.logic(gtime%, dt%)
end sub

sub _gsFirePhasors2.render()
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
' FP3: fire phasors
'
sub _gsFirePhasors3.init()
  _printInfo(weaponsOfficer + "Firing phasors!")
  _setCmdPrompt("")
  _setCmdInfo("")
  secTargetIdx = 1
  phasorsFiring = 0
end sub

sub _gsFirePhasors3.input()

end sub

sub _gsFirePhasors3.logic(gtime%, dt%)
static integer fireTime = 0
  inc fireTime, dt%

  if fireTime > 2000 then
    fireTime = 0
    _switchGameState("FirePhasors4")
  elseif fireTime > 1000 then
    phasorsFiring = 1
  endif
end sub

sub _gsFirePhasors3.render()
local integer c, x, y, sx(3) = (7,4,7,10), sy(3) = (4,7,10,7)
  sector.render()

  x = secTarget(0,0)+sx(secTarget(3,0))
  y = secTarget(1,0)+sy(secTarget(3,0))
  c = map(fix(rnd()*256))

  if phasorsFiring = 1 then
    local integer i, dx=SEC_CELL_SX\2, dy=SEC_CELL_SY\2
    local integer ppx(3) = (0,1,1,0), ppy(3) = (0,0,1,1)

    for i=1 to numSecTargets
      line x+ppx(i-1),y+ppy(i-1), secTarget(0,i)+dx,secTarget(1,i)+dy,,c
    next
  else
    box x,y, 2,2,,c,c
  endif
end sub


'
' FP4: settle up
'
sub _gsFirePhasors4.init()
local integer i, idx, sx,sy, dx, dy, v, df%
local float d, pe, se
local string msg, smsg, sys$

  _setCmdPrompt("")
  _setCmdInfo("Hit <ENTER> or~    <ESC> to continue")
  __pha_num_dest = 0

  ' reduce our energy from reserves
  ship.batt.setValue(0, ship.batt.getValue(0) - phasorEnergy)

  ' get ship position
  ship.pos.getSec(0, sx, sy)

  ' enemy outcomes
  for i=1 to numSecTargets
    idx = secTarget(2, i)
    ship.pos.getSec(idx, dx, dy)
    dx = dx - sx
    dy = dy - sy
    d = sqr(dx*dx + dy*dy)

    pe = cint(2*phasorEnergy / d)
    se = ship.shld.getValue(idx)
    msg = weaponsOfficer + "Enemy-" + str$(idx)

    if se > 0 then
      smsg = msg + "'s shields "
      v = se - pe
      se = max(0, v)
      pe = max(0, -v)
      ship.shld.setValue(idx, se)
      if se > 0 then
        _printInfo(smsg + "absorbed phasors.")
      elseif pe = 0 then
        _printInfo(smsg + "are now down.")
      else
        _printInfo(smsg + "have been penetrated.")
      endif
    endif

    if pe > 0 then
      pe = cint(pe * ship.hull.getDmgFrac(idx))
      if (pe > 0) then
        ship.doPhasorDamage(idx, pe, sys$, df%)
        if df% <> 0 then
          _printInfo(msg + " has been destroyed!")
          __pha_destA(__pha_num_dest) = idx
          inc __pha_num_dest
        else
          _printInfo(msg + "'s " + sys$ + " has been hit.")
        endif
      else
        _printInfo(msg + "'s armor absorbed phasor fire.")
      endif
    endif
  next

  ' remove destroyed
  for i=0 to __pha_num_dest-1
    ship.destroyed(__pha_destA(i))
    inc enemiesKilled
  next

  ' switch states
  _switchGameState("CommandInput")
end sub

sub _gsFirePhasors4.input()
end sub

sub _gsFirePhasors4.logic(gtime%, dt%)
end sub

sub _gsFirePhasors4.render()
  sector.render()
end sub

