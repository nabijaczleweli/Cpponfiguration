#!/usr/bin/env python


from datetime import datetime
from os import listdir


def options(opts):
	opts.load('compiler_cxx')

def configure(conf):
	conf.load('compiler_cxx')
	conf.env.append_value('INCLUDES', ['.', '../include/cpponfig', '../Catch/include'])
	conf.check(features='cxx cxxprogram', cxxflags=['-std=c++14', '-Wall', '-Wextra', '-O3', '-pedantic', '-pipe'], uselib_store='M')
	conf.check(features='cxx cxxprogram', function_name='std::put_time<char>', header_name='iomanip', use='M', mandatory=False)
	conf.define('CPPONFIGURATION_VERSION', datetime.utcnow().strftime('%Y.%m.%d %H:%M:%S UTC'))
	conf.write_config_header('config.h')

def build(buld):
	buld(features='cxx cxxstlib', source=buld.path.ant_glob('src/**/*.cpp'), target='cpponfig', use='M')
	if listdir('Catch'):
		buld(features='cxx cxxprogram', source=buld.path.ant_glob('test/**/*.cpp'), target='cpponfig_test', use=['M', 'cpponfig'])
