PROJ_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Configuration of extension
EXT_NAME=waddle
EXT_CONFIG=${PROJ_DIR}extension_config.cmake

# Include the Makefile from extension-ci-tools
include extension-ci-tools/makefiles/duckdb_extension.Makefile

# Keep the clangd compile-commands tree in sync with every `make` invocation,
# without disabling unity builds in the real build/release tree.
# this is done so my nvim lsp works
EXT_DEBUG_FLAGS := -DDISABLE_UNITY=1
all: clangd duckdb-clangd

# nvim-lspconfig roots a second clangd instance at duckdb/ (it has its own
# .clangd + .git), so its compile database needs to be generated too.
duckdb-clangd:
	$(MAKE) -C duckdb clangd
