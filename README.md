# 3dCutscenes
A C engine to display 3d-looking cutscenes

# How it works
This tool can take any number of images and draw them onto the 4 backgrounds, offering different scrolling speeds for each background to simulate a 3d effect.  

The images must follow this naming convention:  
`BG{bg_id}_{frame_id}.png`  
where `bg_id` is the id of the background this particular image will be drawn on, and `frame_id` is a unique identifier of this frame within this background.

# Internals
This is more like a handy to-do list for me to follow, but if you're curious:
- the images must be compiled into C arrays of pals, tilesets, and tilemaps (there is probably a much better way where many similar images can share a single tileset)
- for each background there have to be 2 data structures:
  - a `frame_data` struct, that maps the start time of frames to their `frame_id`.
  - a `scroll_data` struct, that maps the start time of the scrolling with their data (speed and direction)
- from these structures, the code must:
  - compile a timeline by taking the `frame_data` and the `scroll_data` maps and sorting them by their keys
  - init the backgrounds to the frame 0 images, also init an internal structure that contains the `curr_frame_id` and the `curr_actions_to_do`
  - start a c2 loop that is called frame by frame and check the timeline for updates
    - if there are any updates, update the `curr_actions_to_do`
    - do any actions listed in `curr_actions_to_do`
    - increment `curr_frame_id`
  - end the loop after a configurable `animation_duration`

 idea of `frame_data`:
 ```C
struct frame_data {
  u32 start;
  u32 frame_id; 
}

struct frame_data bg0_frame_data[] = //...
```

 idea of `scroll_data`:
 ```C
struct scroll_data {
  u32 start;
  u32 direction;
  u32 speed;
}

struct frame_data bg0_scroll_data[] = //...
```

 idea of `loop_state`:
 ```C
struct loop_state {
  u32 curr_frame_id;
  i32 scroll_y_speed; //positive is downward scroll, negative is upwards
  i32 scroll_x_speed; //positive is right scroll, negative is left
}

struct loop_state *ptr_to_loop_state = malloc(sizeof(struct loop_state));
ptr_to_loop_state->curr_frame_id = 0;
ptr_to_loop_state->scroll_y_speed = 0;
ptr_to_loop_state->scroll_x_speed = 0;
```
