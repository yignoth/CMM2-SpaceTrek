'
' Stats panel display code.
'

' Constants
const __CLR_HEALTH_GREEN = rgb(0,128,0)
const __CLR_HEALTH_YELLOW = rgb(128,128,0)
const __CLR_HEALTH_RED1 = rgb(128,0,0)
const __CLR_HEALTH_RED2 = rgb(255,0,0)
const __CLR_HEALTH_DESTROYED = CLR_BLACK

const __DNUM_STATS = 4
const __DNUM_HEALTH = 12


' Variables
' this should be STATIC under _renderStats() but it gives an error

dim string __debug$ = ""
dim integer __clrRedTimer = 0
dim integer __dsAltPos, __dsAltText(256)


'
' Functions
'

' draw stats
sub _renderStats()
local integer x0=HUD_STATS_X0+mm.info(fontwidth)*15+4
local integer y0=HUD_STATS_Y0+1

  __clrRedTimer = (__clrRedTimer + 1) mod 100
  __dsAltPos = 0

  text x0,y0,str$(starDate/10.0, 4, 1),,,,CLR_WHITE,CLR_STAT_BG
  inc y0, mm.info(fontheight)
  inc x0, mm.info(fontwidth)
  text x0,y0,str$(starsSurveyed,2)+"/"+str$(totalStars,2),,,,CLR_WHITE,CLR_STAT_BG
  inc y0, mm.info(fontheight)
  text x0,y0,str$(enemiesKilled,2)+"/"+str$(totalEnemies+enemiesKilled,2),,,,CLR_WHITE,CLR_STAT_BG

  ' values
  y0 = HUD_STATS2_Y0 + 1
  _disStats.print(str$(ship.warp.getProd(statsInfoIdx), 4), CLR_WHITE, y0)
  _disStats.print(str$(ship.batt.getValue(statsInfoIdx), 4), CLR_WHITE, y0)
  _disStats.print(str$(ship.shld.getValue(statsInfoIdx), 4), CLR_WHITE, y0)
  _disStats.print(str$(ship.torp.getValue(statsInfoIdx), 4), CLR_WHITE, y0)
  _disStats.print(str$(ship.life.getValue(statsInfoIdx), 4), CLR_WHITE, y0)

  ' health
  y0 = HUD_HEALTH_Y0+1
  _disStats.phealth(ship.warp.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.impl.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.batt.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.shld.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.phas.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.torp.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.lrs.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.srs.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.scan.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.life.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.comp.getHealth(statsInfoIdx), y0)
  _disStats.phealth(ship.hull.getHealth(statsInfoIdx), y0)

  if __debug$ <> "" then text HUD_HEALTH_X0+1,HUD_HEALTH_Y0-mm.info(fontheight)-4, __debug$
end sub

' helpers
sub _disStats.updateStatus()
local integer i
local float hlth = ship.comp.getHealth(0)

  __dsAltPos = 0
  for i=0 to 256
    __dsAltText(i) = (rnd() >= hlth)
  next
end sub

sub _disStats.phealth(h!, y%)
local integer v = cint(h!*100.0 + 0.499999)
local integer fclr

    if ship.syshealth.isGreen(v) <> 0 then
      fclr = __CLR_HEALTH_GREEN
    elseif ship.syshealth.isYellow(v) <> 0 then
      fclr = __CLR_HEALTH_YELLOW
    elseif v > 0 then
      if __clrRedTimer > 50 then
        fclr = __CLR_HEALTH_RED1
      else
        fclr = __CLR_HEALTH_RED2
      endif
    else
      fclr = __CLR_HEALTH_DESTROYED
    endif

    _disStats.print(str$(v, 3) + "%", fclr, y%)
end sub

sub _disStats.print(a$, fclr%, y%)
static integer x% = HUD_STATS_X0 + mm.info(fontwidth)*17 + 4
static integer fh% = mm.info(fontheight)
local integer i, v
local string b$

  b$ = ""
  for i=1 to len(a$)
    v = asc(mid$(a$,i,1))
    if fclr% <> __CLR_HEALTH_DESTROYED then
      if __dsAltText(__dsAltPos) > 0 then
        inc v, fix(rnd()*128)
      endif
    else
      v = asc("#")
    endif
    b$ = b$ + chr$(v and &h7F)
    inc __dsAltPos
  next

  text x%,y%,b$,,,,fclr%,CLR_STAT_BG
  inc y%, fh%  
end sub
