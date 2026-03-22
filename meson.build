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
cmake = import('cmake')

threads_dep = dependency('threads', required: false)

# ---- googletest (CMake subproject) ----
gtest_opts = cmake.subproject_options()
gtest_opts.add_cmake_defines({
                               'BUILD_GTEST': 'ON',
                               'BUILD_GMOCK': 'OFF',
                               'INSTALL_GTEST': 'OFF',
                               'BUILD_TESTING': 'OFF',
                             })

gtest_sp = cmake.subproject('gtest', options: gtest_opts)
gtest_dep = gtest_sp.dependency('gtest')

# ---- google benchmark (CMake subproject) ----
bench_opts = cmake.subproject_options()
bench_opts.add_cmake_defines({
                               'BENCHMARK_ENABLE_TESTING': 'OFF',
                               'BENCHMARK_ENABLE_GTEST_TESTS': 'OFF',
                               'BENCHMARK_DOWNLOAD_DEPENDENCIES': 'ON',
                             })

bench_sp = cmake.subproject('google-benchmark', options: bench_opts)
benchmark_dep = bench_sp.dependency('benchmark')

# ---- boost ----
boost_dep = dependency('boost', required: false)

if not boost_dep.found()
  boost_proj = subproject('boost')
  boost_dep = boost_proj.get_variable('boost_dep')
endif

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

  bench_path = d + '/bench.cpp'
  have_bench_support = fs.exists(bench_path)

  bench_support_lib = []
  if have_bench_support
    bench_support_deps = [boost_dep, benchmark_dep]
    if threads_dep.found()
      bench_support_deps += [threads_dep]
    endif

    bench_support_lib = static_library(
      d + '_bench_support',
      bench_path,
      dependencies: bench_support_deps,
      install: false
    )
  endif

  foreach f : cpp_files
    if f == ''
      continue
    endif

    filename = f.split('/')[-1]

    # bench.cpp не собираем как отдельный executable
    if filename == 'bench.cpp'
      continue
    endif

    base = filename.split('.cpp')[0]
    target = d + '_' + base

    is_test = (d == 'tests') or filename.endswith('_test.cpp')
    is_bench = filename.endswith('_bench.cpp')

    deps = [boost_dep]
    link_with_libs = []

    if is_test
      deps += [gtest_dep]
      if threads_dep.found()
        deps += [threads_dep]
      endif
    endif

    if is_bench
      deps += [benchmark_dep]
      if threads_dep.found()
        deps += [threads_dep]
      endif

      if have_bench_support
        link_with_libs += [bench_support_lib]
      endif
    endif

    executable(
      target,
      f,
      dependencies: deps,
      link_with: link_with_libs,
      install: false
    )
  endforeach
endforeach
