include $(top_srcdir)/config/common.am

EXTRA_DIST = *.ttf


install-data-local:
	if [ ! -e $(prefix)/share/fonts/Xforms/DejaVu ]; then \
		$(install_sh) -m 755 -d $(prefix)/share/fonts/Xforms/DejaVu; \
	fi
	$(install_sh) -m 644 *.ttf $(prefix)/share/fonts/Xforms/DejaVu
