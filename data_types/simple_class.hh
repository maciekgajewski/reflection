#pragma once

#include <memory>

#include <string>

namespace ProjectNamespace
{

namespace Detail
{
	struct SomeDetails
	{
		std::shared_ptr<std::string> mValue;
	};
}

class SimpleClass
{
public:

	SimpleClass();

	void AddToCounter(unsigned value);
	void SetName(const std::string& name, int number);

	unsigned GetCounter() const { return mCounter; }
	std::string GetName() const { return mName; }

private:

	unsigned mCounter;
	std::string mName;
	Detail::SomeDetails mGoryDetails;
};

}

