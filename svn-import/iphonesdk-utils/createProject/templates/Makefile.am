TEMPLATES = Application \
	    Command_line_utility \
	    Example_Player	\
	    Example_UICatalog	\
	    Notification_Center_widget \
			Dynamic_Library \
			Static_Library \
			Dynamic_Framework \
			Static_Framework

TEMPLATE_FILES = $(TEMPLATES:%=%.template)

templatedir = $(TEMPLATES_DIR)
template_DATA = $(TEMPLATE_FILES)
%.template:%
	@cd $(abs_srcdir)/$^; tar --exclude-vcs -zcf $(abs_builddir)/$@ *
	@echo $@ genarated.

EXTRA_DIST = $(TEMPLATE_FILES)
CLEANFILES = $(TEMPLATE_FILES)
DISTCLEANFILES = $(TEMPLATE_FILES)
