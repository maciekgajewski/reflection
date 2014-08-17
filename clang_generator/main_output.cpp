#include "main_output.hpp"

#include <clang/AST/DeclTemplate.h>

#include <sstream>
#include <cstdint>

#include <iostream>

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
		;
}

main_output::~main_output()
{
	out_ <<
		"}\n"
		;
}

main_output::scope_t& main_output::emit_root_namespace()
{
	out_ <<
		"struct _ns_root\n"
		"{\n"
		"};\n"
		"using root_namespace = _ns_root;\n"
		"\n"
		;

	return insert(nullptr, scope_t{"_ns_root", "", "", false});
}

main_output::scope_t& main_output::emit_namespace(const clang::NamespaceDecl* nd)
{
	assert(nd);
	const clang::NamespaceDecl* orig = nd->getOriginalNamespace();
	assert(orig);

	scope_t& parent = get_or_create_parent_scope(nd->getParent());

	scope_t ns_scope;

	if (orig->isAnonymousNamespace() || orig->isInline() || parent.anonymous_)
	{
		ns_scope.anonymous_ = true;
		ns_scope.generated_type_name_ = "_ns_anon" + std::to_string(std::uintptr_t(orig));
	}
	else
	{
		std::string name;
		llvm::raw_string_ostream ss(name);
		orig->printName(ss);

		ns_scope.anonymous_ = false;
		ns_scope.generated_type_name_ = parent.generated_type_name_ + "_" + name;
		ns_scope.qualified_name_ = parent.qualified_name_ + "::" + name;
	}

	return insert(static_cast<const clang::DeclContext*>(orig), std::move(ns_scope));
}


void main_output::emit_enum(const clang::EnumDecl* decl)
{
	assert(decl);

	clang::IdentifierInfo* ii = decl->getIdentifier();
	if (ii)
	{
		scope_t& parent = get_or_create_parent_scope(decl->getDeclContext());
		if (parent.anonymous_)
		{
			// TODO proper waring system
			std::cerr << "Not emiting enum " << ii->getName().str() << " : " << parent.error_ << std::endl;
			return;
		}

		std::string generated_name;
		if (parent.anonymous_)
		{
			return;
		}

		std::string buf;
		llvm::raw_string_ostream ss(buf);
		ss << parent.qualified_name_ << "::" << ii->getName();
		std::string qualified_name = ss.str();

		out_ <<
			"template<>\n"
			"struct meta_type<" << qualified_name << ">\n"
			"{\n"
			"	static const char* constexpt name = \"" << ii->getName() << "\";\n"
			"	using type = " << qualified_name << ";\n"
			;

		// TODO enumerate and dispatch here

		out_ <<
			"};\n"
			"\n"
		;
	}

}

main_output::scope_t& main_output::get_or_create_parent_scope(const clang::DeclContext* ctx)
{
	assert(ctx);

	const clang::DeclContext* parentCtx = ctx->getParent();

	auto it = scopes_.find(parentCtx);
	if (it == scopes_.end())
	{
		return create_scope(parentCtx);
	}
	else
	{
		return it->second;
	}
}

main_output::scope_t& main_output::create_scope(const clang::DeclContext* ctx)
{
	assert(scopes_.find(ctx) == scopes_.end() && "scope already exists");

	// root ns
	if (ctx == nullptr)
	{
		return emit_root_namespace();
	}
	// other ns
	if (const clang::NamespaceDecl* nd = llvm::dyn_cast<clang::NamespaceDecl>(ctx))
	{
		std::cerr << "ctx=" << (const void*)ctx << ", nd=" << (const void*)nd << std::endl;
		return emit_namespace(nd);
	}

	static scope_t error_scope;

	error_scope.error_ = std::string("Unsuported scope type: ") + ctx->getDeclKindName();
	error_scope.anonymous_ = true;
	return error_scope;
}

std::string generate_name(clang::NamedDecl* decl)
{
	static std::string buf;
	llvm::raw_string_ostream ss(buf);

	const clang::DeclContext* ctx = decl->getDeclContext();

	ss << "__" << ctx->getDeclKindName();

	llvm::SmallVector<const clang::DeclContext*, 8> ctxs;
	while (ctx && llvm::isa<clang::NamedDecl>(ctx)) {
		ctxs.push_back(ctx);
		ctx = ctx->getParent();
	}


	for (auto it = ctxs.rbegin(); it != ctxs.rend(); ++it)
	{
		ss << "_";
		// class pecialization
		if (const clang::ClassTemplateSpecializationDecl* spec = llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(*it))
		{
			ss << spec->getName();
			const clang::TemplateArgumentList& templateArgs = spec->getTemplateArgs();

			for(unsigned i = 0; i < templateArgs.size(); ++i)
			{
				const clang::TemplateArgument& arg = templateArgs[i];
				ss << "_";
				arg.print(clang::PrintingPolicy(clang::LangOptions()), ss);
			}
		}
		// namespace
		else if (const clang::NamespaceDecl* nd = llvm::dyn_cast<clang::NamespaceDecl>(*it))
		{
			if (nd->isAnonymousNamespace() || nd->isInline())
			{
				continue; // TODO what now?
			}
			else
			{
				nd->printName(ss);
			}
		}
		// record
		else if (const clang::RecordDecl* rd = llvm::dyn_cast<clang::RecordDecl>(*it))
		{
			if (!rd->getIdentifier())
			{
				// TODO anonymous class, what now?
			}
			else
			{
				rd->printName(ss);
			}
		}
		else
		{
			assert(false && "Unsupported scope type");
		}
	}

	return ss.str();
}


}
