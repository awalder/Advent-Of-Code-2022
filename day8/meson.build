executable('day8_part1', ['part1.cpp'], dependencies : [FMT, RANGES])
executable('day8_simd', ['part1_simd.cpp'], dependencies : [FMT], cpp_args : ['-march=native'])
executable('day8_part2', ['part2.cpp'], dependencies : [FMT, RANGES])

executable(
  'day8_benchmark',
  sources : ['benchmark.cpp'],
  dependencies : [RANGES, FMT, BENCHMARK],
)
