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

} // detail

// Creates valid, Postgres definiton of table to hold values of specific type
template<typename T>
void generate_table_definition(std::ostream& out, const std::string& table_name)
{
	out << "CREATE TABLE " << table_name << " (\n";

	::reflection::meta_object<T>::enumerate_fields(detail::table_def_field_receiver(out));

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

} // ns

