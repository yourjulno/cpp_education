test_sources = run_command('find', '.', '-name', '*_test.cpp', check: false).stdout().strip().split('\n')


foreach source_file : test_sources
  if source_file != ''
    file_name = source_file.split('/')[-1].split('.cpp')[0]
    exe_name = 'gtest_' + file_name

    test_exe = executable(exe_name,
      source_file,
      dependencies: [gtest_main_dep],
      pie: false,
      install: false)

    test(exe_name, test_exe)
  endif
endforeach