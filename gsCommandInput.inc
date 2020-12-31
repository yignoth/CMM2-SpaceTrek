'
' GAME STATE: CommandInput
'

' Constants
const COMMANDS_DSZ = 5


' init
sub _gsCommandInput.init()
  _setCmdPrompt("Command:")
  cmdText$ = ""
  mapDisplay = MAP_DISPLAY_SECTOR

  local string m$ = "awaiting orders...~"
  m$ = m$ + "~ <IMP> impulse move"
  m$ = m$ + "~ <PHA> fire phasors"
  m$ = m$ + "~ <SHL> set shields"
  m$ = m$ + "~ <TOR> fire torpedos"
  m$ = m$ + "~ <WAR> warp move"
  m$ = m$ + "~ <XXX> resign"
  m$ = m$ + "~~~ <F1> show quad/sect"
  _setCmdInfo(m$)
end sub

dim string cmdStr(COMMANDS_DSZ) length 3 = ("IMP", "SHL", "PHA", "TOR", "WAR", "XXX")
dim string cmdStrState(COMMANDS_DSZ) length 11 = ("ImpulseMove", "SetShields", "FirePhasors", "FireTorpedo", "WarpMove", "Resign")
' "Resupply",
' "ScanShip",
' "Survey",

' input
sub _gsCommandInput.input()
local integer i, clen

 if keyInputVal = KEY_F1 then
    mapDisplay = (mapDisplay+1) MOD 2

  elseif keyInputVal = KEY_BS then
    clen = len(cmdText$)
    if (clen > 0) then
      cmdText$ = left$(cmdText$, clen-1)
    endif

  elseif keyInputVal <> 0 then
'__debug$ = str$(asc(ucase$(keyInput))) + ", " + str$(keyInputVal)
    local string uc$ = ucase$(keyinput$)
    if uc$ >= "A" and uc$ <= "Z" then   
      cmdText$ = cmdText$ + uc$
      clen = len(cmdText$)

      if clen = 3 then
        for i=0 to COMMANDS_DSZ
          if cmdStr(i) = cmdText$ then
            _switchGameState(cmdStrState(i))
            exit sub
          endif
        next i

        _printInfo("Unknown command '" + cmdText$ + "' Captain.")
        ' beep()
        cmdText$ = ""
      endif
    endif
  endif
end sub


' logic
sub _gsCommandInput.logic(gtime%, dt%)
end sub


' render
sub _gsCommandInput.render()
  if mapDisplay = MAP_DISPLAY_SECTOR then
    sector.render()
  elseif mapDisplay = MAP_DISPLAY_QUAD then
    quadrant.render()
  endif

  _renderCmd()
end sub


