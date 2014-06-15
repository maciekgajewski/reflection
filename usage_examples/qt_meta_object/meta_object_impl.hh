#pragma once

#include <QGenericReturnArgument>
#include <QGenericArgument>

#include "meta_object.hh"

#include <tuple>
#include <type_traits>

namespace QtLike { namespace Detail {


struct arg_iterator
{
	QGenericArgument* args[3];
	unsigned index = 0;

	template <typename T>
	T& get()
	{
		using naked_type = typename std::remove_reference<T>::type;
		naked_type* ptr = reinterpret_cast<naked_type*>(args[index++]->data());

		return *ptr;
	}
};


template<typename MethodType>
class MetaMethodImpl : public BaseMetaMethodImpl
{
public:
	virtual void invoke(
		void* object,
		QGenericReturnArgument returnValue,
		QGenericArgument arg0,
		QGenericArgument arg1,
		QGenericArgument arg2
		) const override
	{
		using compatible_function = std::function<typename MethodType::signature>;

		unpack(
			object,
			returnValue,
			arg0, arg1, arg2,
			static_cast<compatible_function*>(nullptr)
			);
	}

private:

	template<typename Ret, typename... Args>
	void unpack(
		void* object,
		QGenericReturnArgument returnValue,
		QGenericArgument arg0,
		QGenericArgument arg1,
		QGenericArgument arg2,
		std::function<Ret(Args...)>* dummy) const
	{
		// TODO ignore return value for now
		using tuple_type = std::tuple<Args...>;

		// repack generic arguments as a tuple

		arg_iterator iter;
		iter.args[0] = &arg0;
		iter.args[1] = &arg1;
		iter.args[2] = &arg2;

		tuple_type params { iter.get<Args>()... };

		//MethodType::unpack(*(reinterpret_cast<typename MethodType::class_type*>(object)), params);
	}


};

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
			std::move(paramNames_),
			new MetaMethodImpl<MethodType>()
		)
	);
}


}}

