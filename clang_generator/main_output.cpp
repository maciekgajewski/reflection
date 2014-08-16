#include "main_output.hpp"

namespace reflection_generator
{

main_output::main_output(llvm::raw_ostream& out) : output(out)
{
	out_ <<
		"#pragma once\n"
		"\n"
		"namespace reflection\n"
		"{\n"
		"\n"
		"template<typename T>\n"
		"struct meta_type;\n"
		"\n"
		"struct root_namespace\n"
		"{\n"
		"}\n"
		"\n"
		;
}

main_output::~main_output()
{
	out_ <<
		"}\n"
		;
}

void main_output::emit_enum(clang::EnumDecl* decl)
{
	clang::IdentifierInfo* ii = decl->getIdentifier();
	if (ii)
	{
		out_ <<
			"template<>\n"
			"struct meta_type<::";
			decl->printQualifiedName (out_);
		out_ <<
			">\n"
			"{\n"
			"	static const char* constexpt name = \"" << ii->getName() << "\";\n"
			"};\n"
			"\n"
		;
	}

}

void main_output::emit_namespace(clang::NamespaceDecl* decl)
{
	clang::NamespaceDecl* orig = decl->getOriginalNamespace();
	if (emitted_namespaces_.find(orig) == emitted_namespaces_.end())
	{
		emitted_namespaces_.insert(orig);

		out_ << "generated type for namespace: ";
		orig->printQualifiedName (out_);
		out_ << "\n";
	}
}


}
