package=native_openssl
$(package)_version=1.1.1f
$(package)_download_path=https://www.openssl.org/source/old/1.1.1
$(package)_file_name=openssl-$($(package)_version).tar.gz
$(package)_sha256_hash=186c6bfe6ecfba7a5b48c47f8a1673d0f3b0e5ba2e25602dd23b629975da3f35
define $(package)_set_vars
$(package)_build_config_opts= --prefix=$(build_prefix) no-zlib no-shared no-krb5C linux-generic32 -m32
endef

define $(package)_config_cmds
  ./Configure $($(package)_build_config_opts) &&\
  sed -i "s|engines apps test|engines|" Makefile
endef

define $(package)_build_cmds
  $(MAKE) -j1
endef

define $(package)_stage_cmds
  $(MAKE) INSTALL_PREFIX=$($(package)_staging_dir) -j1 install_sw
endef
