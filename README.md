# 3dCutscenes
A C engine to display 3d-looking cutscenes

# How it works
This tool can take any number of images and draw them onto the 4 backgrounds, offering different scrolling speeds for each background to simulate a 3d effect.  
Additionally, text can also be configured to appear on screen with various colors, speeds, fonts(?), and positions.  


## Images
The images must follow this naming convention:  
`bg{bg_id}_{frame_start}.png`  
where `bg_id` is the id of the background this particular image will be drawn on, and `frame_start` identifies when the picture will be drawn on screen.  
Assuming a stable 60FPS, `bg0_120.png` will be drawn on background #0 (the one on top of all the others) after roughly 2 seconds.


## Misc
To configure the other behaviours, some C knowledge is required.  
Open the file `src/config/config.h`. This file contains all the configurable elements.  
Change the number after `#define ANIMATION_LEN` to change the total duration of the animation. The duration is expressed in frames.  
For example, assuming a stable 60FPS, `#define ANIMATION_LEN 60` will make the animation last 1 second.

## Scrolling
To configure the scrolling behaviour, modify the `scrolling_keyframes` array in the `src/config/config.h` file. The array is of type `struct scrolling_keyframe`, here's a breakdown of all the fields:  
- `frame_start`: this is the exact same concept as seen in the images naming convention. **IMPORTANT**: The array must be sorted by this field in ascending order. This means for example that making element 0 have frame_start=120 and element 1 frame_start=60 is not supported and WILL break the animation.
- `bg_id`: the background this scroll will apply to
- `speed`: how fast the bg will scroll. This is measured in 1/256 of a pixel (of course a change of <1 pixel will not be seen on screen, it just means that the scroll will take longer)
- `distance`: how much the screen will travel. This is measured the same as the `speed` field. Please note that this is more of a "final position" indicator: if the bg has already been scrolled by 60 pixels and we set distance to 120 pixels, the screen will only scroll an additional 60 pixels. Set to 0 for an "infinite" scroll (until the animatione ends)
- `axis`: either X or Y, pretty self explanatory
- `scroll_mode`: can be ADD, SUB, or SET. ADD/SUB will add or subtract the speed to the current scroll each frame, SET will just set the value put in the `speed` field as the current scroll distance.

## Text
To configure text appearing on screen, texboxes and text palettes must first be configured.  
Please note that text will always appear on background #0 (the one on top of all the others), and that background will not be available for image rendering (text can still scroll.)  

### Textboxes
Textboxes are rectangles on the screen where text can be displayed.  
To configure the textboxes, open the `src/config/config.h` file and look for the `textboxes[]` array. This array contains one entry for each texbox, plus an entry that marks the end of the array. The array is of type `struct TextboxTemplate`, here's a breakdown of all the fields:
- `bg_id`: the bg on which the texbox will be drawn. As of right now, the only bg supported is 0. A record with this value set to `0xFF` is considered the entry that marks the end of the array. 
- `x`: horizontal offset, starting from the left side of the screen. This value is multiplied by 8, i.e. a textbox with x=3 will start 24 pixels from the left side of the screen.
- `y`: vertical offset, starting from the top side of the screen. This value is multiplied by 8, then 4 is added, i.e. a textbox with y=3 will start 28 (8*3+4) pixels from the top side of the screen.
- `width`: width of the texbox, multiplied by 8 and then 1 is subtracted.
- `height`: height of the texbox, multiplied by 8 and then 1 is subtracted.
- `pal_id`: palette id of the textbox. As of right now, the only pal supported is 0xF (15)
- `charbase`: offset detailing where in VRAM the text will be inserted. As of right now, the only value supported is 1.

Please remember to always put a record with `bg_id`=`0xFF` to mark the end of the array.  
Having the first element of the array be the one with `bg_id`=`0xFF` will be interpreted as "no textbox configured", and bg #0 will be free to be used for image rendering. 

### Text color
Text color configuration is split in two:
- First, a palette has to be defined, this can be done by editing the `text_palettes` array, preferrably via the `rgba5()` macro.
- Secondly, one or more `struct TextColor` objects have to be defined. A `struct TextColor` has three fields:
  - `background`: the id of the background color within `text_palettes`.
  - `foreground`: the id of the foreground color within `text_palettes`.
  - `shadow`: the id of the shadow color within `text_palettes`.

Just as an example, defining a `struct TextColor` as `{0, 3, 4}` will set the background color to the color #0 within `text_palettes` (color #0 is always transparency), color #3 as the "main" text color, and color #4 as the text shadow color.

### Text contents
To draw text on screen, the text has to be put into the ROM first. This can be accomplished by editing the `main.s` file: navigate somewhere under the `.org freespace` directive, and place your text like so:  
```
text_label:
.string "Example text",0xFF
```
Please make sure to follow these rules:
- `text_label` must be unique withing the file, this label will be used later to reference the text
- The text has to be put after the `.string` directive and within double quotes. Special characters such as `\n` are supported.
- The text **must** be followed by the `,0xFF` terminator, so that the game will know the string ended.

Please note that sometimes, for still unknown reasons, the very first character of text will not be drawn on screen.

### Text keyframes
Once textboxes and text colors are set up, we can modify the `text_keyframes` array to configure which text should appear and when.  
The array must be terminated by a `END_TEXT_FRAME` entry.  
The array is of type `struct text_keyframe`, here's a breakdown of all the fields:
- `frame_start`: this is the exact same concept as seen in the images naming convention. **IMPORTANT**: The array must be sorted by this field in ascending order. This means for example that making element 0 have frame_start=120 and element 1 frame_start=60 is not supported and WILL break the animation.
- `textbox_id`: id of the textbox on which the text will be displayed. This is effectively the position in the `textboxes[]` array, starting from 0.
- `text`: this is the actual text to be displayed. In order to reference the text that has been inserted into the `main.s` file, just put the label name in this field 
- `font`: to be completely honest, I have no clue what this does.
- `x`: sets a horizontal margin within the textbox, starting from the left side of the textbox (measured in pixels)
- `y`: sets a vertical margin within the textbox, starting from the top side of the textbox (measured in pixels)
- `color`: A reference to the previously defined `struct TextColor` objects
- `speed`: An integer representing the speed at which text will be drawn. Setting a value of 0 will make the text appear instantly. Other than that the higher the value, the slower text is displayed on screen

# How to use
This tool requires:
- python (tested with 3.8.3)
- [grit](https://www.coranac.com/projects/grit/) 0.8.6+
- [devkitARM](https://devkitpro.org/wiki/devkitARM)

Once you have all the necessary software installed and ready to go, just navigate to the root of this project and copy/paste your ROM there. Rename the file to `BPRE0.gba`.  
Open the terminal, navigate to the root of the project, then run `python scripts/build.py 0xFFFFFFFF`, where `0xFFFFFFFF` is the free space in your ROM, and the script should insert everything, creating two new files:
- `multi.gba`: the updated ROM
- `symbols.txt`: a text file containing all the addresses known to the compiler. Conserve this file to know where everything in your ROM was inserted

# Internals
This is more like a handy to-do list for me to follow, but if you're curious:
- the images must be compiled into C arrays of pals, tilesets, and tilemaps (there is probably a much better way where many similar images can share a single tileset)
- the user can configure 2 distinct arrays:
  - `drawing_keyframes`, containing an ordered list of pictures, which bg they belong to, and when to draw them
  - `scrolling_keyframes`, containing an ordered list of scrolling data, which bg they belong to, and when to apply them
- from these arrays, the code must:
  - init an internal structure that will serve as the animation state
  - start a c2 loop that is called frame by frame and checks the keyframe arrays for updates
    - if there are any updates, apply the scroll or draw the image
    - increment `curr_frame_id`
  - end the loop after a configurable `animation_duration`

 `drawing_keyframes` is an array of `drawing_keyframe`:
 ```C
struct drawing_keyframe {
    u32 frame_start;
    u8 bgid;
    struct asset asset; //compiled by the macro ASSET(bgid, framestart)
};
```
 `drawing_keyframes` **must** be terminated by an `END_DRAWING_FRAME`

 `scrolling_keyframes` is an array of  `scrolling_keyframe`:
 ```C
struct scrolling_keyframe {
    u32 frame_start;
    u16 bg_id;
    u32 speed; //0xFF is a single pixel
    u32 distance; //0xFF is a single pixel. The scrolling will stop after this distance has been reached. Set this field to 0 for infinite scrolling
    enum scroll_mode scroll_mode; //SET, ADD, SUB
    enum axis axis; //X, Y
};
```
 `scrolling_keyframes` **must** be terminated by an `END_SCROLLING_FRAME`
