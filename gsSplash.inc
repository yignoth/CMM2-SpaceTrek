'
' GAME STATE: Splash Screen
'

' Constants


' Variables


'
'Functions
'

' init
sub _gsSplash.init()
end sub


' input
sub _gsSplash.input()
  if keyInputVal <> 0 then
    _switchGameState("Welcome")
  endif
end sub


' logic
sub _gsSplash.logic(gtime%, dt%)
end sub


' render
sub _gsSplash.render()
  cls
  text 100,30,"S p a c e     T r e k",,4,,CLR_YELLOW
  text 80,80,"(============)",,1,,CLR_GREEN
  text 80,90,"        \ \    /-------*-------\",,1,,CLR_GREEN
  text 80,100,"         \ \   \---------------/",,1,,CLR_GREEN
  text 80,110,"          \ \     /  /",,1,,CLR_GREEN
  text 80,120,"        \--\-\--------|",,1,,CLR_GREEN
  text 80,130,"         \            +(-",,1,,CLR_GREEN
  text 80,140,"          \-----------|",,1,,CLR_GREEN
  text 10,180,"  1 2 3 4 5 6 7 8",,7,,CLR_GREEN
  text 10,190,"1 . . . . . . . .",,7,,CLR_GREEN
  text 10,200,"2 . * . . . . E .",,7,,CLR_GREEN
  text 10,210,"3 . . . . . . . .",,7,,CLR_GREEN
  text 10,220,"4 . . . . . . . .",,7,,CLR_GREEN
  text 10,230,"5 . . . . * . . .",,7,,CLR_GREEN
  text 10,240,"6 . K . . . . . .",,7,,CLR_GREEN
  text 10,250,"7 . . . . . . . .",,7,,CLR_GREEN
  text 10,260,"8 . . . K . . . .",,7,,CLR_GREEN
  text 200,210,"Hit Any Key",,1,,CLR_WHITE
  text 200,220," to Start",,1,,CLR_WHITE
end sub



