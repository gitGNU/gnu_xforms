include $(top_srcdir)/config/common.am

EXTRA_DIST = CHANGES  COPYING README *.otf

install-data-local:
	if [ ! -e $(prefix)/share/fonts/Xforms/FLP_Neu ]; then \
		$(install_sh) -m 755 -d $(prefix)/share/fonts/Xforms/FLP_Neu; \
	fi
	$(install_sh) -m 644 *.otf $(prefix)/share/fonts/Xforms/FLP_Neu
