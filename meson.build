project('cpp_education', 'cpp',
  version: '1.0.0',
  default_options: [
    'warning_level=3',
    'cpp_std=c++23',
    'default_library=static'
  ]
)

gtest_proj = subproject('gtest', required: true, default_options: ['default_library=static'])
gtest_main_dep = gtest_proj.get_variable('gtest_main_dep')

gtest_dep = dependency('gtest',
  main: true,
  required: false,
  fallback: ['gtest', 'gtest_main_dep'],
  default_options: ['default_library=static'])

# test_3 файлы
test_3_sources = run_command('find', 'test_3', '-name', '*.cpp', check: false).stdout().strip().split('\n')
foreach source_file : test_3_sources
  if source_file != ''
    file_name = source_file.split('/')[-1].split('.cpp')[0]
    exe = executable(file_name, source_file, install: false)
    test(file_name, exe)
  endif
endforeach

subdir('tests')