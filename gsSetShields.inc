'
' GAME STATE: SetShields
'

' Constants


' Variables


'
' set shield level
'
sub _gsSetShields.init()
local integer be = ship.batt.getValue(0)
local integer se = ship.shld.getValue(0)
local integer sm = ship.shld.getMaxValue(0)
local string s$ = str$(min(sm, be+se))

  _setCmdPrompt("Energy:")
  _setCmdInfo("Set shield energy.~Max energy: " + s$ + "~~ <ESC> cancel")
  cmdText$ = ""
end sub


sub _gsSetShields.input()
  if keyInputVal = KEY_ESC then
    _printInfo(weaponsOfficer + "Set shields order canceled")
    _switchGameState("CommandInput")
  elseif keyInputVal = KEY_BS then
    if cmdText$ <> "" then cmdText$ = left$(cmdText$, len(cmdText$)-1)
  elseif keyInputVal = KEY_ENTER then
    if cmdText$ = "" then
      _switchGameState("CommandInput")
    else
      local integer shlds = val(cmdText$)
      local integer be = ship.batt.getValue(0)
      local integer se = ship.shld.getValue(0)
      local integer sm = ship.shld.getMaxValue(0)

      if shlds > sm then
        _printInfo(weaponsOfficer + "Shields cannot be set to that strength")
        cmdText$ = ""
      elseif shlds > be+se then
        _printInfo(weaponsOfficer + "Not enough energy for that setting")
        cmdText$ = ""
      else
        ship.batt.setValue(0, be+se - shlds)
        ship.shld.setValue(0, shlds)
        if shlds > 0 then
          _printInfo(weaponsOfficer + "Shields set to " + str$(shlds))
        else
          _printInfo(weaponsOfficer + "Lowering shields")
        endif
        _switchGameState("CommandInput")
      endif
    endif    
  elseif keyInputVal >= KEY_0 and keyInputVal <= KEY_9 then
    if len(cmdText$) < 4 then
      cmdText$ = cmdText$ + keyInput$
    endif
  endif
end sub

sub _gsSetShields.logic(gtime%, dt%)
end sub

sub _gsSetShields.render()
  sector.render()
  _renderCmd()
end sub

