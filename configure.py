import glob
import os
import ninja_syntax
import sys
import subprocess
import time

red_text = "\033[91m"
green_text = "\033[92m"
yellow_text = "\033[93m"
magenta_text = "\033[95m"
cyan_text = "\033[96m"
white_text = "\033[97m"
reset = "\033[0m"

start_time = time.time()

dir_path = "src/"
asm_path = "asm/"
assets_path = "assets/"
build_dir = "build/"

opt_flag = "-O2"

compile_command = "mips64-elf-gcc -D_LANGUAGE_C -Wall -O2 -mtune=vr4300 -march=vr4300 -mabi=32 -fno-builtin -fomit-frame-pointer -G0 -c"

c_files = glob.glob(f"{dir_path}/**/*.c", recursive=True)
s_files = glob.glob(f"{asm_path}/**/*.s", recursive=True)
bin_files = glob.glob(f"{assets_path}/**/*.bin", recursive=True)

if not os.path.exists(build_dir):
	os.makedirs(build_dir)

if not os.path.exists("rom/bk.z64"):
	print("bk.z64 not found! Place a .z64 rom at rom/bk.z64")
	sys.exit()

"""
//Automatically generated by configure.py, do not edit
.n64 // Let armips know we're coding for the N64 architecture
.open "rom/bk.z64", "rom/bk.mod.z64", 0 // Open the ROM file
.include "asm/symbols.asm"
.close
"""

object_file_header = """//Auto generated by configure.py
.headersize 0x80400000 - 0x1000000
.org 0x80400000
PAYLOAD_START:\n"""

object_file_base = """
hookDecompress:
ADDIU sp, sp, -0x20
SW ra, 0x0018 (sp)

LI a0, rarezip_uncompress
LI a1, rarezip_uncompress_hook
JAL hookCode
NOP

LW ra, 0x0018 (sp)
JR ra
ADDIU sp, sp, 0x20

hookedBootCode:
JAL hookDecompress
NOP
LUI t9, 0x8024
ADDIU t9, t9, 0xDA20
JALR t9, ra
LW a0, 0x0020 (sp)
LW ra, 0x0014 (sp)
JR ra
ADDIU sp, sp, 0x20\n
"""

with open("asm/object_files.asm", "w") as file:
	file.write(object_file_header)
	for c_file in c_files:
		if (os.path.basename(c_file) == "dma.c"):
			continue
		try:
			build_file_path = f"{build_dir}{os.path.basename(c_file.replace('.c', '.o'))}"
			# print(build_file_path)
			command = f"{compile_command} {c_file} -o {build_file_path}"
			print(f"{cyan_text}{command}")
			subprocess.run( f"{command}", shell=True, check=True)
			# print(f"{c_file} compiled")
			file.write(f'.importobj \"{build_file_path}\"\n') # .c -> .o
		except subprocess.CalledProcessError as e:
			print(e.stderr)
	file.write(object_file_base)
	file.write(".align 8\nPAYLOAD_END:\n")
	

print(f"{green_text}armips asm/main.asm")
subprocess.run("armips asm/main.asm")
print(f"{magenta_text}n64crc \"rom/bk.mod.z64\"")
subprocess.run(f"n64crc \"rom/bk.mod.z64\"")

end_time = time.time()
execution_time = end_time - start_time
print(f"{reset}Compile time: {execution_time:.2f} seconds")