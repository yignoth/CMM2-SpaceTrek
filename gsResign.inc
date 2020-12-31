'
' GAME STATE: X
'

' Constants


' Variables


'
'Functions
'

' init
sub _gsResign.init()
end sub


' input
sub _gsResign.input()
  if keyInputVal <> 0 then
    end
  endif
end sub


' logic
sub _gsResign.logic(gtime%, dt%)
end sub


' render
sub _gsResign.render()
  cls
  colour CLR_GREEN
  print
  print
  print "  You have resigned your commision with " + str$(totalEnemies) " enemies remaining."
  if totalEnemies > 0 then
    print "It is only a metter of time before the enemies reach earth"
    print "and your family and friends are enslaved forever."
  else
    print "Congratulations on job well done.  It's Miller time!"
  endif
  print
  print
  print "Hit any key to end the game."
end sub



