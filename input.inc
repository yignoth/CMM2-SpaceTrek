'
' Input Methods
'

' Constants
const KEY_BS=8
const KEY_LF=10
const KEY_ENTER=13
const KEY_ESC=27
const KEY_SPACE = asc(" ")
const KEY_0 = asc("0")
const KEY_9 = asc("9")
const KEY_A = asc("A")
const KEY_Z = asc("Z")
const KEY_F1=145
const KEY_F2=146
const KEY_UP=128
const KEY_DOWN=129
const KEY_LEFT=130
const KEY_RIGHT=131


' Variables
dim string keyInput$, __keyLF$
dim integer keyInputVal    ' asc() value of user input


'
' Functions
'

' user input
sub _getKeyboardInput()
  keyInput$ = inkey$
  keyInputVal = asc(keyInput$)
  if keyInputVal = KEY_ENTER then __keyLF$ = inkey$
end sub

