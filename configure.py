import glob
import os
import ninja_syntax
import sys

dir_path = "src/"
asm_path = "asm/"
assets_path = "assets/"
build_dir = "build/"

c_files = glob.glob(f"{dir_path}/**/*.c", recursive=True)
s_files = glob.glob(f"{asm_path}/**/*.s", recursive=True)
bin_files = glob.glob(f"{assets_path}/**/*.bin", recursive=True)


if not os.path.exists(build_dir):
	os.makedirs(build_dir)

if not os.path.exists("rom/bk.z64"):
	print("bk.z64 not found! Place a .z64 rom at rom/bk.z64")
	sys.exit()

