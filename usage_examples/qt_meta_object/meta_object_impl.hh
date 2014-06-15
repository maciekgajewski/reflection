#pragma once

#include <QGenericReturnArgument>
#include <QGenericArgument>

#include "meta_object.hh"

#include <array>

namespace QtLike { namespace Detail {

// seq builder
template<int ...N>
struct seq
{ };

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...>
{ };

template<int ...S>
struct gens<0, S...>
{
	using type = seq<S...>;
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

	using argument_array = std::array<QGenericArgument*, 3>;

	template<typename Ret, typename... Args>
	void unpack(
		void* object,
		QGenericReturnArgument returnValue,
		QGenericArgument arg0,
		QGenericArgument arg1,
		QGenericArgument arg2,
		std::function<Ret(Args...)>* dummy) const
	{
		argument_array args;
		using seq_type = typename gens<sizeof...(Args)>::type;

		args[0] = &arg0;
		args[1] = &arg1;
		args[2] = &arg2;

		unpack2<Ret, Args...>(object, returnValue, args, seq_type());
	}

	template<typename T, int I>
	static T& get_arg(argument_array& args)
	{
		using naked_T = typename std::remove_reference<T>::type;
		QGenericArgument* ga = std::get<I>(args);
		naked_T* ptr = reinterpret_cast<naked_T*>(ga->data());
		return *ptr;
	}

	template<typename Ret, typename... Args, int... Idx>
	void unpack2(
		void* object,
		QGenericReturnArgument returnValue,
		std::array<QGenericArgument*, 3>& args,
		seq<Idx...> s
		) const
	{
		using class_type = typename MethodType::class_type;
		class_type* instance_ptr = reinterpret_cast<class_type*>(object);
		MethodType::call(*instance_ptr, get_arg<Args, Idx>(args)...);
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

