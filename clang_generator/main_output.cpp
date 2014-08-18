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
		"struct _ns\n"
		"{\n"
		"};\n"
		"using root_namespace = _ns;\n"
		"\n"
		;

	return insert(nullptr, scope_t{"_ns", "", "", false});
}

main_output::scope_t& main_output::emit_namespace(const clang::NamespaceDecl* nd)
{
	assert(nd);
	const clang::NamespaceDecl* orig = nd->getOriginalNamespace();
	assert(orig);

	scope_t& parent = get_or_create_parent_scope(orig);

	scope_t ns_scope;

	if (orig->isAnonymousNamespace() || parent.anonymous_)
	{
		static scope_t error_scope;
		error_scope.anonymous_ = true;
		error_scope.error_ = "Anonymous namespae";
	}
	else if ( orig->isInline())
	{
		// dig deeper
		return get_or_create_parent_scope(orig);
	}
	else
	{
		std::string buf;
		llvm::raw_string_ostream ss(buf);
		orig->printName(ss);
		std::string name = ss.str();

		ns_scope.anonymous_ = false;
		ns_scope.generated_type_name_ = parent.generated_type_name_ + "_" + name;
		ns_scope.qualified_name_ = parent.qualified_name_ + "::" + name;

		out_ <<
			"struct " << ns_scope.generated_type_name_ << "\n"
			"{\n"
			"	static const char* name = \"" << name << "\";\n"
			"	using parent_scope = " << parent.generated_type_name_ << ";\n"
			"}\n"
			"\n"
		;
	}

	const clang::DeclContext* as_decl = static_cast<const clang::DeclContext*>(orig);
	std::cerr << "orig=" << (const void*)orig << ", as decl=" << (const void*)as_decl << std::endl;
	return insert(as_decl, std::move(ns_scope));
}


void main_output::emit_enum(const clang::EnumDecl* decl)
{
	assert(decl);

	clang::IdentifierInfo* ii = decl->getIdentifier();
	if (ii)
	{
		scope_t& parent = get_or_create_parent_scope(decl);
		if (parent.anonymous_)
		{
			// TODO proper warning system
			std::cerr << "Not emiting enum " << ii->getName().str() << " : " << parent.error_ << std::endl;
			return;
		}


		std::string buf;
		llvm::raw_string_ostream ss(buf);
		ss << parent.qualified_name_ << "::" << ii->getName();
		std::string qualified_name = std::move(ss.str());

		// constant meta-types
		struct enum_const
		{
			std::string name;
			std::string generated_type_name;
			std::string value;
		};

		std::vector<enum_const> enum_constants; // generated name, fully-qualified name
		for (auto it = decl->enumerator_begin(); it != decl->enumerator_end(); ++it)
		{
			clang::EnumConstantDecl* constDecl = *it;

			ss << "_const_enum_" << ii->getName() << "_" << std::uintptr_t(constDecl);
			std::string generated_name = std::move(ss.str());

			constDecl->printName(ss);
			std::string name = std::move(ss.str());

			enum_constants.push_back({name, generated_name, constDecl->getInitVal().toString(10)});
		}

		for(auto& p : enum_constants)
		{
			out_ <<
				"struct " << p.generated_type_name << "\n"
				"{\n"
				"	static const char* name = \"" << p.name << "\";\n"
				"	static const int value = " << p.value << ";\n"
				"}\n"
				"\n"
			;
		}

		out_ <<
			"template<>\n"
			"struct meta_type<" << qualified_name << ">\n"
			"{\n"
			"	static const char* name = \"" << ii->getName() << "\";\n"
			"	using type = " << qualified_name << ";\n"
			"	using parent_scope = " << parent.generated_type_name_ << ";\n"
			;

		// TODO enumerate and dispatch here


		out_ <<
			"};\n"
			"\n"
		;
	}

}

main_output::scope_t& main_output::get_or_create_parent_scope(const clang::NamedDecl* decl)
{
	assert(decl);
	const clang::DeclContext* ctx = decl->getDeclContext();
	return get_or_create_scope(ctx);
}

main_output::scope_t& main_output::get_or_create_scope(const clang::DeclContext* ctx)
{
	auto it = scopes_.find(ctx);
	if (it == scopes_.end())
	{
		return create_scope(ctx);
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
	// record
	if (const clang::RecordDecl* rd = llvm::dyn_cast<clang::RecordDecl>(ctx))
	{
		static scope_t error_scope;

		error_scope.error_ = std::string("Unsuported scope type: ") + ctx->getDeclKindName();
		error_scope.anonymous_ = true;
		return error_scope;
	}
	// others - keep on digging
	else
	{
		return get_or_create_scope(ctx->getParent());
	}

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
