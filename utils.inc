'
' Utility methods
'

' Swap rows in a 2d integer array(col, row)
' args: row1, row2, 2d-array, index-of-last-column-to-swap (NOTE: not the length)
sub _swapArrayPlaces2(r1%, r2%, a%(), rlen%)
local integer i, t
  if r1% <> r2% then
    for i=0 to rlen%
      t = a%(i,r1%)
      a%(i,r1%) = a%(i,r2%)
      a%(i,r2%) = t
    next i
  endif
end sub

' Swap two elements in an integer array
sub _swapArrayPlaces1(i1%, i2%, a%())
local integer i, t
    t = a%(i1%)
    a%(i1%) = a%(i2%)
    a%(i2%) = t
end sub

' integer ternary function
function _tern.i(d, x%, y%) as integer
  if (d <> 0) then
    _tern.i = x%
  else
    _tern.i = y%
  endif
end function

' float ternary function
'function _tern.f(d, x, y) as float
'  if (d <> 0) then
'    _itern = x
'  else
'    _itern = y
'  endif
'end function

' string ternary function
'function _tern.s(d, x$, y$) as string
'  if (d <> 0) then
'    _itern = x$
'  else
'    _itern = y$
'  endif
'end function


' rolls an X sided die Y times
function _rollD(x%, y%) as integer
local i, t%
  t% = 0
  for i=1 to y%
    t% = t% + fix(rnd()*x%)+1
  next i
  _rollD = t%
end function
