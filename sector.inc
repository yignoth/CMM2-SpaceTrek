'
' Space Trek
'

' Constants
const __MAX_BG_STARS=100     ' max number of background stars blinking in sector

' Global vars
dim integer __bgStarsNum
dim integer __bgStars(2,__MAX_BG_STARS)


'
' Functions
'

' update sector background stars
sub sector.updateBackground(gtime, dt)
local integer i, v
static integer lastTime = 0
  inc lastTime, dt
  if lastTime > 250 then
    lastTime = 0
    i = int(rnd() * __bgStarsNum)
    v = __bgStars(1,i)
    __bgStars(1,i) = (v and &hffffffff00000000) + _starBrightness()
  endif
end sub

' render sector
sub sector.render()
local integer i, h, px, py, sx, sy, c, m
static integer simg = 0

  for i=0 to __bgStarsNum-1
    px = __bgStars(0,i) \ 1000
    py = __bgStars(0,i) MOD 1000
    c = (__bgStars(1,i) \ &h100000000) and &hffffffff
    m = __bgStars(1,i) AND &hffffffff
    pixel HUD_MAP_X0+px, HUD_MAP_Y0+py, c AND m
  next

  ' draw ship
  ship.pos.getSec(0, sx,sy)
  h = ship.pos.getHead(0)
  _calcSectorPixelCoord(sx, sy, px, py)
  blit 32,h*16,px,py,SEC_CELL_SX,SEC_CELL_SY,ASSET_PAGE,&b100

  ' draw enemies
  for i=1 to numEnemies
    ship.pos.getSec(i, sx, sy)
    _calcSectorPixelCoord(sx, sy, px, py)
    blit 48,0,px,py,SEC_CELL_SX,SEC_CELL_SY,ASSET_PAGE,&b100
  next

  ' draw items
  for i=0 to numItems-1
    c = item.getType(i)
    item.pos.getSec(i, sx, sy)
    _calcSectorPixelCoord(sx, sy, px, py)
    blit 16,c*16,px,py,SEC_CELL_SX,SEC_CELL_SY,ASSET_PAGE,&b100 
  next

  ' draw stars
  for i=0 to numStars-1
    star.pos.getSec(i, sx, sy)
    _calcSectorPixelCoord(sx, sy, px, py)
    blit 0,int(simg/20)*SEC_CELL_SY,px,py,SEC_CELL_SX,SEC_CELL_SY,ASSET_PAGE,&b100
  next
  simg = (simg + 1) mod 80
end sub


' initialize sector: happens whenever ship enters a new sector
sub sector.init()
local integer i, x, y, sqx, sqy, qx, qy, sx, sy, p
local integer smap(SECTOR_WIDTH-1, SECTOR_HEIGHT-1)
local integer sclrs(3) = (rgb(255,255,128), rgb(255,128,128), rgb(128,255,128), rgb(128,128,255))
local integer hlth

  ' place our ship
  ship.pos.getQuad(0, sqx, sqy)
  _findEmptySectorSpot(smap(), sx, sy)
  ship.pos.setSec(0, sx, sy)

  ' update quardrant visited (or scanned using LRS if not broken)
  hlth = ship.syshealth.isGreen(ship.lrs.getHealth(0)*100)
  for x=sqx-1 to sqx+1
    if x >= 0 and x < QUAD_WIDTH then
      for y=sqy-1 to sqy+1
        if y >= 0 and y < QUAD_HEIGHT then
          if (x = sqx and y = sqy) or (hlth <> 0) then
              quadrant.setVisited(x,y)
          endif
        endif
      next
    endif
  next

  ' find and place all enemies in our quadrant
  numEnemies = 0
  for i=1 to totalEnemies
    ship.pos.getQuad(i, qx, qy)
    if qx = sqx and qy = sqy then
      numEnemies = numEnemies + 1
      if i <> numEnemies then
        ' move current sector enemies to top of list (after our ship)
        ship.swapRecords(i, numEnemies)
      endif
      _findEmptySectorSpot(smap(), sx, sy)
      ship.pos.setSec(numEnemies, sx, sy)
    endif
  next

  ' place items, if any
  numItems = 0
  for i=0 to totalItems-1
    item.pos.getQuad(i, qx, qy)
    if qx = sqx and qy = sqy then
      if i <> numItems then
        item.swapRecords(i, numItems)
      endif
      _findEmptySectorSpot(smap(), sx, sy)
      item.pos.setSec(numItems, sx, sy)
      numItems = numItems + 1
    endif
  next

  ' place stars, if any
  numStars = 0
  for i=0 to totalStars-1
    star.pos.getQuad(i, qx, qy)
    if qx = sqx and qy = sqy then
      if i <> numStars then
        star.swapRecords(i, numStars)
      endif
      _findEmptySectorSpot(smap(), sx, sy)
      star.pos.setSec(numStars, sx, sy)
      numStars = numStars + 1
    endif
  next

  ' place background stars
  __bgStarsNum = __MAX_BG_STARS/2 + int(rnd()*__MAX_BG_STARS/2)

  for i=0 to __bgStarsNum-1
    x = rnd()*SECTOR_WIDTH*SEC_CELL_SX
    y = rnd()*SECTOR_HEIGHT*SEC_CELL_SY
    if rnd() > 0.75 then
      qx = sclrs(int(rnd()*4))
    else
      qx = CLR_WHITE
    endif
    qy = _starBrightness()
    __bgStars(0,i) = x*1000 + y
    __bgStars(1,i) = qx * &h100000000 + qy
  next
end sub


'
' Utility methods
'

' find the sector pixel coord
sub _calcSectorPixelCoord(px%, py%, sx%, sy%)
  sx% = HUD_MAP_X0 + px%*SEC_CELL_SX
  sy% = HUD_MAP_Y0 + py%*SEC_CELL_SY
end sub

' find empty spot in sector
sub _findEmptySectorSpot(smap%(), x%, y%)
  do
    x% = int(rnd()*SECTOR_WIDTH)
    y% = int(rnd()*SECTOR_HEIGHT)
  loop while smap%(x%,y%) = 1
  smap%(x%,y%) = 1
end sub

' utility function to set a random star brightness
function _starBrightness() as integer
local integer v(3) = (&h3f,&h8f,&hcf,&hff)
local integer t
  t = v(int(rnd()*4))
  _starBrightness = &HFF000000 + &H10000*t + &H100*t + t
end function

