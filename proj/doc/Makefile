ROOT_DIR := ..
DOXYFILE := $(ROOT_DIR)/Doxyfile
DOXYGEN_OUT := doxygen
REPORT_TEX := report.tex
REPORT_PDF := report.pdf
REFMAN_PDF := $(DOXYGEN_OUT)/latex/refman.pdf

all: $(REPORT_PDF) $(REFMAN_PDF)

$(REPORT_PDF): $(REPORT_TEX)
	pdflatex $(REPORT_TEX)

$(REFMAN_PDF): $(DOXYFILE)
	cd $(ROOT_DIR) && doxygen Doxyfile
	$(MAKE) -C $(DOXYGEN_OUT)/latex
	cp $(REFMAN_PDF) .

clean:
	rm -f *.aux *.log *.out *.toc *.synctex.gz *.lof *.lot *.nav *.snm
	rm -f $(REPORT_PDF) doxygen.pdf
	[ -d $(DOXYGEN_OUT) ] && $(MAKE) -C $(DOXYGEN_OUT)/latex clean || true

mrproper: clean
	rm -f $(REPORT_PDF) $(REFMAN_PDF) refman.pdf
