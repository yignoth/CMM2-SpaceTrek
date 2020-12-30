'
' Space Trek
'

' Load Assets
'
' Loads the game sprite and sound assets.
'

'
' loads the assets
'
sub _loadAssets()
  ' load main images on page
  page write ASSET_PAGE
  load bmp "graphics/sprites.bmp"

  ' creates a smaller 8x8 set of images
  page write ASSET_PAGE+1
  image resize 0,0, 256,256, 0,0, 128,128, ASSET_PAGE

  ' load and play music file
  'play modfile "music/helly_-_space.mod",48000
end sub

