#!/usr/bin/env python
 
from glob import glob
import os
import itertools
import hashlib
import subprocess
import sys

# Default path for Windows
PATH = 'C:/devkitPro/devkitARM/bin'

# Check for the '-wsl' flag in the arguments
if len(sys.argv) > 2 and sys.argv[2] == '-wsl':
    PATH = '/usr/bin'

PREFIX = '/arm-none-eabi-'
AS = (PATH + PREFIX + 'as')
CC = (PATH + PREFIX + 'gcc')
LD = (PATH + PREFIX + 'ld')
OBJCOPY = (PATH + PREFIX + 'objcopy')
ROOT = './'
SRC = ROOT+'src/'
BUILT_GRAPHICS = './src/built_graphics'
BUILD = './build'
ASFLAGS = ['-mthumb']
LDFLAGS = ['-z','muldefs','-T', 'linker.ld', '-T', 'BPRE.ld', '-r']
CFLAGS= ['-Isrc/include', '-mthumb', '-mno-thumb-interwork', '-mcpu=arm7tdmi',
         '-fno-inline', '-mlong-calls', '-march=armv4t', '-fno-builtin', '-Wall', '-O2', '-std=c11']
GRITFLAGS=['-gB4',      #4bpp
           '-gzl',      #tileset is lz77 compressed
           '-pzl',      #pal is lz77 compressed
           '-pn16',     #pal is 16 colours
           '-gu8',      #tileset is u8 array
           '-pu8',      #pal is u8 array
           '-mzl',      #map is lz77 compressed
           '-mR4',      #
           '-aw256',    #area width is 256
           '-gTFF0000', #rgb(255,0,0) (#0xFF0000) is trasparency
           '-ftc']      #file_type: c
 
def run_command(cmd):
    try:
        subprocess.check_output(cmd)
    except subprocess.CalledProcessError as e:
        print(e.output.decode(), file=sys.stderr)
        sys.exit(1)
 
def make_output_file(filename):
    '''Return hash of filename to use as object filename'''
    m = hashlib.md5()
    m.update(filename.encode())
    return os.path.join(BUILD, m.hexdigest() + '.o')
 
def process_assembly(in_file):
    '''Assemble'''
    out_file = make_output_file(in_file)
    cmd = [AS] + ASFLAGS + ['-c', in_file, '-o', out_file]
    run_command(cmd)
    return out_file
 
def process_c(in_file):
    '''Compile C'''
    out_file = make_output_file(in_file)
    cmd = [CC] + CFLAGS + ['-c', in_file, '-o', out_file]
    run_command(cmd)
    return out_file
 
from string import Template

def process_img(in_file, frame_data):
    '''Compile IMGs'''

    # populate frame_data
    filename = os.path.splitext(os.path.basename(in_file))[0][2:]
    bgid = filename.split('_')[0]
    frame_start = filename.split('_')[1]
    frame_data.append(dict(
        bgid=bgid,
        frame_start=int(frame_start)
    ))

    # imgs are first converted to .c/.h files, then built like the rest of the source code
    out_file = os.path.join(os.path.dirname(in_file), '..', 'built_graphics', os.path.basename(in_file))

    print('Running Grit on '+os.path.abspath(out_file))
    cmd = ['grit', in_file, '-o', out_file, '-mp'+bgid] + GRITFLAGS
    run_command(cmd)


    return out_file
 
def link(objects):
    '''Link objects into one binary'''
    linked = 'build/linked.o'
    cmd = [LD] + LDFLAGS + ['-o', linked] + list(objects)
    #print(cmd)
    run_command(cmd)
    return linked
 
def objcopy(binary):
    cmd = [OBJCOPY, '-O', 'binary', binary, 'build/output.bin']
    run_command(cmd)
 
def run_glob(globstr, fn):
    '''Glob recursively and run the processor function on each file in result'''
    files = glob(os.path.join(ROOT, globstr), recursive=True)
    return map(fn, files)

def clear_folder(foldername):
    '''Empty folder of files'''
    files = glob(foldername+'/*')
    for f in files:
        os.remove(f)

 
from string import Template

def main():
    if len(sys.argv) < 2:
        print("No custom address for the compiled code has been set.")
        print("To add a custom address simply put it after the python executable name, like so: python scripts/build.py 0x08800000.")
        print("Aborting...")
        return

    customAddr = sys.argv[1]
    print("Inserting code at "+customAddr)
 
    # Create output directory
    try:
        os.makedirs(BUILD)
        os.makedirs(BUILT_GRAPHICS)
    except FileExistsError:
        pass

    clear_folder(BUILD)
    clear_folder(BUILT_GRAPHICS)
    
    img_globs = {
        './**/*.png',
        './**/*.bmp'
    }

    frame_data =[]

    for globstr in img_globs:
        files = glob(os.path.join(ROOT, globstr), recursive=True)
        for file in files:
            process_img(file, frame_data)

    #array must be sorted by 
    frame_data = sorted(frame_data, key=lambda x: x['frame_start']) 

    #create frame_data c files from populated frame_data variable
    includes = ""
    drawing_keyframes = "struct drawing_keyframe drawing_keyframes[] = {"
    for frame in frame_data:
        drawing_keyframes += Template("""{  
            .bgid=$bgid,
            .asset=ASSET($bgid,$frame_start),
            .frame_start=$frame_start
        },""").substitute(frame)
        includes += Template("""#include "../built_graphics/bg${bgid}_${frame_start}.h"
""").substitute(frame)
    drawing_keyframes+="END_DRAWING_FRAME};"

    body = dict(body=includes+drawing_keyframes)
    config_c_contents = Template("""
#ifndef CONFIG_C
#define CONFIG_C

#include "config.h"

$body

#endif
""").substitute(body)

    config_c = open("src/config/config.c", "w")
    config_c.write(config_c_contents)
    config_c.close()

    globs = {
        '**/*.c': process_c
    }
 
    # Gather source files and process them
    objects = itertools.starmap(run_glob, globs.items())
 
    # Link and extract raw binary
    linked = link(itertools.chain.from_iterable(objects))
    objcopy(linked)
	
	#ARMPIS
    cmd = ['armips', './src/main.s','-sym','symbols.txt', '-equ', 'freespace', customAddr]
    run_command(cmd)
    print("DONE")
 
if __name__ == '__main__':
    main()
