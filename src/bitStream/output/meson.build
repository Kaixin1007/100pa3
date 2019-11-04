output_files = files(['BitOutputStream.cpp', 'BitOutputStream.hpp'])
output_includes = include_directories('.')
bit_output = library('bit_output', sources: output_files)
bit_output_dep = declare_dependency(include_directories: output_includes, link_with: bit_output)