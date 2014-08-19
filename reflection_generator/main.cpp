#include "main_output.hpp"
#include "ast_visitor.hpp"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>

#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>

#include <llvm/Support/CommandLine.h>

#include <iostream>


namespace reflection_generator
{


class ast_consumer : public clang::ASTConsumer
{
public:

	ast_consumer(llvm::raw_ostream& out): out_(out)
	{
	}

	virtual void HandleTranslationUnit(clang::ASTContext& ctx) override
	{
		ast_visitor visitor(out_);
		visitor.TraverseDecl(ctx.getTranslationUnitDecl());
	}

private:

	llvm::raw_ostream& out_;

};

static llvm::cl::opt<std::string> OptionOut("o", llvm::cl::desc("Specify output filename"), llvm::cl::value_desc("filename"));

class action : public clang::ASTFrontendAction
{
protected:
	virtual clang::ASTConsumer *CreateASTConsumer(
		clang::CompilerInstance& compiler, llvm::StringRef inFile) override
	{
		llvm::raw_fd_ostream* out = compiler.createOutputFile(OptionOut.getValue(), false, false, "", "", true);
		return new ast_consumer(*out);
	}
};

} // ns

int main(int argc, const char** argv)
{
	llvm::cl::OptionCategory toolCategory("reflection_generator options");

	clang::tooling::CommonOptionsParser optionsParser(argc, argv, toolCategory);

	auto sources = optionsParser.getSourcePathList();


	clang::tooling::ClangTool tool(optionsParser.getCompilations(), sources);

	int result = tool.run(clang::tooling::newFrontendActionFactory<reflection_generator::action>().get());

	return result;
}
