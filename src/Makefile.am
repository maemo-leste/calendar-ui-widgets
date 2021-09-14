lib_LTLIBRARIES = libcalendarui-widgets.la

libcalendarui_widgets_la_CFLAGS = \
		$(HILDON_CFLAGS) -I$(srcdir)/../include

libcalendarui_widgets_la_LDFLAGS = \
		-Wl,--as-needed $(HILDON_LIBS) -Wl,--no-undefined

libcalendarui_widgets_la_SOURCES = \
		$(calendarui_widgets_built_sources) \
		pip-color-picker.c \
		pip-calendar-color.c

calendarui_widgets_public_headers = \
		calendar-ui-widgets.h \
		helper-widgets.h \
		PipCalendarColor.h \
		PipCalendarDialog.h \
		PipColorPicker.h

calendarui_widgets_built_public_headers = \
		calendar-widgets-enum-gen.h

calendarui_widgets_built_sources = \
		calendar-widgets-enum-gen.c

calendarui_widgetsinclude_HEADERS = \
		$(calendarui_widgets_public_headers) \
		$(calendarui_widgets_built_public_headers)

calendarui_widgetsincludedir = $(pkgincludedir)

calendar-widgets-enum-gen.h: $(calendarui_widgets_public_headers)
	(cd $(srcdir) && $(GLIB_MKENUMS) --identifier-prefix Pip \
	--symbol-prefix PIP --template calendar-widgets-enum-gen.h.in \
	--output $@ \
	$(calendarui_widgets_public_headers))

calendar-widgets-enum-gen.c: $(calendarui_widgets_public_headers)
	(cd $(srcdir) && $(GLIB_MKENUMS) --output $@ \
	--template calendar-widgets-enum-gen.c.in \
	$(calendarui_widgets_public_headers))


BUILT_SOURCES = \
	$(calendarui_widgets_built_sources) \
	$(calendarui_widgets_built_public_headers)

CLEANFILES = $(BUILT_SOURCES)

MAINTAINERCLEANFILES = Makefile.in

