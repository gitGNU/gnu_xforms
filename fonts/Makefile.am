include $(top_srcdir)/config/common.am

SUBDIRS = DejaVu FPL_Neu

install-data-local:
	fc-cache -vf

uninstall-local:
	rm -rf $(prefix)/share/fonts/XForms
