.gba
.thumb
.open "./BPRE0.gba","./multi.gba", 0x08000000
.loadtable "./charmap.txt"


.org 0x83DEF2C
.thumb
.align 2
.word gui_1_new_gui_by_item|1;
.pool

.org freespace

example_text_1:
.string " Lorem Ipsum\nDolor sit amet",0xFF
example_text_2:
.string " consectetur adipiscing elit",0xFF

.importobj "./build/linked.o"

.close
