#pragma once

#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QGenericReturnArgument>
#include <QGenericArgument>

#include <memory>

namespace QtLike
{

// Qt-like meta object system.
// Stores class meta-data in runtine structure

class MetaObject;

namespace Detail {

class BaseMetaMethodImpl
{
public:

	virtual void invoke(
		void* object, // void* instead of QObject*
		QGenericReturnArgument returnValue = QGenericReturnArgument(),
		QGenericArgument arg0 = QGenericArgument(),
		QGenericArgument arg1 = QGenericArgument(),
		QGenericArgument arg2 = QGenericArgument()
		) const = 0;
};

template<typename T>
class MetaObjectMaker;

template<typename T>
MetaObject makeMetaObject();

}

class MetaMethod
{
public:

	MetaMethod() = default;

	bool isValid() const { return !!impl_; }

	void invoke(
		void* object, // void* instead of QObject*
		QGenericReturnArgument returnValue = QGenericReturnArgument(),
		QGenericArgument arg0 = QGenericArgument(),
		QGenericArgument arg1 = QGenericArgument(),
		QGenericArgument arg2 = QGenericArgument()
		) const
	{
		impl_->invoke(object, returnValue, arg0, arg1, arg2);
	}

	QByteArray name() const { return name_; }
	int parameterCount() const{ return paramNames_.size(); }
	QList<QByteArray> parameterNames() const { return paramTypes_; }
	QList<QByteArray> parameterTypes() const { return paramNames_; }

private:

	template <typename T> friend class Detail::MetaObjectMaker;

	MetaMethod(
		QByteArray&& name,
		QList<QByteArray>&& paramNames,
		QList<QByteArray>&& paramTypes,
		Detail::BaseMetaMethodImpl* impl
		)
		: name_(name)
		, paramNames_(std::move(paramNames))
		, paramTypes_(std::move(paramTypes))
		, impl_(impl)
	{
	}

	QByteArray name_;
	QList<QByteArray> paramNames_;
	QList<QByteArray> paramTypes_;

	Detail::BaseMetaMethodImpl* impl_ = nullptr;
};

class MetaProperty
{
};


class MetaObject
{
public:

	const char* className() const { return className_; }

	int methodCount() const { return methods_.size(); }
	const MetaMethod& method(int index) const { return methods_[index]; }

private:

	template <typename T> friend class Detail::MetaObjectMaker;

	MetaObject(const char* className, QList<MetaMethod>&& methods)
		: className_(className)
		, methods_(std::move(methods))
	{
	}

	const char* className_;

	QList<MetaMethod> methods_;
};

template<typename T>
MetaObject* getMetaObject()
{
	static MetaObject metaObject(Detail::makeMetaObject<T>());
	return &metaObject;
}

}
