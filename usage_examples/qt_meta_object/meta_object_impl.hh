#pragma once

#include <QGenericReturnArgument>
#include <QGenericArgument>

#include "meta_object.hh"

namespace QtLike { namespace Detail {

template<typename T>
class MetaObjectMaker
{
public:

	MetaObject make();

	template<typename MethodType>
	void method();

private:

	QList<MetaMethod> methods_;

};

template<typename T>
MetaObject makeMetaObject()
{
	MetaObjectMaker<T> maker;
	return maker.make();
};

template<typename T>
MetaObject MetaObjectMaker<T>::make()
{
	using meta = reflection::meta_object<T>;

	meta::enumerate_methods(*this);

	return MetaObject(meta::name, std::move(methods_));
}

template<typename T>
template<typename MethodType>
void MetaObjectMaker<T>::method()
{
	methods_.push_back(MetaMethod(MethodType::name));
}


}}

