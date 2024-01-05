# 3dCutscenes
A C engine to display 3d-looking cutscenes

# How it works
This tool can take any number of images and draw them onto the 4 backgrounds, offering different scrolling speeds for each background to simulate a 3d effect.  

The images must follow this naming convention:  
`bg{bg_id}_{frame_start}.png`  
where `bg_id` is the id of the background this particular image will be drawn on, and `frame_start` identifies when the picture will be drawn on screen.  
Assuming a stable 60FPS, `bg0_120.png` will be drawn on background #0 (the "lowest" one) after roughly 2 seconds.

To configure the other behaviours, some C knowledge is required.  
Open the file `src/config/config.h`, this file contains all the editable stuff.  
Change the number after `#define ANIMATION_LEN` to change the total duration of the animation. The duration is expressed in frames.  
For example, assuming a stable 60FPS, `#define ANIMATION_LEN 60` will make the animation last 1 second.

To configure the scrolling behaviour, modify the `scrolling_keyframes` array.  The array is of type `struct scrolling_keyframe`, let's explore each field:  
- `frame_start`: this is the exact same concept as seen in the images naming convention. **IMPORTANT**: The array must be ordered by this field in ascending order. This means for example that making element 0 have frame_start=120 and element 1 frame_start=60 is not supported and WILL break the animation.
- `bg_id`: the background this scroll will apply to
- `speed`: how fast the bg will scroll. This is measured in 1/256 of a pixel (of course a change of <1 pixel will not be seen on screen, it just means that the scroll will take longer)
- `distance`: how much the screen will travel. This is measured the same as the `speed` field. Please note that this is more of a "final position" indicator: if the bg has already been scrolled by 60 pixels and we set distance to 120 pixels, the screen will only scroll an additional 60 pixels. Set to 0 for an "infinite" scroll (until the animatione ends)
- `axis`: either X or Y, pretty self explanatory
- `scroll_mode`: can be ADD, SUB, or SET. ADD/SUB will add or subtract the speed to the current scroll each frame, SET will just set the value put in the `speed` field as the current scroll distance.

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
