

current_dir := ${CURDIR}/third_party

configure_readline_args := --disable-shared --enable-install-examples --host=${HOST_NAME} --prefix=${CURDIR}/target
configure_ncurses_args := CFLAGS="" --host=${HOST_NAME} --prefix=${CURDIR}/target 
configure_ncurses_args += --without-cxx --without-cxx-binding  --without-manpages --disable-tic-depends \
--without-ticlib --disable-stripping --disable-leaks --disable-macros --disable-gnat-projects \
--disable-db-install

lib_name_readline := readline-8.0
lib_name_ncurses := ncurses-6.2

lib_name_suffix :=.tar.gz

sub_target:	
	$(call auto_make_install_compressed_lib_once,${current_dir},${lib_name_ncurses},$(lib_name_suffix),$(configure_ncurses_args))
	$(call auto_make_install_compressed_lib_once,${current_dir},${lib_name_readline},$(lib_name_suffix),$(configure_readline_args))



