## Richard Allen <rsaxvc@rsaxvc.net>
## Helder Correia <helder.correia@netcabo.pt>

bin_PROGRAMS = ioload

ioload_SOURCES = \
	dataframe.h										\
	device.cpp device.h								\
	devreader.cpp devreader.h						\
	devreaderfactory.cpp devreaderfactory.h			\
	form_field.cpp form_field.h						\
	graph.cpp graph.h								\
	main.cpp main.h									\
	opt_window.cpp opt_window.h						\
	screen.cpp screen.h								\
	setting.cpp setting.h							\
	settingfilter.cpp settingfilter.h               \
	settingstore.cpp settingstore.h					\
	statistics.cpp statistics.h						\
	stringutils.cpp stringutils.h					\
	traffic_window.cpp traffic_window.h				\
	window.cpp window.h

if HAVE_LINUX
ioload_SOURCES += \
	devreader-linux.cpp devreader-linux.h			\
	devreader-linux-proc.cpp devreader-linux-proc.h
endif

