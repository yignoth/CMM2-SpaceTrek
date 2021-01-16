'
' GAME STATE: X
'

' Constants
const __EF_MODE_PHAS = 1
const __EF_MODE_TORP = 2


' Variables
dim integer __efIdx=0, __efDestroyed, __efPE(3)


'
'Functions
'

' init
sub _gsEnemyFire.init()
local integer i, pe, nt, px,py, fmode
  if numEnemies = 0 then
    _switchGameState("CommandInput")
  endif

  hud.updateStatus()

  __efIdx = 0
  __efDestroyed = 0
  phasorsFiring = 0

  for i=0 to numEnemies
    ship.pos.getSec(i, px, py)
    secTarget(0,i) = HUD_MAP_X0 + px*SEC_CELL_SX + SEC_CELL_SX\2
    secTarget(1,i) = HUD_MAP_Y0 + py*SEC_CELL_SY + SEC_CELL_SY\2

    if i > 0 then
      pe = ship.phas.adjustForDist(ship.phas.getValue(i), i, 0)
      nt = ship.torp.getValue(i)

      if pe >= 100 and nt > 0 then
        if rnd() > 0.5 then
          fmode = __EF_MODE_PHAS
        else
          fmode = __EF_MODE_TORP
        endif
      elseif nt > 0 then
        fmode = __EF_MODE_TORP
      elseif pe > 0 then
        fmode = __EF_MODE_PHAS
      else
        fmode = 0
      endif

      secTarget(2,i) = fmode
      if fmode = __EF_MODE_TORP then
        ship.torp.setValue(i, ship.torp.getValue(i)-1)
      elseif fmode = __EF_MODE_PHAS then
        __efPE(i) = pe
      endif
    else
      secTarget(2,i) = 0
    endif
  next
end sub


' input
sub _gsEnemyFire.input()
end sub


' logic
sub _gsEnemyFire.logic(gtime%, dt%)
local integer fmode = secTarget(2,__efIdx), pe, sh, df%
local string s$

  if fmode > 0 then
    inc phasorsFiring, dt%
    if phasorsFiring > 1000 then
      secTarget(2,__efIdx) = 0
      
      if fmode = __EF_MODE_PHAS then
        pe = __efPE(__efIdx)
        sh = ship.shld.getValue(0)
        if pe > sh then
          _printInfo(enginesOfficer + "Enemy phasors penetrated shields")
          inc pe, -sh
          sh = 0
        elseif sh > 0 then
          inc sh, -pe
          pe = 0
          _printInfo(enginesOfficer + "Shields absorbed enemy phasor fire")
        else
          _printInfo(enginesOfficer + "Ship hit by enemy phasors")
        endif

        ship.shld.setValue(0, sh)
        if pe > 0 then
          ship.doPhasorDamage(0, pe, s$, df%)
          hud.updateStatus()
        endif
      elseif fmode = __EF_MODE_TORP then
        _printInfo(enginesOfficer + "Ship hit by enemy torpedo")
        ship.doTorpedoDamage(__efIdx, 0, s$, df%)
        hud.updateStatus()
      endif

      if df% <> 0 then __efDestroyed = 1
    endif
  elseif __efIdx < numEnemies then
    inc __efIdx
    phasorsFiring = 0
  else
    _switchGameState("EnemyFire2")
  endif
end sub


' render
sub _gsEnemyFire.render()
  sector.render()

  if __efIdx > 0 and __efIdx <= numEnemies then
    if phasorsFiring >= 0 and phasorsFiring <= 1000 then
      local integer fmode = secTarget(2,__efIdx)
      local integer c = map(fix(rnd()*256))

      if fmode = __EF_MODE_PHAS then
        line secTarget(0,0),secTarget(1,0), secTarget(0,__efIdx),secTarget(1,__efIdx)-7,,c              
      elseif fmode = __EF_MODE_TORP then
        local integer dx = secTarget(0,0) - secTarget(0,__efIdx)
        local integer dy = secTarget(1,0) - secTarget(1,__efIdx)
        local float frac = phasorsFiring / 1000.0
        local integer x = secTarget(0,__efIdx) + dx * frac
        local integer y = secTarget(1,__efIdx) + dy * frac
        box x-1,y-1,3,3,,c,c
      endif
    endif
  endif
end sub



'
' Phase 2: settle up
'

' init
sub _gsEnemyFire2.init()
  if __efDestroyed <> 0 then
    _switchGameState("Destroyed")
  else
    _switchGameState("CommandInput")
  endif
end sub


' input
sub _gsEnemyFire2.input()
end sub


' logic
sub _gsEnemyFire2.logic(gtime%, dt%)
end sub


' render
sub _gsEnemyFire2.render()
  sector.render()
end sub

