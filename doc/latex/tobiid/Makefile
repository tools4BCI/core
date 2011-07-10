# Edited by M. Tavella <michele.tavella@epfl.com> on 09/04/09 12:55:43

TARGET=libtobiid
INCLUDES_TEX=$(wildcard include/*.tex)
INCLUDES_FIG=$(wildcard images/*)
INCLUDES_TAB=$(wildcard tables/*)
INCLUDES_BIB=$(wildcard bibliography/*.bib)
TARGET_TEX=${TARGET}.tex
TARGET_PDF=${TARGET}.pdf
TARGET_PS=${TARGET}.ps
DATE=`date +%Y%m%d%H%M%S `

default: ${TARGET_PDF}
all: ${TARGET_PS}

pdf: ${TARGET_PDF}
ps: ${TARGET_PS}

${TARGET_PDF}: ${TARGET_TEX} ${INCLUDES_TEX} ${INCLUDES_BIB} ${INCLUDES_FIG} ${INCLUDES_TAB}
	latexmk -pv- -pdf ${TARGET}
	@if [ `pidof xpdf` ]; then xpdf -remote 127.0.0.1 -reload; fi

clean:
	@rm -f ${TARGET_PDF}
	@rm -f ${TARGET_PS}
	@rm -f ${TARGET}.aux*
	@rm -f ${TARGET}.bbl*
	@rm -f ${TARGET}.blg*
	@rm -f ${TARGET}.dvi*
	@rm -f ${TARGET}.log*
	@rm -f ${TARGET}.nav*
	@rm -f ${TARGET}.out*
	@rm -f ${TARGET}.snm*
	@rm -f ${TARGET}.toc*

${TARGET_PS}: ${TARGET_PDF}
	pdf2ps $?

final: ${TARGET_PS}
	@mv ${TARGET_PDF} ${DATE}_${TARGET_PDF}
	@mv ${TARGET_PS} ${DATE}_${TARGET_PS}

view: ${TARGET_PDF}
	@evince $?

xview: ${TARGET_PDF}
	@xpdf -remote 127.0.0.1 $?

edit:
	@e1 ${TARGET_TEX} ${INCLUDES_TEX} ${INCLUDES_BIB} &
	
sedit:
	@e1 -S session.vim &

kill:
	@killall pdflatex 
