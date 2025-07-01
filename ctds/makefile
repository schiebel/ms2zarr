# cd /home/zuul06-2/dschieb/casa/dedicated-table-sys/casacore/BUILD/tables && ccache /opt/rh/gcc-toolset-11/root/usr/bin/c++ -DCFITSIO_VERSION_MAJOR=3 -DCFITSIO_VERSION_MINOR=470 -DHAVE_FFTW3 -DHAVE_FFTW3_THREADS -DHAVE_O_DIRECT -DHAVE_READLINE -DUSE_THREADS -DWCSLIB_VERSION_MAJOR=7 -DWCSLIB_VERSION_MINOR=12 -Dcasa_tables_EXPORTS -I/home/zuul06-2/dschieb/casa/dedicated-table-sys/casacore -I/home/zuul06-2/dschieb/casa/dedicated-table-sys/casacore/BUILD -I/usr/include/cfitsio -I/home/zuul06-2/dschieb/casa/dedicated-table-sys/casacore/BUILD/tables -fcx-fortran-rules -Wextra -Wall -W -Wpointer-arith -Woverloaded-virtual -Wwrite-strings -pedantic -Wno-long-long -fopenmp -pthread -g -std=gnu++17 -fPIC -MD -MT tables/CMakeFiles/casa_tables.dir/Tables/BaseTable.cc.o -MF CMakeFiles/casa_tables.dir/Tables/BaseTable.cc.o.d -o CMakeFiles/casa_tables.dir/Tables/BaseTable.cc.o -c /home/zuul06-2/dschieb/casa/dedicated-table-sys/casacore/tables/Tables/BaseTable.cc

QUIET := @
CWD := $(realpath .)
LDFLAGS :=

TSTDIR = testsrc

LIBSRC = $(shell find casacore -name "*.cc")
LIBOBJ = $(patsubst casacore/%.cc, casacore/%.o, $(LIBSRC))

##LIBS = -ldl $(shell pkg-config --cflags xtensor-zarr | perl -pe 's|-I(.*?)/include|-L$$1/lib -Wl,-rpath,$$1/lib|g') -lxtensor-zarr-gdal
##LIBS = -ldl $(shell pkg-config --cflags nlohmann_json | perl -pe 's|.*?-I(.*?)/include.*|-L$$1/lib -Wl,-rpath,$$1/lib|g') $(shell pkg-config --cflags nlohmann_json | perl -pe 's|.*?-I(.*?)/include.*|-L$$1/lib/tensorstore -Wl,-rpath,$$1/lib/tensorstore -L$$1/lib/tensorstore/driver/zarr3 -Wl,-rpath,$$1/lib/tensorstore/driver/zarr3|g') -ltensorstore -lmetadata -ldata_type
##LIBS = -ldl $(shell pkg-config --cflags nlohmann_json | perl -pe 's|.*?-I(.*?)/include.*|-L$$1/lib -Wl,-rpath,$$1/lib|g') -ltensorstore_complete
##LIBS = -ldl -L/home/zuul06/conda/envs/zarr-c++ts/lib -Wl,-rpath,/home/zuul06/conda/envs/zarr-c++ts/lib -ltensorstore_complete -L/home/zuul06/conda/envs/zarr-c++ts/lib/abseil -Wl,-rpath,/home/zuul06/conda/envs/zarr-c++ts/lib/abseil
#LIBS = -ldl -L/home/zuul06/conda/envs/zarr-c++ts/lib -Wl,-rpath,/home/zuul06/conda/envs/zarr-c++ts/lib -ltensorstore_complete -labsl_status -labsl_log_internal_message -labsl_strings -labsl_base

INC = -I$(CWD)
#INC = -I. $(shell pkg-config --cflags xtensor-zarr)
#INC = -I. -I/home/zuul06/conda/envs/zarr-c++ts/include -I/home/zuul06/conda/envs/zarr-c++ts/include/half_float
#INC = -I. -I/home/zuul06/conda/envs/zarr-c++ts/include -I/home/zuul06/conda/envs/zarr-c++ts/include/half_float

CCACHE = ccache
#CXXFLAGS = -g $(INC) -Dcasacore=ctds -DCFITSIO_VERSION_MAJOR=3 -DCFITSIO_VERSION_MINOR=470 -DHAVE_FFTW3 -DHAVE_FFTW3_THREADS -DHAVE_O_DIRECT -DHAVE_READLINE -DUSE_THREADS -DWCSLIB_VERSION_MAJOR=7 -DWCSLIB_VERSION_MINOR=12 -Dcasa_tables_EXPORTS -fopenmp -pthread -g -std=gnu++17 -fPIC -g
#CXXFLAGS = -g $(INC) -Dcasacore=ctds -D_GLIBCXX_USE_CXX11_ABI=1 -DCFITSIO_VERSION_MAJOR=3 -DCFITSIO_VERSION_MINOR=470 -DHAVE_FFTW3 -DHAVE_FFTW3_THREADS -DHAVE_O_DIRECT -DHAVE_READLINE -DUSE_THREADS -DWCSLIB_VERSION_MAJOR=7 -DWCSLIB_VERSION_MINOR=12 -Dcasa_tables_EXPORTS -fopenmp -pthread -g -std=gnu++17 -fPIC -g
CXXFLAGS = -O2 $(INC) -Dcasacore=ctds -DCFITSIO_VERSION_MAJOR=3 -DCFITSIO_VERSION_MINOR=470 -DHAVE_FFTW3 -DHAVE_FFTW3_THREADS -DHAVE_O_DIRECT -DHAVE_READLINE -DUSE_THREADS -DWCSLIB_VERSION_MAJOR=7 -DWCSLIB_VERSION_MINOR=12 -Dcasa_tables_EXPORTS -fopenmp -pthread -std=gnu++17 -fPIC


RUN = $(patsubst $(TSTDIR)/%.run, %, $(wildcard $(TSTDIR)/*.run))
CMP = $(filter-out $(RUN), $(patsubst $(TSTDIR)/%.out, %, $(wildcard $(TSTDIR)/*.out)))
STA = $(filter-out $(RUN) $(CMP), $(patsubst $(TSTDIR)/%.cc, %, $(wildcard $(TSTDIR)/*.cc)))

RUNTGT = $(patsubst %, %.run, $(RUN))
CMPTGT = $(patsubst %, %.cmp, $(CMP))
STATGT = $(patsubst %, %.sta, $(STA))

TEST_EXCLUDE_START_PATTERN = ^>>>
TEST_EXCLUDE_END_PATTERN = ^<<<

all: RAW-CASACORE libctds.so

test: CLEANTEST $(RUNTGT) $(CMPTGT) $(STATGT)

clean:
	$(QUIET)rm -f $(RUN) $(CMP) $(STA) || true
	$(QUIET)rm -rf $(patsubst %, %.o, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tbl, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.ex1, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.sel, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.ism, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.data, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.data1, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.data2, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.data2a, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.data3, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tab, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tb1, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.newtbl, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tab2, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tabcp, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.conctab, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.dataref, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.datasub, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.datb, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.datb2, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.file, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tabdsc, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp_sub.tabdsc, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.rc, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.log1, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp.tab1, $(RUN) $(CMP) $(STA)) || true
	$(QUIET)rm -rf $(patsubst %, %_tmp1.tabdsc, $(RUN) $(CMP) $(STA)) || true

cleanall: clean
	$(QUIET)rm -f $(LIBOBJ) || true
	$(QUIET)rm -f *~ testsrc/*~ || true
	$(QUIET)rm -rf RAW-CASACORE || true
	$(QUIET)rm -f libctds.so || true

$(RUNTGT): %.run : %
	$(QUIET)testsrcdir=$(CWD)/testsrc testsrc/$@ > $<.actual 2>&1
	$(QUIET)if [ $$? -ne 0 ]; then \
		echo "$< failed."; \
		exit 1; \
	fi
	$(QUIET)sed -i -e 's|$(CWD)/||' $<.actual
	$(QUIET)if [ -f testsrc/$<.out ]; then \
	    sed -e "/$(TEST_EXCLUDE_START_PATTERN)/,/$(TEST_EXCLUDE_END_PATTERN)/d" $<.actual > $<.actual.FILTERED; \
	    sed -e "/$(TEST_EXCLUDE_START_PATTERN)/,/$(TEST_EXCLUDE_END_PATTERN)/d" testsrc/$<.out > $<.out.FILTERED; \
	    diff $<.out.FILTERED $<.actual.FILTERED; \
	    if [ $$? -ne 0 ]; then \
	        echo "$< failed."; \
	        exit 1; \
	    else \
	        echo "$< success."; \
	    fi; \
	    rm $<.actual.FILTERED; \
	    rm $<.out.FILTERED; \
	fi
	$(QUIET)rm $<.actual

$(CMPTGT): %.cmp : %
	$(QUIET)testsrcdir=$(CWD)/testsrc ./$< > $<.actual 2>&1
	$(QUIET)if [ $$? -ne 0 ]; then \
		echo "$< failed."; \
		exit 1; \
	fi
	$(QUIET)sed -i -e 's|$(CWD)/||' $<.actual
	$(QUIET)if [ -f testsrc/$<.out ]; then \
	    sed -e "/$(TEST_EXCLUDE_START_PATTERN)/,/$(TEST_EXCLUDE_END_PATTERN)/d" $<.actual > $<.actual.FILTERED; \
	    sed -e "/$(TEST_EXCLUDE_START_PATTERN)/,/$(TEST_EXCLUDE_END_PATTERN)/d" testsrc/$<.out > $<.out.FILTERED; \
	    diff $<.out.FILTERED $<.actual.FILTERED; \
	    if [ $$? -ne 0 ]; then \
	        echo "$< failed."; \
	        exit 1; \
	    else \
	        echo "$< success."; \
	    fi; \
	    rm $<.actual.FILTERED; \
	    rm $<.out.FILTERED; \
	fi
	$(QUIET)rm $<.actual

$(STATGT): %.sta : %
	$(QUIET)testsrcdir=$(CWD)/testsrc ./$< > $<.actual 2>&1
	$(QUIET)if [ $$? -ne 0 ]; then \
		echo "$< failed."; \
		exit 1; \
	else \
	    echo "$< success."; \
	fi
	$(QUIET)rm $<.actual

%: %.o libctds.so
	$(QUIET)$(CXX) $(CXXFLAGS) -o $@ $< -Wl,-rpath,'$(CWD)' -Wl,--enable-new-dtags -L. -lctds $(LIBS)

objs: $(LIBOBJ)
	@echo Built $@

libctds.so: $(LIBOBJ)
	$(QUIET)echo Linking $@
	$(QUIET)$(CXX) $(LDFLAGS) -shared -o $@ $^

%.o: $(TSTDIR)/%.cc
	$(QUIET)$(CCACHE) $(CXX) $(CXXFLAGS) -Itestsrc -c $< -o $@

casacore/%.o: casacore/%.cc
	$(QUIET)echo Compiling $<
	$(QUIET)$(CCACHE) $(CXX) $(CXXFLAGS) -c $< -o $@

CLEANTEST:
	$(QUIET)rm -rf tConcatTable_tmp.tab

RAW-CASACORE:
	$(QUIET)if [ ! -d "RAW-CASACORE" ]; then \
	    echo "Cloning source from https://github.com/casacore/casacore.git"; \
		git clone https://github.com/casacore/casacore.git RAW-CASACORE; \
	    cd RAW-CASACORE; \
	    git checkout 8e2648a90745a6cd82fd64dbe8662a968b81b54e; \
	    echo "Checked out 8e2648a90745a6cd82fd64dbe8662a968b81b54e"; \
	fi

.SILENT: tTable_1.o tConcatTable.o tTableDesc.o
.PHONY: objs
