#include <iostream>

#include "data_types/reflection.hh"

#include "meta_object.hh"
#include "meta_object_impl.hh"


void printObjectData(QtLike::MetaObject* metaObject)
{
	std::cout << "type: " << metaObject->className() << std::endl;
}

int main()
{
	std::cout << "Hello!" << std::endl;

	QtLike::MetaObject* metaObject = QtLike::getMetaObject<ProjectNamespace::SimpleClass>();

	printObjectData(metaObject);
}
