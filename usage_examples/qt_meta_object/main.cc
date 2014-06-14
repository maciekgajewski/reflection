#include <iostream>

#include "data_types/reflection.hh"

#include "meta_object.hh"
#include "meta_object_impl.hh"


void printObjectData(QtLike::MetaObject* metaObject)
{
	std::cout << "== Type signature ==" << std::endl;
	std::cout << "Type: " << metaObject->className() << std::endl;
	std::cout << "Methods: " << std::endl;

	for (int i = 0; i < metaObject->methodCount(); ++i)
	{
		const QtLike::MetaMethod& method = metaObject->method(i);

		std::cout << " * " << method.name().data() << "(";

		const QList<QByteArray> paramNames = method.parameterNames();
		const QList<QByteArray> paramTypes = method.parameterTypes();
		const char* sep = "";

		for(int j =0; j < paramNames.size(); ++j)
		{
			std::cout << sep << paramTypes[j].data() << " " << paramNames[j].data();
			sep = ", ";
		}
		std::cout << ")" << std::endl;
	}
}

int main()
{
	std::cout << "Hello!" << std::endl;

	QtLike::MetaObject* metaObject = QtLike::getMetaObject<ProjectNamespace::SimpleClass>();

	printObjectData(metaObject);
}
