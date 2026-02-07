project('cpp_education', 'cpp',
  version: '1.0.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++23',
    'default_library=static',
    'b_pie=false',
  ],
)

fs = import('fs')

# зависимости для gtest
gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_dep')

threads_dep = dependency('threads', required: false)

source_dirs = [
  '2nd_semester',
  'test_2',
  'test_3',
  'tests',
]

foreach d : source_dirs
  if not fs.is_dir(d)
    continue
  endif

  cpp_files = run_command(
    'find', d,
    '-maxdepth', '1',
    '-type', 'f',
    '-name', '*.cpp',
    check: false
  ).stdout().strip().split('\n')

  if cpp_files.length() == 1 and cpp_files[0] == ''
    continue
  endif

  # bench.cpp (без main) -> библиотека, если есть
  bench_path = d + '/bench.cpp'
  have_bench = fs.exists(bench_path)

  bench_lib = []
  if have_bench
    bench_lib = static_library(d + '_bench_support',
      bench_path,
      install: false
    )
  endif

  foreach f : cpp_files
    if f == ''
      continue
    endif

    filename = f.split('/')[-1]

    # bench.cpp сам по себе не executable
    if filename == 'bench.cpp'
      continue
    endif

    base = filename.split('.cpp')[0]
    target = d + '_' + base

    link_with_libs = []
    deps = []

    # если это *_bench.cpp и есть bench.cpp -> подлинкуем
    if have_bench and filename.endswith('_bench.cpp')
      link_with_libs = [bench_lib]
    endif

    # если это тест (папка tests ИЛИ имя *_test.cpp) -> линкуем gtest
    if d == 'tests' or filename.endswith('_test.cpp')
      if gtest_dep.found()
        deps += [gtest_dep]
      endif
      if threads_dep.found()
        deps += [threads_dep]
      endif
    endif

    executable(target,
      f,
      dependencies: [gtest_dep, dependency('threads')],
      install: false
    )
  endforeach
endforeach
