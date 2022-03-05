


current_dir := ${CURDIR}/third_party

configure_readline_args := --disable-shared --enable-install-examples --host=${HOST_NAME} --prefix=${CURDIR}/target

lib_name_readline := readline-8.0
lib_name_suffix :=.tar.gz

sub_target:	
	$(call auto_make_install_compressed_lib_once,${current_dir},${lib_name_readline},$(lib_name_suffix),$(configure_readline_args))



