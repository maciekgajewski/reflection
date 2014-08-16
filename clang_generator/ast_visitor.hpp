#pragma once

#include <clang/AST/RecursiveASTVisitor.h>

#include "main_output.hpp"

namespace reflection_generator
{

class ast_visitor : public clang::RecursiveASTVisitor<ast_visitor>
{
public:

	ast_visitor(llvm::raw_ostream& out): main_out_(out)
	{
	}

	virtual ~ast_visitor()
	{
	}

	virtual bool VisitEnumDecl(clang::EnumDecl* decl)
	{
		main_out_.emit_enum(decl);
		return true;
	}

	virtual bool VisitNamespaceDecl(clang::NamespaceDecl* decl)
	{
		main_out_.emit_namespace(decl);
		return true;
	}

private:

	main_output main_out_;
};

}
