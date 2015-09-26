#!/usr/bin/env python3


import glob


def options(opts):
	opts.load('compiler_cxx')
	opts.add_option('--build-test', dest='build_test', default=False, action='store_true', help='build tests')

def configure(conf):
	conf.load('compiler_cxx')
	conf.check(features='cxx cxxprogram', cxxflags=['-std=c++14', '-Wall', '-Wextra', '-O3', '-pedantic', '-pipe'], uselib_store='M')

def build(buld):
	buld(features='cxx cxxstlib', source=glob.glob('src/*.cpp') + glob.glob('src/*/*.cpp'), target='cpponfig', use='M')
	if buld.options.build_test:
		buld(features='cxx cxxprogram', source=glob.glob('test/*.cpp'), target='test', use=['M', 'cpponfig'], cxxflags=['-I../include/cpponfig'])