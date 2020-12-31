'
' Quadrant
'

' Constants
const CLR_QUAD_SHIP_BG = rgb(32,0,0)


' Variables
dim integer quadMap(QUAD_WIDTH-1, QUAD_HEIGHT-1)


' render the quadrant
sub quadrant.render()
local integer i, x, y, px, py, h%, spx, spy

  ' draw grid
  for i = 1 to QUAD_WIDTH-1
    px = HUD_MAP_X0 + i*QUAD_CELL_SX
    line px,HUD_MAP_Y0,px,HUD_MAP_Y0+MAP_HEIGHT-1,,CLR_GRAY
  next
  for i = 1 to QUAD_HEIGHT-1
    py = HUD_MAP_Y0 + i*QUAD_CELL_SY
    line HUD_MAP_X0,py,HUD_MAP_X0+MAP_WIDTH-1,py,,CLR_GRAY
  next

  ' draw background for our ship cell
  ship.pos.getQuad(0, px, py)
  spx = HUD_MAP_X0 + px*QUAD_CELL_SX
  spy = HUD_MAP_Y0 + py*QUAD_CELL_SY
  box spx+1,spy+1,QUAD_CELL_SX-1,QUAD_CELL_SY-1,,CLR_QUAD_SHIP_BG,CLR_QUAD_SHIP_BG

  ' draw quad cells
  for x = 0 to QUAD_WIDTH-1
    px = HUD_MAP_X0 + x*QUAD_CELL_SX
    for y = 0 to QUAD_HEIGHT-1
      py = HUD_MAP_Y0 + y*QUAD_CELL_SY
      if quadrant.hasVisited(x,y) <> 0 then
        ' draw enemies
        for i=1 to quadrant.getNumEnemies(x,y)
          blit 24,0,px+1+(i-1)*10,py+2,8,8,ASSET_PAGE+1
        next
        ' draw items
        if quadrant.getNumItems(x,y) > 0 then blit 8,0,px+2,py+12,8,8,ASSET_PAGE+1
        ' draw stars
        for i=1 to quadrant.getNumStars(x,y)
          blit 0,0,px+1+(i-1)*10,py+22,8,8,ASSET_PAGE+1
        next
      else
        box px+1, py+1, QUAD_CELL_SX-1, QUAD_CELL_SY-1,, CLR_DK_GRAY,CLR_DK_GRAY
      endif
    next
  next

  ' draw our ship
  h% = ship.pos.getHead(0)
  blit 16,h%*8, spx+12,spy+12, 8,8, ASSET_PAGE+1
end sub


' initialize quadrant variables
sub quadrant.init()
local integer i, j, qx, qy, v, n, sx, sy, nitems

  totalEnemies = min(_rollD(6,5), MAX_SHIPS)
  nitems = min(1 + int(rnd()*totalEnemies/3), MAX_ITEMS)
  totalStars = min(_rollD(6,8), MAX_STARS)

  ' clear quad map
  for i=0 to QUAD_WIDTH-1
    for j=0 to QUAD_HEIGHT-1
      quadMap(i,j) = 0
    next
  next

  ' initialize ship values
  for i=0 to totalEnemies
    ship.initStats(i)
  next

  ' initialize ship's position in quadrant
  sx = fix(rnd()*QUAD_WIDTH)
  sy = fix(rnd()*QUAD_HEIGHT)
  ship.pos.setQuad(0, sx,sy)
  ship.pos.setHead(0, 0)

  ' initialize enemy positions
  for i=1 to totalEnemies
    do
      qx = fix(rnd()*QUAD_WIDTH)
      qy = fix(rnd()*QUAD_HEIGHT)
      if qx <> sx or qy <> sy then    ' no ships in initial landing spot
        n = quadrant.getNumEnemies(qx,qy)
        if n < 3 then
          ship.pos.setQuad(i, qx, qy)
          quadrant.setNumEnemies(qx,qy,n+1)
          exit do
        endif
      endif
    loop
  next i

  ' initialize resupply positions
  for i=0 to nitems-1
    do
      qx = fix(rnd()*QUAD_WIDTH)
      qy = fix(rnd()*QUAD_HEIGHT)
      n = quadrant.getNumItems(qx,qy)
      if n = 0 then                   ' only 1 per sector
        item.addSupply(qx, qy)
        quadrant.setNumItems(qx,qy,1)
        exit do
      endif
    loop
  next i

  ' initialize star positions
  for i=0 to totalStars-1
    do
      qx = fix(rnd()*QUAD_WIDTH)
      qy = fix(rnd()*QUAD_HEIGHT)
      n = quadrant.getNumStars(qx,qy)
      if n = 0 then                     ' only 1 per sector
        star.pos.setQuad(i, qx, qy)
        quadrant.setNumStars(qx,qy,n+1)
        exit do
      endif
    loop
  next i
end sub

'
' quad map methods
'

function quadrant.hasVisited(x%, y%)
  quadrant.hasVisited = quadMap(x%,y%) and &h1000
end function
sub quadrant.setVisited(x%, y%)
  quadMap(x%,y%) = (quadMap(x%,y%) and &hffffffffffffefff) + &h1000
end sub

function quadrant.getNumEnemies(x%,y%)
  quadrant.getNumEnemies = (quadMap(x%,y%) and &hf00) \ 256
end function
sub quadrant.setNumEnemies(x%,y%,v%)
  quadMap(x%,y%) = (quadMap(x%,y%) and &hfffffffffffff0ff) + (v% * 256)
end sub

function quadrant.getNumItems(x%,y%)
  quadrant.getNumItems = (quadMap(x%,y%) and &h0f0) \ 16
end function
sub quadrant.setNumItems(x%,y%,v%)
  quadMap(x%,y%) = (quadMap(x%,y%) and &hffffffffffffff0f) + (v% * 16)
end sub

function quadrant.getNumStars(x%,y%)
  quadrant.getNumStars = (quadMap(x%,y%) and &h00f)
end function
sub quadrant.setNumStars(x%,y%,v%)
  quadMap(x%,y%) = (quadMap(x%,y%) and &hfffffffffffffff0) + v%
end sub
