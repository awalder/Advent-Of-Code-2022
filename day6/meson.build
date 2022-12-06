executable('day6_part1', ['part1.cpp'], dependencies : [FMT])
executable('day6_part2', ['part2.cpp'], dependencies : [FMT])

executable(
  'day6_benchmark',
  sources : ['benchmark.cpp'],
  dependencies : [FMT, BENCHMARK, TRACY],
  # cpp_args : ['-DTRACY_ENABLE']
)
