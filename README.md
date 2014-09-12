# Lua bindings for FTGL.

## Usage example:

    local ftgl = require "luaftgl"
    local font = ftgl.createPixmapFont("somefont.ttf")
    ftgl.setFontFaceSize(font, 24, 72)
    ftgl.setFontCharMap(font, ftgl.ENCODING_UNICODE)
    
    -- inside display loop
    ftgl.renderFont(font, "Hello FTGL!", ftgl.RENDER_ALL)
    
## Supported features:

None of the functions dealing with glyphs are currently supported, including
`ftglCreateCustomFont`. The rest of the C API is supported.

## Naming conventions:

- All function names have the "ftgl" prefix removed and the initial letter
  lowercased.

- `getLayoutBoundingBox` and `getFontBoundingBox` may be used in place of
  `getLayoutBBox` and `getFontBBox`.
  
- `getLayoutAlignment` may be used in place of `getLayoutAlignement` [sic].

## API changes:

Aside from the naming conventions mentioned above, the API is nearly identical
to the FTGL C API, with a few exceptions:

- The `*BBox` functions return a table instead of taking a `bounds` argument.

- `getFontBBox` does not take a `length` argument; this value is calculated
  from the length of the Lua string passed as the second argument.
  
- `attachData` takes a string for the `data` argument. It does not take a third
  argument; the size of the data is calculated from the length of the string.
