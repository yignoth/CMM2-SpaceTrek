'
' GAME STATE: X
'

' Constants


' Variables


'
'Functions
'

' init
sub _gsWelcome.init()
end sub


' input
sub _gsWelcome.input()
  if keyInputVal <> 0 then
    _switchGameState("EnterSector")
  endif
end sub


' logic
sub _gsWelcome.logic(gtime%, dt%)
end sub


' render
sub _gsWelcome.render()
  cls
  colour CLR_GREEN
  print
  print
  print "  You are the captain of the exploration ship Endevor.  You have"
  print "been gated into the Gorzon Quadrant to survey the local star"
  print "systems.  Your ship is equipted with weapons should you encounter"
  print "any enemies.  Resupply pods have also been gated to various"
  print "places throughout the quadrant.  Use the pods to resupply your"
  print "ship and to help with any repairs if needed."
  print
  print "  Weapons at your disposal are Phasors and Torpedoes.  Phasor"
  print "damage is reduced by shields, and any remaining damage is applied"
  print "to working ship systems, one at a time until they are destroyed"
  print "or the phasor energy is used up.  Torpedoes go through shields"
  print "and apply partial damage to many ship systems, working or not."
  print "Both weapon systems can target one or more enemies at a time,"
  print "but the phasor energy allocated will be divided between targets."
  print
  print "Good Luck Captain"
  print
  print
  print
  print "Hit any key to continue."
end sub



