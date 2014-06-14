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

	template<typename ParamType>
	void param(const char* name)
	{
		paramNames_.push_back(name);
		paramTypes_.push_back(typeid(ParamType).name()); // too lazy to play with it
	}

private:

	QList<MetaMethod> methods_;
	QList<QByteArray> paramNames_;
	QList<QByteArray> paramTypes_;

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
	MethodType::enumerate_params(*this);

	methods_.push_back(
		MetaMethod(
			MethodType::name,
			std::move(paramTypes_),
			std::move(paramNames_)
		)
	);
}


}}

