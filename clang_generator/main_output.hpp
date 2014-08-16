#pragma once

#include <clang/AST/ASTContext.h>

#include <llvm/Support/raw_ostream.h>

#include <unordered_set>

namespace reflection_generator
{

class output
{
public:
	output(llvm::raw_ostream& out) : out_(out) {}
protected:
	llvm::raw_ostream& out_;
};

struct main_output : public output
{
	main_output(llvm::raw_ostream& out);

	~main_output();

	void emit_enum(clang::EnumDecl* decl);
	void emit_namespace(clang::NamespaceDecl* decl);

private:

	std::unordered_set<clang::NamedDecl*> emitted_namespaces_;
};


}
