'
' Routines for the INFO panel.
'

' Constants
const __DI_INFO_DSZ = 9


' Variables
dim string infoStr$(__DI_INFO_DSZ) length 42 ' info string
dim integer infoPos = 0
dim string weaponsOfficer = "Weapons: "
dim string helmOfficer = "Helm: "
dim string enginesOfficer = "Engineering: "


'
' Functions
'

sub disInfo.init()
  infoPos = 0
end sub

' draw info panel
sub _renderInfo()
local integer i, fclr, y0, fh
  if infoPos > 0 then
    y0 = HUD_INFO_Y0+1
    fh = mm.info(fontheight)

    for i=max(0,infoPos-4) to infoPos-1
      fclr = CLR_GRAY : if i = infoPos-1 then fclr = CLR_WHITE
      text HUD_INFO_X0+1,y0,infoStr$(i),,,,fclr,CLR_INFO_BG
      inc y0, fh
    next
  endif
end sub


' set the information string
sub _printInfo(a$)
local integer idx, clen
  clen = len(a$)
  if clen > 0 then
    if clen <= 42 then
      _printInfo2(a$)
    else
      idx = instr(a$,":")
      if idx > 0 then
        _printInfo2(left$(a$,idx))
        _printInfo2(right$(a$,clen-idx))
      else
        _printInfo2(a$)
      endif
    endif
  endif
end sub

sub _printInfo2(a$)
local integer i
  if len(a$) > 42 then a$ = left$(a$,39) + "..."

  if infoPos <= __DI_INFO_DSZ then
    infoStr$(infoPos) = a$
    inc infoPos
  else
    for i=1 to __DI_INFO_DSZ
      infoStr$(i-1) = infoStr$(i)
    next
    infoStr$(__DI_INFO_DSZ) = a$
  endif
end sub

