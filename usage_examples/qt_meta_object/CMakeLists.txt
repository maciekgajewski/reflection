find_package(Qt5Core REQUIRED)

add_executable(qt_meta_object

	main.cc
	meta_object.hh
	meta_object_impl.hh
)


target_link_libraries(qt_meta_object
	data_types
)

qt5_use_modules(qt_meta_object
	Core)
