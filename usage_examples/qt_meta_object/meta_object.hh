#pragma once

#include <QVariant>
#include <QList>
#include <QByteArray>
#include <QGenericReturnArgument>
#include <QGenericArgument>

namespace QtLike
{

// Qt-like meta object system.
// Stores class meta-data in runtine structure

class MetaObject;

namespace Detail {

class MetaMethodImpl
{
public:

	virtual void invoke(
		void* object, // void* instead of QObject*
		QGenericReturnArgument returnValue,
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

	bool isValid() const { return !!impl_; }

	void invoke(
		void* object, // void* instead of QObject*
		QGenericReturnArgument returnValue,
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

	MetaMethod(QByteArray&& name)
		: name_(name)
	{
	}

	QByteArray name_;
	QList<QByteArray> paramNames_;
	QList<QByteArray> paramTypes_;

	Detail::MetaMethodImpl* impl_ = nullptr;
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
