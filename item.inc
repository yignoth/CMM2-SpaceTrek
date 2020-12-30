
' Constants
const ITEM_TYPE_SUPPLY = 0
const ITEM_TYPE_DEAD_ENEMY = 1


' Variables
dim integer __itemStats(MAX_ITEMS-1)


'
' item/supply position access methods
'
sub item.pos.getQuad(i%, x%, y%)
  local pval% = __itemStats(i%)
  x% = (pval% \ &h1000000) and &hff
  y% = (pval% \ &h10000) and &hff
end sub
sub item.pos.setQuad(i%, x%, y%)
  local pval% = (x%*&h100 + y%)*&h10000
  __itemStats(i%) = (__itemStats(i%) AND &hffffffff0000ffff) + pval%
end sub

sub item.pos.getSec(i%, x%, y%)
  local pval% = __itemStats(i%)
  x% = (pval% \ &h100) and &hff
  y% = pval% and &hff
end sub
sub item.pos.setSec(i%, x%, y%)
  local pval% = x%*&h100 + y%
  __itemStats(i%) = (__itemStats(i%) AND &hffffffffffff0000) + pval%
end sub

function item.getType(i%) as integer
  item.getType = (__itemStats(i%) and &hff00000000) \ &h100000000
end function
sub item.setType(i%, t%)
  __itemStats(i%) = (__itemStats(i%) and &hffffff00ffffffff) + (t% * &h100000000)
end sub

sub item.addSupply(qx%, qy%)
  item.setType(totalItems, ITEM_TYPE_SUPPLY)
  item.pos.setQuad(totalItems, qx%, qy%)
  inc totalItems
end sub 

sub item.makeDeadShip(qx%, qy%, sx%, sy%)
  item.setType(totalItems, ITEM_TYPE_DEAD_ENEMY)
  item.pos.setQuad(totalItems, qx%, qy%)
  item.pos.setSec(totalItems, sx%, sy%)
  item.swapRecords(numItems, totalItems)
  inc totalItems
  inc numItems
end sub

sub item.swapRecords(r1%, r2%)
    local integer t = __itemStats(r1%)
    __itemStats(r1%) = __itemStats(r2%)
    __itemStats(r2%) = t
end sub
