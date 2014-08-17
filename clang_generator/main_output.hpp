#pragma once

#include <clang/AST/ASTContext.h>

#include <llvm/Support/raw_ostream.h>

#include <unordered_map>

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

	void emit_enum(const clang::EnumDecl* decl);

private:

	struct scope_t
	{
		std::string generated_type_name_;
		std::string qualified_name_;
		std::string error_;
		bool anonymous_;
	};

	main_output::scope_t& emit_root_namespace();
	main_output::scope_t& emit_namespace(const clang::NamespaceDecl* decl);

	// gets (creates if neccesary) parent scope for declaration
	scope_t& get_or_create_parent_scope(const clang::DeclContext* ctx);
	scope_t& create_scope(const clang::DeclContext* ctx);



	scope_t& insert(const clang::DeclContext* ctx, scope_t&& scope)
	{
		auto res = scopes_.insert(std::make_pair(ctx, std::move(scope)));
		assert(res.second);
		return res.first->second;
	}

	std::unordered_map<const clang::DeclContext*, scope_t> scopes_;
};


}
