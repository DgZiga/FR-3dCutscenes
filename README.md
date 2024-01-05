# 3dCutscenes
A C engine to display 3d-looking cutscenes

# How it works
This tool can take any number of images and draw them onto the 4 backgrounds, offering different scrolling speeds for each background to simulate a 3d effect.  

The images must follow this naming convention:  
`bg{bg_id}_{frame_start}.png`  
where `bg_id` is the id of the background this particular image will be drawn on, and `frame_start` identifies when the picture will be drawn on screen.  
Assuming a stable 60FPS, `bg0_120.png` will be drawn on background #0 (the "lowest" one) after roughly 2 seconds.

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