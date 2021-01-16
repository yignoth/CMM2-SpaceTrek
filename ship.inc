'
' Ship State definition.
'
' ship(col,row) is a 2D integer array
' col = the stats for a ship
' row = the ships
' ex: ship(:,1) is the stats for ship 1 
'

'
' Constants
'
const _STAT_HEALTH_GREEN = 95, _STAT_HEALTH_YELLOW = 60, _STAT_HEALTH_RED = 0


' system stats indicies
const _SIDX_POS=0    ' encoded position heading, quadrant-xy, sector-xy
const _SIDX_WARP=1   ' warp drive stats
const _SIDX_IMPL=2   ' impulse engine stats
const _SIDX_BATT=3   ' reserve stats
const _SIDX_SHLD=4   ' shield stats
const _SIDX_PHAS=5   ' phasor stats
const _SIDX_TORP=6   ' torpedo stats
const _SIDX_LRS=7    ' long range sensor stats
const _SIDX_SRS=8    ' short range sensor stats
const _SIDX_SCAN=9   ' scanner stats (scans stars and enemy ships)
const _SIDX_LIFE=10  ' life support and crew stats
const _SIDX_COMP=11  ' computer stats
const _SIDX_HULL=12  ' hull / armor
const _SIDX_DSZ = 12  ' array size (assumes zero based)

' Position stat encoding
const _POS_QX = 0    ' quadrant x
const _POS_QY = 1    ' quadrant y
const _POS_SX = 2    ' sector x
const _POS_SY = 3    ' sector y
const _POS_HEAD = 4  ' heading: 0=N, 1=E, 2=S, 3=W

' Other system stats encoding
const _STAT_LEVEL = 0   ' level
const _STAT_MAXHP = 1   ' max hp
const _STAT_CURHP = 2   ' curr hp
const _STAT_VALUE = 3   ' current value
const _STAT_DSZ = 3



'
' Variables
'

' Ship stats
dim integer shipStats(_SIDX_DSZ, MAX_SHIPS)
dim string shipStatsName(_SIDX_DSZ) length 12 = ("XXXPOS", "WarpDrive", "ImpulseDrive", "Batteries", "Shields", "Phasors", "Torpedoes", "LRS", "SRS", "Scanners", "LifeSupport", "Computer", "Hull")
'dim string shipStatsFunc(_SIDX_DSZ) length 4 = ("pos", "warp", "impl", "batt", "shld", "phas", "torp", "lrs", "srs", "scan", "life", "comp", "hull")


'
' Functions
'

sub ship.initStats(sidx%)
' STATS:
' LEVEL = 2 bytes    ' the tech level or size of the system
' MAX_HP = 2 bytes
' CUR_HP = 2 bytes
' VALUE = 2 bytes
'
' health = CUR_HP / MAX_HP
'
' (these stats do not use VALUE)
' warp: dist=level/20*health, energy-prod=level/4*health
' impl: dist=level/10*health
' lrs : quadrant scan distance = level/10*health
' srs : sector scan distance = SECTOR_SIZE*level/10*health
' phas: max=level*health
' scan: % scan effectiveness = level*health
' comp: max=cmp cap = NA, val = NA
' hull: acts as armor: dmg_mult_frac = (1.0-sqrt(level)/100) * (1.0-health)
'
' (these stats use VALUE)
' batt: max=level*health, value = current available energy to use
' shld: max=level*health,  value = current shields energy
' torp: max=fix(level/10), value = current number of torpedoes, dmg = level*10, #tgt=3*hlth%
' life: max_crew = level*0.10 + level*0.90*sin(pi/2*health), value = current crew number
'
'                                   pos wrp  imp   bat  shld   pha  trp  lrs  srs   sca   crw  cmp  hull
local integer slvl(_SIDX_DSZ)     = (0, 100, 100, 3000, 1000, 1000, 100, 100, 100, 1000, 1000, 100, 100)
local integer smhp(_SIDX_DSZ)     = (0, 100, 100,  100,  100,  100, 100, 100, 100,  100,  500, 100, 500)
local integer schp(_SIDX_DSZ)     = (0, 100, 100,  100,  100,  100, 100, 100, 100,  100,  500, 100, 500)
local integer sval(_SIDX_DSZ)     = (0,   0,   0, 3000, 9999,    0,  10,   0,   0,    0, 1000,   0,   0)
local integer klg_slvl(_SIDX_DSZ) = (0,  10,  50,  500,  300,  300,  30,   0,   0,    0,   50,  25,  25)
local integer klg_smhp(_SIDX_DSZ) = (0,  25,  25,   25,   50,  100,  50,   0,   0,    0,  150,  25, 125)
local integer klg_schp(_SIDX_DSZ) = (0,  25,  25,   25,   50,  100,  50,   0,   0,    0,  150,  25, 125)
local integer klg_sval(_SIDX_DSZ) = (0,   0,   0,  400,  100,  300,   1,   0,   0,    0,   50,   0,   0)
local integer ss(_STAT_DSZ)
local integer i,j

  for i=1 to _SIDX_DSZ
    if sidx% = 0 then
      ss(_STAT_LEVEL) = slvl(i)
      ss(_STAT_MAXHP) = smhp(i)
      ss(_STAT_CURHP) = schp(i)
      ss(_STAT_VALUE) = sval(i)
    else
      ss(_STAT_LEVEL) = klg_slvl(i)
      ss(_STAT_MAXHP) = klg_smhp(i)
      ss(_STAT_CURHP) = klg_schp(i)
      ss(_STAT_VALUE) = klg_sval(i)
    endif
    _encodeShipStats(ss(), i, sidx%)
  next
end sub


' Ship position and system access methods:
'   sidx% is the ship, the index into the shipsStats-array

' warp
function ship.warp.getHealth(sidx%) as float
  ship.warp.getHealth = _decodeStatHealth(_SIDX_WARP, sidx%)
end function
function ship.warp.getProd(sidx%) as integer
local integer p%(_STAT_DSZ)
  _decodeShipStats(_SIDX_WARP, sidx%, p%())
  ship.warp.getProd = fix(0.25 * p%(_STAT_LEVEL) * _convStatHealth(p%()))
end function

function ship.warp.getMaxDist(sidx%) as float
local integer st%(_STAT_DSZ)
  _decodeShipStats(_SIDX_WARP, sidx%, st%())
  local float hlth = _convStatHealth(st%())
  ship.warp.getMaxDist = (st%(_STAT_LEVEL) / 20) * hlth  
end function


' impulse
function ship.impl.getHealth(sidx%) as float
  ship.impl.getHealth = _decodeStatHealth(_SIDX_IMPL, sidx%)
end function

function ship.impl.getMaxDist(sidx%) as float
local integer st%(_STAT_DSZ)
  _decodeShipStats(_SIDX_IMPL, sidx%, st%())
  local float hlth = _convStatHealth(st%())
  ship.impl.getMaxDist = (st%(_STAT_LEVEL) / 10) * hlth  
end function


' batteries
function ship.batt.getHealth(sidx%) as float
  ship.batt.getHealth = _decodeStatHealth(_SIDX_BATT, sidx%)
end function
function ship.batt.getMaxValue(sidx%) as integer
local integer st%(_STAT_DSZ)
  _decodeShipStats(_SIDX_BATT, sidx%, st%())
  ship.batt.getMaxValue = st%(_STAT_VALUE)*_convStatHealth(st%())
end function
function ship.batt.getValue(sidx%) as integer
  ship.batt.getValue = _decodeStatValue(_SIDX_BATT, sidx%)
end function
sub ship.batt.setValue(sidx%, v%)
  _encodeStatValue(_SIDX_BATT, sidx%, v%)
end sub


' shields
function ship.shld.getHealth(sidx%) as float
  ship.shld.getHealth = _decodeStatHealth(_SIDX_SHLD, sidx%)
end function
function ship.shld.getValue(sidx%) as integer
  ship.shld.getValue = _decodeStatValue(_SIDX_SHLD, sidx%)
end function
sub ship.shld.setValue(sidx%, v%)
  _encodeStatValue(_SIDX_SHLD, sidx%, v%)
end sub
function ship.shld.getMaxValue(sidx%) as integer
local integer p%(_STAT_DSZ)
  _decodeShipStats(_SIDX_SHLD, sidx%, p%())
  ship.shld.getMaxValue = fix(p%(_STAT_LEVEL) * _convStatHealth(p%()))
end function

' phasors
function ship.phas.getHealth(sidx%) as float
  ship.phas.getHealth = _decodeStatHealth(_SIDX_PHAS, sidx%)
end function

function ship.phas.getMaxEnergy(sidx%) as integer
local integer p%(_STAT_DSZ)
  _decodeShipStats(_SIDX_PHAS, sidx%,p%())
  ship.phas.getMaxEnergy = cint(p%(_STAT_LEVEL) * _convStatHealth(p%()))
end function

function ship.phas.getValue(sidx%) as integer
  ship.phas.getValue = _decodeStatValue(_SIDX_PHAS, sidx%)
end function

function ship.phas.adjustForDist(pe, s1, s2) as integer
local integer x1,y1, x2,y2, dx,dy
local float d
  ship.pos.getSec(s1, x1, y1)
  ship.pos.getSec(s2, x2, y2)
  dx = x1 - x2
  dy = y1 - y2
  d = sqr(dx*dx + dy*dy)
  ship.phas.adjustForDist = cint(2*pe / d)
end function

' torpedoes
function ship.torp.getLevel(sidx%) as integer
  ship.torp.getLevel = _decodeStatLevel(_SIDX_TORP, sidx%)
end function
function ship.torp.getHealth(sidx%) as float
  ship.torp.getHealth = _decodeStatHealth(_SIDX_TORP, sidx%)
end function
function ship.torp.getMaxValue(sidx%) as integer
  ship.torp.getMaxValue = _decodeStatLevel(_SIDX_TORP, sidx%) / 10
end function
function ship.torp.getValue(sidx%) as integer
  ship.torp.getValue = _decodeStatValue(_SIDX_TORP, sidx%)
end function
sub ship.torp.setValue(sidx%, v%)
  _encodeStatValue(_SIDX_TORP, sidx%, v%)
end sub
function ship.torp.getMaxTargets(sidx%) as integer
  ship.torp.getMaxTargets = cint(3*_decodeStatHealth(_SIDX_TORP, sidx%))
end function

' long range sensors
function ship.lrs.getHealth(sidx%) as float
  ship.lrs.getHealth = _decodeStatHealth(_SIDX_LRS, sidx%)
end function

function ship.lrs.getMaxDist(sidx%) as float
local integer st%(_STAT_DSZ)
  _decodeShipStats(_SIDX_LRS, sidx%, st%())
  local float hlth = _convStatHealth(st%())
  ship.lrs.getMaxDist = (st%(_STAT_LEVEL)/50) * hlth
end function

' short range sensors
function ship.srs.getHealth(sidx%) as float
  ship.srs.getHealth = _decodeStatHealth(_SIDX_SRS, sidx%)
end function

function ship.srs.getMaxDist(sidx%) as float
local integer st%(_STAT_DSZ)
  _decodeShipStats(_SIDX_SRS, sidx%, st%())
  local float hlth = _convStatHealth(st%())
  local float md = sqr(SECTOR_WIDTH*SECTOR_WIDTH + SECTOR_HEIGHT*SECTOR_HEIGHT)
  ship.srs.getMaxDist = md * hlth
end function

' scanners
function ship.scan.getHealth(sidx%) as float
  ship.scan.getHealth = _decodeStatHealth(_SIDX_SCAN, sidx%)
end function

' life support / crew
function ship.life.getHealth(sidx%) as float
  ship.life.getHealth = _decodeStatHealth(_SIDX_LIFE, sidx%)
end function
function ship.life.getValue(sidx%) as integer
  ship.life.getValue = _decodeStatValue(_SIDX_LIFE, sidx%)
end function

' computer
function ship.comp.getHealth(sidx%) as float
  ship.comp.getHealth = _decodeStatHealth(_SIDX_COMP, sidx%)
end function

' hull
function ship.hull.getHealth(sidx%) as float
  ship.hull.getHealth = _decodeStatHealth(_SIDX_HULL, sidx%)
end function
function ship.hull.getDmgFrac(sidx%) as float
local integer p%(_STAT_DSZ)
local float frac
  _decodeShipStats(_SIDX_HULL, sidx%, p%())
  frac = 0.01 * sqr(p%(_STAT_LEVEL)) * _convStatHealth(p%())
  ship.hull.getDmgFrac = 1.0 - frac
end function



'
' ship position/heading in Quadrant and Sector access methods
'
sub ship.pos.getQuad(i%, x%, y%)
  local pval% = shipStats(_SIDX_POS, i%)
  x% = (pval% \ &h1000000) and &hff
  y% = (pval% \ &h10000) and &hff
end sub
sub ship.pos.setQuad(i%, x%, y%)
  local pval% = (x%*&h100 + y%)*&h10000
  shipStats(_SIDX_POS, i%) = (shipStats(_SIDX_POS, i%) AND &hffffffff0000ffff) + pval%
end sub

sub ship.pos.getSec(i%, x%, y%)
  local pval% = shipStats(_SIDX_POS, i%)
  x% = (pval% \ &h100) and &hff
  y% = pval% and &hff
end sub
sub ship.pos.setSec(i%, x%, y%)
  local pval% = x%*&h100 + y%
  shipStats(_SIDX_POS, i%) = (shipStats(_SIDX_POS, i%) AND &hffffffffffff0000) + pval%
end sub

function ship.pos.getHead(i%)
  local pval% = shipStats(_SIDX_POS, i%)
  ship.pos.getHead = (pval% \ &h100000000) and &hff
end function
sub ship.pos.setHead(i%, h%)
  local pval% = h% * &h100000000
  shipStats(_SIDX_POS, i%) = (shipStats(_SIDX_POS, i%) AND &hffffff00ffffffff) + pval%
end sub


'
' Damage and destroyed
'

sub ship.doTorpedoDamage(sidx%, tidx%, sys$, df%)
local integer i, ri, si, dmg, tdmg, dcnt=0, dest=0
local integer sysIdx(_SIDX_DSZ), sysCnt=0, hlthCnt=0
local integer st%(_STAT_DSZ)

  ' get torpedo damage amount
  tdmg = ship.torp.getLevel(sidx%) * 10

  ' find which systems the ship has (even if they are already destroyed)
  ' this differs from phasors which only hit non-damaged systems
  for i=1 to _SIDX_DSZ
    _decodeShipStats(i, tidx%, st%())
    if st%(_STAT_LEVEL) > 0 then
      if st%(_STAT_CURHP) > 0 then
        inc hlthCnt
        sysIdx(sysCnt) = i
      else
        sysIdx(sysCnt) = 0
      endif
      inc sysCnt
    endif
  next

  sys$ = ""

  ' torpedo damages random systems until all damage is used up
  do while tdmg > 0
    ' get system to damage
    ri = fix(rnd()*sysCnt)
    si = sysIdx(ri)

    if si > 0 then
      if dcnt > 0 then sys$ = sys$ + ", "
      sys$ = sys$ + shipStatsName(si)
      inc dcnt
      sysIdx(ri) = -si
    else
      si = -si
    endif

    ' roll damage
    dmg = min(tdmg, _rollD(10,5))
    inc tdmg, -dmg

    ' apply damage to system
    if si > 0 then
      if _modifyStatCurHp(si, tidx%, -dmg) = 0 then inc dest
      sysIdx(ri) = 0
    endif
  loop

  ' check if ship was destroyed
  if dest = hlthCnt then df% = 1
end sub


' phasor damage
sub ship.doPhasorDamage(idx%, pe%, sys$, df%)
local integer i, dmg, petot = pe%, si, ri, st%(_STAT_DSZ)
local integer sysIdx(_SIDX_DSZ), sysCnt=0, dcnt=0

  ' find which non-damaged systems the ship has
  for i=1 to _SIDX_DSZ
    _decodeShipStats(i, idx%, st%())
    if st%(_STAT_LEVEL) > 0 then
      if st%(_STAT_CURHP) > 0 then
        sysIdx(sysCnt) = i
        inc sysCnt
      endif
    endif
  next

  sys$ = ""

  ' loop over random system and damage it
  do while petot > 0 and sysCnt > 0
    ri = fix(rnd()*sysCnt)
    si = sysIdx(ri)

    if dcnt > 0 then sys$ = sys$ + ", "
    sys$ = sys$ + shipStatsName(si)
    inc dcnt

    dmg = min(petot, _decodeStatCurHp(si, idx%))
    inc petot, -dmg

    if _modifyStatCurHp(si, idx%, -dmg) = 0 then
      inc sysCnt, -1
      sysIdx(ri) = sysIdx(sysCnt)
    endif
  loop
  
  ' check if ship was destroyed
  if sysCnt = 0 then df% = 1
end sub


' An enemy ship is destroyed
sub ship.destroyed(sidx%)
local integer qx, qy, sx, sy, n
  ' get position of ship
  ship.pos.getQuad(sidx%, qx, qy)
  ship.pos.getSec(sidx%, sx, sy)

  ' create dead ship item
  item.makeDeadShip(qx, qy, sx, sy)

  ' decrease quadrant enemy count
  n = quadrant.getNumEnemies(qx, qy)
  quadrant.setNumEnemies(qx, qy, n-1)

  ' remove ship from stats
  ship.swapRecords(sidx%, numEnemies)
  ship.swapRecords(numEnemies, totalEnemies)
  inc numEnemies, -1
  inc totalEnemies, -1
  inc enemiesKilled
end sub


'
' Utility methods
'

' check system health status
function ship.syshealth.isGreen(sys)
  ship.syshealth.isGreen = sys >= _STAT_HEALTH_GREEN
end function
function ship.syshealth.isYellow(sys)
  ship.syshealth.isYellow = sys >= _STAT_HEALTH_YELLOW
end function


' Swaps records
sub ship.swapRecords(r1%, r2%)
  if r1% <> r2% then
    local integer i, t
    for i=0 to _SIDX_DSZ
      t = shipStats(i,r1%)
      shipStats(i,r1%) = shipStats(i,r2%)
      shipStats(i,r2%) = t
    next i
  endif
end sub


'
' Private methods
'

' Decodes ship stats
function _decodeStatHealth(stat%, sidx%) as float
local integer p%(_STAT_DSZ)
  _decodeShipStats(stat%, sidx%, p%())
  _decodeStatHealth = _convStatHealth(p%())
end function
function _convStatHealth(p%()) as float
  if p%(_STAT_MAXHP) > 0 then
    _convStatHealth = 1.0 * p%(_STAT_CURHP) / p%(_STAT_MAXHP)
  else
    _convStatHealth = 0
  endif
end function

function _decodeStatLevel(stat%, sidx%) as integer
  _decodeStatLevel = shipStats(stat%, sidx%) and &hffff
end function

function _decodeStatValue(stat%, sidx%) as integer
  _decodeStatValue = (shipStats(stat%, sidx%) \ &h1000000000000) and &hffff
end function
sub _encodeStatValue(stat%, sidx%, v%)
  local integer sval% = shipStats(stat%, sidx%) and &hffffffffffff
  shipStats(stat%, sidx%) = sval + v% * &h1000000000000
end sub

function _decodeStatCurHp(stat%, sidx%) as integer
  _decodeStatCurHp = (shipStats(stat%, sidx%) \ &h100000000) and &hffff
end function
sub _encodeStatCurHp(stat%, sidx%, v%)
  local integer sval% = shipStats(stat%, sidx%) and &hffff0000ffffffff
  shipStats(stat%, sidx%) = sval% + v% * &h100000000
end sub

' modifies the current hp and reduces the current value as well
' this is because damage was done
function _modifyStatCurHp(stat%, sidx%, v%) as integer
local integer st%(_STAT_DSZ)
  _decodeShipStats(stat%, sidx%, st%())

  local integer chp = st%(_STAT_CURHP)
  st%(_STAT_CURHP) = max(0, min(st%(_STAT_MAXHP), chp + v%))
  local integer nhp = st%(_STAT_CURHP)

  if nhp < chp then
    local float vfrac = 1.0 * nhp / chp
    if stat% <> _SIDX_LIFE then
      st%(_STAT_VALUE) = fix(st%(_STAT_VALUE) * vfrac)
    else
      st%(_STAT_VALUE) = max(st%(_STAT_LEVEL)/10, st%(_STAT_VALUE)*(1-vfrac/2))
    endif
  endif
  _encodeShipStats(st%(), stat%, sidx%)
  _modifyStatCurHp = nhp
end function


sub _decodeShipStats(stat%, sidx%, p%())
  local integer pval% = shipStats(stat%, sidx%)
  p%(_STAT_LEVEL) = pval% and &hffff
  p%(_STAT_MAXHP) = (pval% \ &h10000) and &hffff
  p%(_STAT_CURHP) = (pval% \ &h100000000) and &hffff
  p%(_STAT_VALUE) = (pval% \ &h1000000000000) and &hffff
end sub

sub _encodeShipStats(p%(), stat%, sidx%)
  local integer pval% = ((p%(_STAT_VALUE)*&h10000 + p%(_STAT_CURHP))*&h10000 + p%(_STAT_MAXHP))*&h10000 + p%(_STAT_LEVEL)
  shipStats(stat%, sidx%) = pval%
end sub
