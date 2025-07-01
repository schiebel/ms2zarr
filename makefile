CXX := /opt/rh/gcc-toolset-13/root/usr/bin/g++
CC := /opt/rh/gcc-toolset-13/root/usr/bin/gcc

QUIET := 
CCACHE = ccache

ROOT := $(shell realpath .)
HASH := 5f3435aba00bcd7f12062d2e8e1839b4eaf1a575

LIBS := -lctds -ltensorstore_complete -lbase $(CONDA_PREFIX)/lib/libstdc++.so

###########################################################################################################
### The "cpp-half" package is used, but it apparently does not provide a pkg-config package information ###
###########################################################################################################
# cpp-half                  2.1.0                h00ab1b0_1    conda-forge                              ###
###########################################################################################################
INC = -I$(ROOT)/include $(shell pkg-config --cflags-only-I grpc) $(shell pkg-config --cflags-only-I grpc)/half_float
LDFLAGS = -Wl,--enable-new-dtags -Wl,-rpath,'$(ROOT)/lib' -L$(ROOT)/lib -Wl,-rpath,'$(ROOT)/lib/absl' -L$(ROOT)/lib/absl -Wl,-rpath,$(CONDA_PREFIX)/lib -L$(CONDA_PREFIX)/lib $(LIBS)

#CXXFLAGS = $(INC) -Dcasacore=ctds -D_GLIBCXX_USE_CXX11_ABI=1 -fopenmp -pthread -std=gnu++17 -fPIC -O2
CXXFLAGS = $(INC) -Dcasacore=ctds -fopenmp -pthread -std=gnu++17 -fPIC -O2

TENSORSTORE_INC := make_array.inc \
	data_type.h \
	rank.h \
	resize_options.h \
	index_interval.h \
	driver/driver_spec.h \
	driver/write_request.h \
	driver/driver.h \
	driver/read.h \
	driver/read_request.h \
	driver/chunk.h \
	driver/driver_handle.h \
	driver/write.h \
	driver/copy.h \
	open_options.h \
	index_space/output_index_map.h \
	index_space/alignment.h \
	index_space/transformed_array.h \
	index_space/dim_expression.h \
	index_space/output_index_method.h \
	index_space/index_domain.h \
	index_space/dimension_units.h \
	index_space/dimension_index_buffer.h \
	index_space/index_vector_or_scalar.h \
	index_space/internal/index_array_slice_op.h \
	index_space/internal/transpose_op.h \
	index_space/internal/dimension_selection.h \
	index_space/internal/diagonal_op.h \
	index_space/internal/dim_expression_helper.h \
	index_space/internal/transpose.h \
	index_space/internal/mark_explicit_op.h \
	index_space/internal/deep_copy_transform_rep_ptr.h \
	index_space/internal/transform_rep.h \
	index_space/internal/label_op.h \
	index_space/internal/interval_slice_op.h \
	index_space/internal/add_new_dims_op.h \
	index_space/internal/single_index_slice_op.h \
	index_space/internal/inverse_transform.h \
	index_space/internal/transform_array.h \
	index_space/internal/propagate_bounds.h \
	index_space/internal/identity_transform.h \
	index_space/internal/compose_transforms.h \
	index_space/internal/translate_op.h \
	index_space/dimension_identifier.h \
	index_space/index_domain_builder.h \
	index_space/transform_array_constraints.h \
	index_space/index_transform.h \
	index_space/index_transform_builder.h \
	spec_impl.h \
	static_cast.h \
	transaction.h \
	tensorstore_impl.h \
	context.h \
	transaction_impl.h \
	util/float8.h \
	util/maybe_hard_constraint.h \
	util/apply_members/apply_members.h \
	util/element_pointer.h \
	util/division.h \
	util/result.h \
	util/utf8_string.h \
	util/result_impl.h \
	util/executor.h \
	util/small_bit_set.h \
	util/future.h \
	util/execution/execution.h \
	util/execution/sender.h \
	util/execution/any_receiver.h \
	util/int2.h \
	util/future_impl.h \
	util/garbage_collection/fwd.h \
	util/garbage_collection/garbage_collection.h \
	util/str_cat.h \
	util/extents.h \
	util/iterate.h \
	util/status.h \
	util/constant_vector.h \
	util/element_traits.h \
	util/unit.h \
	util/byte_strided_pointer.h \
	util/int4.h \
	util/option.h \
	util/span.h \
	util/bfloat16.h \
	util/dimension_set.h \
	strided_layout.h \
	chunk_layout.h \
	json_serialization_options_base.h \
	kvstore/kvstore.h \
	kvstore/spec.h \
	box.h \
	context_impl_base.h \
	internal/unowned_to_shared.h \
	internal/path.h \
	internal/arena.h \
	internal/meta/integer_range.h \
	internal/meta/attributes.h \
	internal/meta/exception_macros.h \
	internal/meta/type_traits.h \
	internal/meta/integer_sequence.h \
	internal/meta/integer_types.h \
	internal/meta/void_wrapper.h \
	internal/meta/meta.h \
	internal/intrusive_ptr.h \
	internal/nditerable.h \
	internal/json_binding/bindable.h \
	internal/json_binding/std_optional.h \
	internal/elementwise_function.h \
	internal/context_binding.h \
	internal/mutex.h \
	internal/json/same.h \
	internal/preprocessor/expand.h \
	internal/preprocessor/cat.h \
	internal/unique_with_intrusive_allocator.h \
	internal/container/compressed_tuple.h \
	internal/container/multi_vector.h \
	internal/container/multi_vector_view.h \
	internal/container/multi_vector_impl.h \
	internal/container/intrusive_red_black_tree.h \
	internal/lock_collection.h \
	internal/source_location.h \
	internal/cache_key/cache_key.h \
	internal/cache_key/fwd.h \
	internal/integer_overflow.h \
	internal/compare.h \
	internal/gdb_scripting.h \
	internal/string_like.h \
	internal/memory.h \
	internal/tracing/trace_context.h \
	internal/poly/storage.h \
	internal/poly/poly_impl.h \
	internal/poly/poly.h \
	internal/tagged_ptr.h \
	data_type_conversion.h \
	schema.h \
	progress.h \
	tensorstore.h \
	read_write_options.h \
	codec_spec.h \
	batch.h \
	serialization/fwd.h \
	array_storage_statistics.h \
	open_mode.h \
	open.h \
	json_serialization_options.h \
	array.h \
	contiguous_layout.h \
	container_kind.h \
	staleness_bound.h \
	spec.h \
	index.h

TENSORSTORE_INC_TARGETS := $(addprefix include/tensorstore/,$(TENSORSTORE_INC))

all: install-abseil install-tensorstore install-ctds

clean:
	$(QUIET)cd tensorstore && bazel clean
	$(QUIET)cd abseil-cpp-$(HASH) && bazel clean
	$(QUIET)cd ctds && make clean

install-ctds: lib/libctds.so

install-tensorstore: lib/libtensorstore_complete.so

ctds/libctds.so: 
	$(QUIET)cd ctds && make

lib/libtensorstore_complete.so: tensorstore/bazel-bin/libtensorstore_complete.so $(TENSORSTORE_INC_TARGETS)
	$(QUIET)mkdir -p $(ROOT)/lib
	$(QUIET)cp -f -p $< $(ROOT)/lib

include/tensorstore/%: tensorstore/tensorstore/%
	@mkdir -p $(dir $@)
	cp $< $@

lib/libctds.so: ctds/libctds.so
	$(QUIET)mkdir -p $(ROOT)/lib
	$(QUIET)cp -f -p $< $(ROOT)/lib
	$(QUIET)mkdir -p $(ROOT)/include
	$(QUIET)cd ctds && find casacore -name '*.h' -o -name '*.tcc' | tar -chf - -T - | tar -C $(ROOT)/include -xf -


test-ctds:
	$(QUIET)cd ctds && make test

tensorstore/bazel-bin/libtensorstore_complete.so: tensorstore.patch tensorstore/.bazelrc.UPDATED
	$(QUIET)cd tensorstore && bazel build //:libtensorstore_complete.so

tensorstore/.bazelrc.UPDATED: tensorstore
	$(QUIET)if [ ! -f "tensorstore/.bazelrc.UPDATED" ]; then \
	    echo "build --action_env=CC=`which $(CC)`" >> tensorstore/.bazelrc; \
	    echo "build --action_env=CXX=`which $(CXX)`" >> tensorstore/.bazelrc; \
	fi
	@touch $@

tensorstore:
	$(QUIET)git clone https://github.com/google/tensorstore
	$(QUIET)cd tensorstore && git checkout v0.1.75
	$(QUIET)cd tensorstore && git apply ../tensorstore.patch

lib/absl/libbase.so: abseil-cpp-$(HASH)/bazel-bin/absl/base/libbase.so
	$(QUIET)mkdir -p $(ROOT)/include
	$(QUIET)cd abseil-cpp-$(HASH) && find absl -name '*.h' | tar -cf - -T - | tar -C $(ROOT)/include -xf -
	$(QUIET)mkdir -p $(ROOT)/lib/absl
	$(QUIET)cd abseil-cpp-$(HASH) && cp -f -p bazel-bin/absl/base/lib*.so $(ROOT)/lib/absl

install-abseil: lib/absl/libbase.so

abseil-cpp-$(HASH)/bazel-bin/absl/base/libbase.so: abseil-cpp-$(HASH)/.bazelrc.UPDATED
	$(QUIET)cd abseil-cpp-$(HASH) && bazel build //absl/...

abseil-cpp-$(HASH)/.bazelrc.UPDATED: abseil-cpp-$(HASH)
	@echo "build --action_env=CC=`which $(CC)`" >> abseil-cpp-$(HASH)/.bazelrc
	@echo "build --action_env=CXX=`which $(CXX)`" >> abseil-cpp-$(HASH)/.bazelrc
	@touch $@

abseil-cpp-$(HASH): $(HASH).tar.gz
	$(QUIET)tar -xzf $(HASH).tar.gz

$(HASH).tar.gz:
	$(QUIET)wget https://storage.googleapis.com/tensorstore-bazel-mirror/github.com/abseil/abseil-cpp/archive/$(HASH).tar.gz

%.o: %.cc lib/libctds.so  lib/libtensorstore_complete.so lib/absl/libbase.so
	$(CCACHE) $(CXX) $(CXXFLAGS) -c $< -o $@

tCtds2Zarr: tCtds2Zarr.o lib/libctds.so  lib/libtensorstore_complete.so lib/absl/libbase.so data/ea25_cal_small_before_fixed.split.ms
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

%: %.o ctds/libctds.so  tensorstore/bazel-bin/libtensorstore_complete.so lib/absl/libbase.so
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS) $(LIBS)

data/ea25_cal_small_before_fixed.split.ms: data/ea25_cal_small_before_fixed-split.tar.gz
	$(QUIET)cd data && tar zxf ea25_cal_small_before_fixed-split.tar.gz

data/ea25_cal_small_before_fixed-split.tar.gz:
	$(QUIET)mkdir -p data
	$(QUIET)curl -L https://casa.nrao.edu/download/devel/casavis/$@ -o $@

.PHONY: all test-ctds install-abseil install-tensorstore

