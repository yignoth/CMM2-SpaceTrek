'
' GAME STATE: X
'

' Constants


' Variables


'
'Functions
'

' init
sub _gsDestroyed.init()
end sub


' input
sub _gsDestroyed.input()
  if keyInputVal <> 0 then
    end
  endif
end sub


' logic
sub _gsDestroyed.logic(gtime%, dt%)
end sub


' render
sub _gsDestroyed.render()
  cls
  colour CLR_GREEN
  print
  print "...some extravigant, award wining death sequence..."
  print
  print
  print
  print "  You were destroyed by the enemy!  You have failed your mission."
  print "Your family back home will live in disgrace.  The enemy's final"
  print "transmission to you: ALL YOUR BASE ARE BELONG TO US"
  print
  print
  print
  print
  print "Hit any key to end game."
end sub



