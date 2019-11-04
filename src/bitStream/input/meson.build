
input_files = files(['BitInputStream.cpp', 'BitInputStream.hpp'])
input_includes = include_directories('.')
bit_input = library('bit_input', sources: input_files)
bit_input_dep = declare_dependency(include_directories: input_includes,
   link_with: bit_input)