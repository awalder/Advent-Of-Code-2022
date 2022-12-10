project(
  'AOC2022',
  'cpp',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20',
    ]
)

cmake = import('cmake')
benchmark_opts = cmake.subproject_options()
benchmark_opts.add_cmake_defines({'BENCHMARK_ENABLE_GTEST_TESTS': false})
BENCHMARK = cmake.subproject('google-benchmark', options : benchmark_opts).dependency('benchmark')
RANGES = subproject('range-v3').get_variable('range_dep')

FS = import('fs')
FMT = dependency('fmt')
TRACY = subproject('tracy').get_variable('tracy_dep')

foreach num : [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]
  dir = f'day@num@'
  if FS.is_dir(dir)
    subdir(dir)
  endif

endforeach

