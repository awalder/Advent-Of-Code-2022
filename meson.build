project(
  'AOC2022',
  'cpp',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20',
    ]
)

FS = import('fs')
FMT = dependency('fmt')

foreach num : [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24]
  dir = f'day@num@'
  if FS.is_dir(dir)
    subdir(dir)
  endif

endforeach

# subdir('day1')
# subdir('day2')
# subdir('day3')
# subdir('day4')
# subdir('day5')
# subdir('day6')
# subdir('day7')
# subdir('day8')
# subdir('day9')
# subdir('day10')
# subdir('day11')
# subdir('day12')
# subdir('day13')
# subdir('day14')
# subdir('day15')
