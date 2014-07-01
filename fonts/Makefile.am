include $(top_srcdir)/config/common.am

EXTRA_DIST = *.ttf AUTHORS LICENSE README

# Only install fonts if they don't already exist

install-data-local:
	all=$$(fc-list : file); \
	needed=""; \
	for ttf in *.ttf; do \
		pat="/$$ttf:"; \
		if [ "$${all#*$$pat}" = "$$all" ]; then \
			needed="$$needed $$ttf"; \
		fi; \
	done; \
	if [ ! -z "$$needed" ]; then \
		$(install_sh) -m 755 -d $(prefix)/share/fonts/truetype/Xforms; \
		$(install_sh) -m 644 $$needed $(prefix)/share/fonts/truetype/Xforms; \
		fc-cache -f $(prefix)/share/fonts/truetype/XForms; \
	fi

uninstall-local:
	rm -rf $(prefix)/share/fonts/truetype/XForms
