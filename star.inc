' Variables
dim integer __starStats(MAX_STARS-1)


'
' item/supply position access methods
'
sub star.pos.getQuad(i%, x%, y%)
  local pval% = __starStats(i%)
  x% = (pval% \ &h1000000) and &hff
  y% = (pval% \ &h10000) and &hff
end sub
sub star.pos.setQuad(i%, x%, y%)
  local pval% = (x%*&h100 + y%)*&h10000
  __starStats(i%) = (__starStats(i%) AND &hffffffff0000ffff) + pval%
end sub

sub star.pos.getSec(i%, x%, y%)
  local pval% = __starStats(i%)
  x% = (pval% \ &h100) and &hff
  y% = pval% and &hff
end sub
sub star.pos.setSec(i%, x%, y%)
  local pval% = x%*&h100 + y%
  __starStats(i%) = (__starStats(i%) AND &hffffffffffff0000) + pval%
end sub

sub star.swapRecords(r1%, r2%)
    local integer t = __starStats(r1%)
    __starStats(r1%) = __starStats(r2%)
    __starStats(r2%) = t
end sub
