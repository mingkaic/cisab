
######## COVERAGES ########

TMP_COVFILE := /tmp/coverage.info
COVERAGE_INFO_FILE := bazel-out/_coverage/_coverage_report.dat
CCOVER := bazel coverage --config asan --action_env="ASAN_OPTIONS=detect_leaks=0" --config gtest --config cc_coverage
COVERAGE_CTX := tmp/cisab_coverage
CONVERSION_CSV := tmp/cisab_conversions.csv

.PHONY: cov_clean
cov_clean:
	rm *.info
	rm -Rf html

# coverage helper
.PHONY: cov_init
cov_init:
	rm -Rf tmp
	mkdir -p $(COVERAGE_CTX)
	find . -maxdepth 1 | grep -E -v 'tmp|\.git|bazel-' | tail -n +2 | xargs -i cp -r {} $(COVERAGE_CTX)
	find $(COVERAGE_CTX) | grep -E '\.cc|\.h' | python3 scripts/label_uniquify.py $(COVERAGE_CTX) > $(CONVERSION_CSV)
	find $(COVERAGE_CTX) | grep -E '\.yml' | python3 scripts/yaml_replace.py $(CONVERSION_CSV)

.PHONY: cov_copyout
cov_copyout:
	python3 scripts/label_replace.py $(COVERAGE_CTX)/$(COVFILE) $(CONVERSION_CSV) > $(COVFILE)

.PHONY: cov_genhtml
cov_genhtml: cov_copyout
	genhtml -o html $(COVFILE)

.PHONY: clean_test_coverage
clean_test_coverage: ${COVERAGE_INFO_FILE}
	lcov --remove ${COVERAGE_INFO_FILE} '**/test/*' '**/mock/*' '**/*.pb.*' -o ${TMP_COVFILE}

coverage.info:
	$(CCOVER) //...
	@make clean_test_coverage
	lcov --extract ${TMP_COVFILE} 'fmts/*' 'logs/*' 'types/*' -o coverage.info

###### INDIVIDUAL COVERAGES ######

modules_coverage.info: fmts_coverage.info logs_coverage.info types_coverage.info
	lcov \
		-a fmts_coverage.info \
		-a logs_coverage.info \
		-a types_coverage.info \
		-o modules_coverage.info

fmts_coverage.info:
	$(CCOVER) --instrumentation_filter 'fmts/*' //fmts:test
	@make clean_test_coverage
	lcov -a ${TMP_COVFILE} -o fmts_coverage.info

logs_coverage.info:
	$(CCOVER) --instrumentation_filter 'logs/*' //logs:test
	@make clean_test_coverage
	lcov -a ${TMP_COVFILE} -o logs_coverage.info

types_coverage.info:
	$(CCOVER) --instrumentation_filter 'types/*' //types:test
	@make clean_test_coverage
	lcov -a ${TMP_COVFILE} -o types_coverage.info

VERSION := $(shell cat VERSION)

.PHONY: conan_remote
conan_remote:
	conan remote add mingkaic-co "https://gitlab.com/api/v4/projects/23299689/packages/conan"

build/conanbuildinfo.cmake:
	conan install -if build .

.PHONY: conan_install
conan_install: build/conanbuildinfo.cmake

.PHONY: conan_build
conan_build: build/conanbuildinfo.cmake
	conan build -bf build .

.PHONY: conan_create
conan_create:
	conan create . mingkaic-co/stable

.PHONY: conan_upload
conan_upload:
	conan upload cisab/${VERSION}@mingkaic-co/stable --all --remote mingkaic-co

.PHONY: conan_create_n_upload
conan_create_n_upload: conan_install conan_create conan_upload

#### compile db (uncomment to generate)

#EXEC_ROOT := $(shell bazel info execution_root)

#COMP_FILE := $(shell bazel info bazel-bin)/compile_commands.json

#.PHONY: compdb
#compdb:
	#bazel build //:compdb
	#sed -i.bak "s@__EXEC_ROOT__@${EXEC_ROOT}@" "${COMP_FILE}"
	#ln -s "${COMP_FILE}" compile_commands.json
