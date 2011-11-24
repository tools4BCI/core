toolbox = tobicoremat
matpath = ${DESTDIR}${pkgdatadir}/$(toolbox)/
matfiles = ${top_srcdir}/mex/$(toolbox)/*.m
	
EXTRA_DIST = ${matfiles}

install-data-local:
	ls -1
	mkdir -p ${matpath}
	echo "$(matfiles)"
	cp $(matfiles) ${matpath}

uninstall-local:
	rm -rf ${matpath}
