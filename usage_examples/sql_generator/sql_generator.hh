#pragma once

#include <ostream>
#include <cstdint>
#include <type_traits>
// Tools here use reflection to generate SQL queries for structures

namespace sql {

namespace detail {

template<typename T, typename Enabler = void>
struct sql_name_helper;

template<>
struct sql_name_helper<std::int32_t> { static const char* name() { return "INT4"; } };
template<>
struct sql_name_helper<std::int64_t> { static const char* name() { return "INT8"; } };

template<>
struct sql_name_helper<double> { static const char* name() { return "FLOAT8"; } };

template<>
struct sql_name_helper<std::string> { static const char* name() { return "TEXT"; } };

template<typename E>
struct sql_name_helper<E, typename std::enable_if<std::is_enum<E>::value>::type>
{
	static const char* name() { return ::reflection::meta_enum<E>::name; }
};

template<typename T>
const char* sql_type_name()
{
	return sql_name_helper<T>::name();
}

/*
template<>
const char* sql_type_name<std::int32_t>() { return "int4"; }

template<>
const char* sql_type_name<std::int64_t>() { return "int8"; }

template<>
const char* sql_type_name<double>() { return "float8"; }

template<>
const char* sql_type_name<std::string>() { return "text"; }

template<typename E>
std::enable_if<std::is_enum<E>::value, const char*> sql_type_name<E>() { return "enum"; }
*/

struct table_def_field_receiver
{
	table_def_field_receiver(std::ostream& out) : out_(out) {}
	std::ostream& out_;
	bool first_ = true;

	template<typename MetaField>
	void field()
	{
		if (!first_)
			out_ << ",\n";
		else
			first_ = false;

		out_ << "\t" << MetaField::name << " " << sql_type_name<typename MetaField::type>();
	}
};

struct enum_decl_receiver
{
	enum_decl_receiver(std::ostream& out) : out_(out) {}
	std::ostream& out_;
	bool first_ = true;

	template<typename E>
	void value(const char* name, E)
	{
		if (!first_)
			out_ << ", ";
		else
			first_ = false;
		out_ << "'" << name << "'";
	}
};

struct column_name_receiver
{
	column_name_receiver(std::ostream& out) : out_(out) {}
	std::ostream& out_;
	bool first_ = true;

	template<typename MetaField>
	void field()
	{
		if (!first_)
			out_ << ", ";
		else
			first_ = false;

		out_ << MetaField::name;
	}
};

template<typename T>
void print_sql_value(std::ostream& out, const T& v)
{
	out << v;
}

template<>
void print_sql_value<std::string>(std::ostream& out, const std::string& v)
{
	out << "'" << v << "'";
}

template<typename T>
struct value_receiver
{
	value_receiver(std::ostream& out, const T& instance)
		: out_(out), instance_(instance) {}
	std::ostream& out_;
	const T& instance_;
	bool first_ = true;

	template<typename MetaField>
	void field()
	{
		using type = typename MetaField::type;
		if (!first_)
			out_ << ", ";
		else
			first_ = false;

		print_sql_value<type>(out_, MetaField::get(instance_));
	}

};

// generates list of comma-separated column names
template<typename T>
void column_names(std::ostream& out)
{
	::reflection::meta_object<T>::enumerate_fields(column_name_receiver(out));
}

// generates list of comma-separated record values
template<typename T>
void values(std::ostream& out, const T& record)
{
	::reflection::meta_object<T>::enumerate_fields(value_receiver<T>(out, record));
}



} // detail

// Creates valid, Postgres definiton of table to hold values of specific type
template<typename T>
void generate_table_definition(std::ostream& out, const std::string& table_name)
{
	using meta = ::reflection::meta_object<T>;

	out << "CREATE TABLE " << table_name << " (\n";
	meta::enumerate_fields(detail::table_def_field_receiver(out));
	out << "\n);" << std::endl;
}

// Creates enum type declaration
template<typename E>
void generate_enum_definion(std::ostream& out)
{
	using meta = ::reflection::meta_enum<E>;
	out << "CREATE TYPE " << meta::name << " AS ENUM (";
	meta::enumerate_values(detail::enum_decl_receiver(out));
	out << ");" << std::endl;
}

// Creates insert statemen for object range
template<typename Range>
void generate_insert(std::ostream& out, const std::string& table_name, const Range& range)
{
	using t = decltype(*std::begin(range));
	using type = typename std::remove_cv<typename std::remove_reference<t>::type>::type;
	//using meta = ::reflection::meta_object<type>;

	auto first = std::begin(range);
	const auto last = std::end(range);

	if (first != last)
	{
		out << "INSERT INTO " << table_name << " (";
		detail::column_names<type>(out);
		out << ") VALUES ";

		bool f = true;
		for(;first != last; ++first)
		{
			if (!f) out << ", ";
			else f = false;

			out << "(";
			detail::values<type>(out, *first);
			out << ")";
		}

		out << ";" << std::endl;
	}
}

} // ns

