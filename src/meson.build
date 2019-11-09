subdir('encoder')
subdir('bitStream')

utils = library('utils', sources: ['FileUtils.hpp'])

inc = include_directories('.')

utils_dep = declare_dependency(include_directories: inc,
  link_with: utils)

HCTree = library('HCTree', sources: HCTree_files,dependencies : [ bit_output_dep, bit_input_dep])
HCTree_dep = declare_dependency(include_directories: HCTree_includes,
   link_with: HCTree)
   
compress_exe = executable('compress.cpp.executable', 
    sources: ['compress.cpp'],
    dependencies : [bit_input_dep, bit_output_dep, HCTree_dep,utils_dep,cxxopts_dep],
    install : true)

uncompress_exe = executable('uncompress.cpp.executable', 
    sources: ['uncompress.cpp'],
    dependencies : [bit_input_dep, bit_output_dep, HCTree_dep,utils_dep,cxxopts_dep],
    install : true)