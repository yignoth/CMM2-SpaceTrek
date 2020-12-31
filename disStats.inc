'
' Stats panel display code.
'

' Constants
const __CLR_HEALTH_GREEN = rgb(0,128,0)
const __CLR_HEALTH_YELLOW = rgb(128,128,0)
const __CLR_HEALTH_RED1 = rgb(128,0,0)
const __CLR_HEALTH_RED2 = rgb(255,0,0)

const __DNUM_STATS = 4
const __DNUM_HEALTH = 12


' Variables
' this should be STATIC under _renderStats() but it gives an error

dim string __debug$ = ""
dim integer __clrRedTimer = 0


'
' Functions
'

' draw stats
sub _renderStats()
local integer y0=HUD_STATS_Y0+1

  __clrRedTimer = (__clrRedTimer + 1) mod 100

  text HUD_STATS_X0+mm.info(fontwidth)*13+4,y0,str$(starDate/10.0, 6, 1),,,,CLR_WHITE,CLR_STAT_BG
  inc y0, mm.info(fontheight)
  _disStats.print(str$(starsSurveyed, 4), CLR_WHITE, y0)
  _disStats.print(str$(enemiesKilled, 4), CLR_WHITE, y0)

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
sub _disStats.phealth(h!, y%)
local integer v = cint(h!*100.0 + 0.499999)
local integer fclr

    if ship.syshealth.isGreen(v) <> 0 then
      fclr = __CLR_HEALTH_GREEN
    elseif ship.syshealth.isYellow(v) <> 0 then
      fclr = __CLR_HEALTH_YELLOW
    elseif __clrRedTimer > 50 then
      fclr = __CLR_HEALTH_RED1
    else
      fclr = __CLR_HEALTH_RED2
    endif

    _disStats.print(str$(v, 3) + "%", fclr, y%)
end sub

sub _disStats.print(a$, fclr%, y%)
static integer x% = HUD_STATS_X0 + mm.info(fontwidth)*17 + 4
static integer fh% = mm.info(fontheight)  
  text x%,y%,a$,,,,fclr%,CLR_STAT_BG
  inc y%, fh%
end sub
