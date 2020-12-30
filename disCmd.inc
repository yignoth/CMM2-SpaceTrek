'
' The orders/command panel.
'

' Variables
dim string cmdPrompt$, cmdText$


'
' Functions
'

' set prompt
sub _setCmdPrompt(a$)
  cmdPrompt$ = a$
  cmdText$ = ""
end sub

sub _setCmdText(a$)
  cmdText$ = a$
end sub

sub _setCmdInfo(a$)
local integer x0 = HUD_CMD_X0 + 4
local integer y0 = HUD_CMD2_Y0 + 1
local integer alen = len(a$)
local integer p0=1, p=1

  page write HUD_PAGE
  box HUD_CMD_X0,HUD_CMD2_Y0,mm.hres-HUD_CMD_X0-4,HUD_HEALTH_Y0-HUD_CMD2_Y0-4,,CLR_CMD_BG,CLR_CMD_BG

  do while p <= alen
    if mid$(a$,p,1) = "~" then
      if p > p0 then
        __disCmdText(x0,y0,mid$(a$,p0,p-p0))
      endif
      p0 = p+1
      inc y0, mm.info(fontheight)
    endif
    inc p
  loop
  if p > p0 then
    __disCmdText(x0, y0, mid$(a$,p0,p-p0))
  endif
end sub

sub __disCmdText(x as integer, y as integer, a$ as string)
local integer i, x0 = x, alen=len(a$), clr = CLR_GRAY
local string c$

  for i = 1 to alen
    c$ = mid$(a$,i,1)

    if c$ = "<" then
      clr = CLR_YELLOW
    elseif c$ = ">" then
      clr = CLR_GRAY
    else
      text x0, y, c$,,,,clr,CLR_CMD_BG
      x0 = mm.info(hpos)
    endif
  next
end sub


' draw headline
sub _renderCmd()
local integer x0 = HUD_CMD_X0+4

  ' command section
  text x0, HUD_CMD_Y0+1, cmdPrompt$,,,,CLR_GRAY,CLR_CMD_BG
  inc x0, len(cmdPrompt$)*mm.info(fontwidth)

  if cmdText$ <> "" then
    text x0, HUD_CMD_Y0+1, cmdText$,,,,CLR_WHITE,CLR_CMD_BG
    inc x0, len(cmdText$)*mm.info(fontwidth)
  endif

  if blinkPrompt <> 0 then
    text x0, HUD_CMD_Y0+1, chr$(127),,,,CLR_WHITE,CLR_CMD_BG
  endif
end sub

