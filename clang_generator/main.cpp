#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>

#include <llvm/Support/CommandLine.h>

#include <iostream>


class MyAstVisitor : public clang::RecursiveASTVisitor<MyAstVisitor>
{
public:

	virtual ~MyAstVisitor() {}

//	bool virtual VisitNamespaceDecl(clang::NamespaceDecl* nsDecl)
//	{
//		clang::IdentifierInfo* ii = nsDecl->getIdentifier();
//		std::cout << "ns: " ;
//		if (ii)
//		{
//			llvm::StringRef s = ii->getName();
//			std::cout << s.str();
//		}
//		std::cout << std::endl;

//		return true;
//	}

	virtual bool VisitEnumDecl(clang::EnumDecl* decl)
	{
		clang::IdentifierInfo* ii = decl->getIdentifier();
		if (ii)
		{
			std::string fqn = decl->getQualifiedNameAsString();
			std::cout << "enum: " << ii->getName().str() << " (" << fqn << ")" << std::endl;

			if (decl->hasAttrs())
			{
				clang::AttrVec attrs = decl->getAttrs();
				for(clang::Attr* attr : attrs)
				{
					std::cout << "* attr: " << attr->getSpelling() << std::endl;
				}
			}
		}
		return true;
	}
};

class MyAstConsumer : public clang::ASTConsumer
{
public:

	virtual void HandleTranslationUnit(clang::ASTContext& ctx) override
	{
		MyAstVisitor visitor;
		visitor.TraverseDecl(ctx.getTranslationUnitDecl());
	}

};

static llvm::cl::opt<std::string> OptionOut("o", llvm::cl::desc("Specify output filename"), llvm::cl::value_desc("filename"));

class MyAction : public clang::ASTFrontendAction
{
protected:
	virtual clang::ASTConsumer *CreateASTConsumer(
		clang::CompilerInstance& compiler, llvm::StringRef inFile) override
	{
		llvm::raw_fd_ostream* out = compiler.createOutputFile(OptionOut.getValue(), false, false, "", "", true);
		*out << "Hello!\n";
		return new MyAstConsumer();
	}
};


int main(int argc, const char** argv)
{
	llvm::cl::OptionCategory toolCategory("reflection_generator options");

	clang::tooling::CommonOptionsParser optionsParser(argc, argv, toolCategory);

	auto sources = optionsParser.getSourcePathList();


	clang::tooling::ClangTool tool(optionsParser.getCompilations(), sources);

	int result = tool.run(clang::tooling::newFrontendActionFactory<MyAction>().get());

	return result;
}
