'
' The main Heads-Up-Display rendering code,
' but some code lives in SECTOR.INC, QUADRANT.INC, disInfo.inc, etc.
'

' Constants
const MAP_WIDTH = 256, MAP_HEIGHT = 256
const SEC_CELL_SX = MAP_WIDTH / SECTOR_WIDTH, SEC_CELL_SY = MAP_HEIGHT / SECTOR_HEIGHT
const QUAD_CELL_SX = MAP_WIDTH / QUAD_WIDTH, QUAD_CELL_SY = MAP_HEIGHT / QUAD_HEIGHT

const MAP_DISPLAY_SECTOR=0, MAP_DISPLAY_QUAD=1, MAP_DISPLAY_HELP=2

const HUD_MAP_X0=4, HUD_MAP_Y0=4
const HUD_STATS_X0 = HUD_MAP_X0+4+SECTOR_WIDTH*SEC_CELL_SX
const HUD_STATS_Y0 = HUD_MAP_Y0
const HUD_STATS2_Y0 = HUD_STATS_Y0 + mm.info(fontheight)*3 + 3
const HUD_INFO_X0 = HUD_MAP_X0
const HUD_INFO_Y0 = HUD_MAP_Y0+4+SECTOR_HEIGHT*SEC_CELL_SY
const HUD_CMD_X0 = HUD_STATS_X0
const HUD_CMD_Y0 = HUD_STATS2_Y0 + mm.info(fontheight)*5 + 5
const HUD_CMD2_Y0 = HUD_CMD_Y0 + mm.info(fontheight) + 2
const HUD_HEALTH_X0 = HUD_STATS_X0
const HUD_HEALTH_Y0 = mm.vres - 5 - mm.info(fontheight)*12

const CLR_WHITE = rgb(255,255,255)
const CLR_YELLOW = rgb(255,255,0)
const CLR_GREEN = rgb(0,255,0)
const CLR_RED = rgb(255,0,0)
const CLR_DK_GRAY = rgb(64,64,64)
const CLR_GRAY = rgb(128,128,128)
const CLR_LT_GRAY = rgb(192,192,192)
const CLR_STAT_LABELS = rgb(128,128,128)
const CLR_STAT_BG = rgb(00,32,64)
const CLR_INFO_BG = CLR_STAT_BG 'rgb(32,32,0)
const CLR_CMD_BG = rgb(32,32,0) 'rgb(32,0,32)


' Variables
dim integer mapDisplay            ' map display mode


'
' Functions
'

' render HUD
sub _renderHUD()
  blit 0,0, 0,0, mm.hres,mm.vres, HUD_PAGE
end sub


' initialize the HUD
sub _initHUD()
  page write HUD_PAGE
  cls
  _drawHUDH(0,0,mm.hres,0)              ' top H line
  _drawHUDH(0,mm.vres-4,mm.hres,1)      ' bot H line
  _drawHUDV(0,0,mm.vres,0)              ' left V line
  _drawHUDV(mm.hres-4,0,mm.vres,1)      ' right V line
  _drawHUDV(HUD_STATS_X0-4,HUD_STATS_Y0-4,mm.vres,2) ' map V right
  _drawHUDH(0,HUD_INFO_Y0-4,MAP_WIDTH+8,2) ' map bot H line
  _drawHUDH(HUD_CMD_X0-4,HUD_CMD_Y0-4,mm.hres-MAP_WIDTH-4,2) ' CMD right split H line
  _drawHUDH(HUD_HEALTH_X0-4,HUD_HEALTH_Y0-4,mm.hres-MAP_WIDTH-4,2) ' HEALTH right split H line

  ' bg for stats and stats2 panel
  box HUD_STATS_X0,HUD_STATS_Y0,mm.hres-HUD_STATS_X0-4,HUD_CMD_Y0-HUD_STATS_Y0-4,,CLR_STAT_BG,CLR_STAT_BG
  line HUD_STATS_X0+2,HUD_STATS2_Y0-1,mm.hres-6,HUD_STATS2_Y0-1,,CLR_GRAY

  ' bg for info panel
  box HUD_INFO_X0,HUD_INFO_Y0,HUD_STATS_X0-8,mm.vres-HUD_INFO_Y0-4,,CLR_INFO_BG,CLR_INFO_BG

  ' bg for cmd panel
  box HUD_CMD_X0,HUD_CMD_Y0,mm.hres-HUD_CMD_X0-4,HUD_HEALTH_Y0-HUD_CMD_Y0-4,,CLR_CMD_BG,CLR_CMD_BG
  line HUD_STATS_X0+2,HUD_CMD2_Y0-1,mm.hres-6,HUD_CMD2_Y0-1,,CLR_GRAY

  ' bg for health panel
  box HUD_STATS_X0,HUD_HEALTH_Y0,mm.hres-HUD_STATS_X0-4,mm.vres-HUD_HEALTH_Y0-4,,CLR_STAT_BG,CLR_STAT_BG

  _initStatsTextPanel()
end sub


sub _initStatsTextPanel()
local integer y0=HUD_STATS_Y0+1

  ' stats text
  _hudStatText("Stardate", y0)
  _hudStatText("Stars Surveyed", y0)
  _hudStatText("Enemies Killed", y0)

  ' stats2 text
  y0 = HUD_STATS2_Y0 + 1
  _hudStatText("Warp Prod", y0)
  _hudStatText("Batteries", y0)
  _hudStatText("Shields", y0)
  _hudStatText("Torpedoes", y0)
  _hudStatText("Crewmen", y0)

  ' health text
  y0 = HUD_HEALTH_Y0 + 1
  _hudStatText("Warp Engines", y0)
  _hudStatText("Impulse Engines", y0)
  _hudStatText("Batteries", y0)
  _hudStatText("Shields", y0)
  _hudStatText("Phasors", y0)
  _hudStatText("Torpedoes", y0)
  _hudStatText("Long Rng Sensors", y0)
  _hudStatText("Shrt Rng Sensors", y0)
  _hudStatText("Scanners", y0)
  _hudStatText("Life Support", y0)
  _hudStatText("Computer", y0)
  _hudStatText("Hull", y0)
end sub

sub _hudStatText(t$, y%)
  text HUD_STATS_X0+4,y%,t$,,,,CLR_STAT_LABELS,CLR_STAT_BG
  inc y%, mm.info(fontheight)
end sub

sub _drawHUDH(x,y,w,t)
local a,b,c,d
  if t = 0 then
    a=0:b=1:c=2:d=3
  elseif t = 1 then
    a=3:b=2:c=1:d=0
  else
    a=4:b=2:c=3:d=3
  endif
  line x+a,y+0,x+w-1-a,y+0,,CLR_DK_GRAY
  line x+b,y+1,x+w-1-b,y+1,,CLR_GRAY
  line x+c,y+2,x+w-1-c,y+2,,CLR_GRAY
  line x+d,y+3,x+w-1-d,y+3,,CLR_LT_GRAY
end sub

sub _drawHUDV(x,y,h,t)
local a,b,c,d
  if t = 0 then
    a=0:b=1:c=2:d=3
  elseif t = 1 then
    a=3:b=2:c=1:d=0
  else
    a=4:b=2:c=3:d=3
  endif
  line x+0,y+a,x+0,y+h-1-a,,CLR_DK_GRAY
  line x+1,y+b,x+1,y+h-1-b,,CLR_GRAY
  line x+2,y+c,x+2,y+h-1-c,,CLR_GRAY
  line x+3,y+d,x+3,y+h-1-d,,CLR_LT_GRAY
end sub

