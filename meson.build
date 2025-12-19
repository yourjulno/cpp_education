project('cpp_education', 'cpp',
  version: '1.0.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++23',
    'default_library=static',
  ],
)

inc = []

benchmark_dep = dependency('benchmark',
  required: true,
  fallback: ['google-benchmark', 'benchmark_dep']
)

gtest_dep = dependency('gtest',
  required: true,
  fallback: ['gtest', 'gtest_dep']
)

bench_sources = []
foreach f : ['*_bench.cpp', 'bench.cpp']
  bench_files = run_command('find', 'test_3', '-maxdepth', '1', '-name', f,
    check: false).stdout().strip().split('\n')
  foreach file : bench_files
    if file != ''
      bench_sources += file
    endif
  endforeach
endforeach

foreach bench_src : bench_sources
  name = bench_src.split('/')[-1].split('.cpp')[0]
  exe = executable(name,
    bench_src,
    include_directories: inc,
    dependencies: [benchmark_dep],
    install: false
  )
  benchmark(name, exe)
endforeach

unit_sources = []
all_files = run_command('find', 'test_3', '-maxdepth', '1', '-name', '*.cpp',
  check: false).stdout().strip().split('\n')

foreach file : all_files
  if file == ''
    continue
  endif

  filename = file.split('/')[-1]
  if filename.endswith('_bench.cpp') or filename == 'bench.cpp'
    continue
  endif

  unit_sources += file
endforeach

foreach unit_src : unit_sources
  name = unit_src.split('/')[-1].split('.cpp')[0]
  exe = executable(name,
    unit_src,
    include_directories: inc,
    dependencies: [gtest_dep],
    install: false
  )
  test(name, exe)
endforeach