//#define __STDC_CONSTANT_MACROS
//#define __STDC_LIMIT_MACROS

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>

#include <clang/AST/ASTConsumer.h>

#include <clang/Parse/ParseAST.h>

#include <clang/Basic/Version.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/Diagnostic.h>

#include <iostream>


class MyAstConsumer : public clang::ASTConsumer
{
public:

	virtual void HandleTranslationUnit(clang::ASTContext& ctx) override
	{
		std::cout << "Boom!" << std::endl;
	}

};


int main(int argc, char** argv)
{
	llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOpts = new clang::DiagnosticOptions();

	clang::TextDiagnosticPrinter* diagClient = new clang::TextDiagnosticPrinter(llvm::errs(), &*diagOpts);


	llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagID(new clang::DiagnosticIDs());

	clang::DiagnosticsEngine diags(diagID, &*diagOpts, diagClient);

	clang::CompilerInstance ci;
	ci.setDiagnostics(&diags);


	clang::CompilerInvocation in;
	clang::CompilerInvocation::CreateFromArgs(in, argv+1, argv + argc, ci.getDiagnostics());

	if (in.getFrontendOpts().ShowHelp)
	{
		std::cout << "This is help" << std::endl;
	}
	else if (in.getFrontendOpts().Inputs.empty())
	{
		std::cout << "No inputs" << std::endl;
	}
	else
	{
		for(const auto& input : in.getFrontendOpts().Inputs)
		{
			std::string f = input.getFile();
			std::cout << "Input: " << f << std::endl;

			ci.createFileManager();
			ci.createSourceManager(ci.getFileManager());
			ci.createPreprocessor(clang::TU_Complete);
			ci.createASTContext();

			MyAstConsumer astConsumer;

			ci.setASTConsumer(&astConsumer);

			ci.createSema(clang::TU_Complete, nullptr);
			ci.InitializeSourceManager(input);

			clang::ParseAST(ci.getSema(), true);

		}
	}

}
