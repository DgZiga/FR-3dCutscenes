.gba
.thumb
.open "./BPRE0.gba","./multi.gba", 0x08000000
.loadtable "./charmap.txt"


.org 0x83DEF2C
.thumb
.align 2
.word gui_1_new_gui_by_item|1;
.pool

.org 0x08810000
.importobj "./build/linked.o"

.close
