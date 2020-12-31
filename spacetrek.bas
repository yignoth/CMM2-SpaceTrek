'
' Space Trek
'

' TODO:
' * Implement time passing as ship moves in warp to new sector
' * Implement self repairs as time goes by
' * Implement resupply from supply container
' * Put in sound effects
' * Put in help / commands screen
' * Put in background music
' * Implement surveying stars
' * Sector impulse movement should be smooth
' * Sector impulse movement should stop if we hit a star, enemy, supply, etc.??? or not???
' * Gray out sector squares that are to far away to move into and prevent cursor from going there.
' * Gray out quadrant squares that are to far away to move into and prevent cursor from going there.
' * Should stars block phasors and torpedoes??????

mode 13,8 'mode 13,8
option base 0
option console screen
option explicit
font 7
cls


' Constants
const WORK_PAGE=1   ' WORK_PAGE+1 is also taken as second buffer
const ASSET_PAGE=3  ' ASSERT_PAGE+1 is also taken by smaller icons
const HUD_PAGE=5   ' HUD page

const QUAD_WIDTH=8, QUAD_HEIGHT=8, SECTOR_WIDTH=16, SECTOR_HEIGHT=16
const MAX_SHIPS=100
const MAX_ITEMS=100
const MAX_STARS=100

const DT = 30, MAX_FRAMETIME = 250


' Includes
#include "input.inc"
#include "ship.inc"
#include "item.inc"
#include "star.inc"
#include "quadrant.inc"
#include "sector.inc"
#include "hud.inc"
#include "disCmd.inc"
#include "disInfo.inc"
#include "disStats.inc"
#include "utils.inc"
#include "assets.inc"
#include "gsSplash.inc"
#include "gsWelcome.inc"
#include "gsEnterSector.inc"
#include "gsCommandInput.inc"
#include "gsSetShields.inc"
#include "gsImpulseMove.inc"
#include "gsWarpMove.inc"
#include "gsMoveShip.inc"
#include "gsFirePhasors.inc"
#include "gsFireTorpedo.inc"
#include "gsEnemyFire.inc"
#include "gsDestroyed.inc"
#include "gsResign.inc"



' Global vars
dim integer starDate, totalEnemies, numEnemies, totalItems, numItems, totalStars, numStars
dim integer starsSurveyed, enemiesKilled
dim string crefStr$

dim string gameState$, nextGameState$

dim integer quitGame = 0
dim integer totTime=1, numFrames=0
dim float fps=0
dim integer blinkPrompt = 0, blinkPromptTime = 0
dim integer statsInfoIdx = 0


'
' Begin program
'
_initGame()

_switchGameState("Splash")
_gameLoop()
END


'
' game loop
'
sub _gameLoop()
local float gameTime, currTime, accumTime, newTime, frameTime
  gameTime = 0
  currTime = timer
  accumTime = 0
  frameTime = 0

  do while quitGame = 0
    gameState$ = nextGameState$
    nextGameState$ = ""
    _GS("init")

    do
      newTime = timer
      frameTime = min(newTime - currTime, MAX_FRAMETIME)
      currTime = newTime

      inc blinkPromptTime, frameTime
      if blinkPromptTime > 250 then
        blinkPromptTime = 0
        blinkPrompt = not blinkPrompt
      endif
    
      _getKeyboardInput()   ' keyboard input
if keyInputVal = KEY_F2 then
  statsInfoIdx = _tern.i(statsInfoIdx = 0, 1, 0)
endif
      _GS("input")

      accumTime = accumTime + frameTime
      do while accumTime >= DT
        _updateGameLogic(gameTime, DT)
        gameTime = gameTime + DT
        accumTime = accumTime - DT
      loop

      _renderFrame()

      ' calc FPS
      totTime = totTime + frameTime
      numFrames = numFrames + 1
      fps = 1000.0 * numFrames / totTime
' display FPS
'__debug$ = str$(fix(fps))
    loop until nextGameState$ <> ""
  loop
end sub


' update game state
sub _updateGameLogic(gtime%, dt%)
  if mapDisplay = MAP_DISPLAY_SECTOR then
    sector.updateBackground(gtime%, dt%)
  endif

  _GSLogic(gtime%, dt%)
end sub


' render the frame
sub _renderFrame()
static integer pageNum = 0
    page write WORK_PAGE + pageNum
    cls

    _renderHUD()
    _renderStats()
    _renderInfo()

    _GS("render")

    page copy WORK_PAGE + pageNum to 0, B
    pageNum = (pageNum + 1) mod 2
end sub


' initialize variables
sub _initGame()
  starDate = 30000

  _loadAssets()
  _initHUD()

  disInfo.init()
  quadrant.init()
end sub


' switch game state
sub _switchGameState(gs$)
  nextGameState = gs$
end sub


' game state function names
sub _GS(s$) as string
local string func$ = "_gs" + gameState$ + "." + s$
  crefStr$ = func$
  call func$
end sub

sub _GSLogic(gtime%, dt%) as string
local string func$ = "_gs" + gameState$ + ".logic"
  crefStr$ = func$
  call func$, gtime%, dt%
end sub

