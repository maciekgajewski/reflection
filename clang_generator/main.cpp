//#define __STDC_CONSTANT_MACROS
//#define __STDC_LIMIT_MACROS

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
//#include <clang/Frontend/CompilerInvocation.h>
//#include <clang/Frontend/TextDiagnosticPrinter.h>

#include <clang/AST/ASTConsumer.h>

//#include <clang/Parse/ParseAST.h>

//#include <clang/Basic/Version.h>
//#include <clang/Basic/TargetInfo.h>
//#include <clang/Basic/Diagnostic.h>

#include <clang/Tooling/Tooling.h>
#include <clang/Tooling/CommonOptionsParser.h>

#include <iostream>


class MyAstConsumer : public clang::ASTConsumer
{
public:

	virtual void HandleTranslationUnit(clang::ASTContext& ctx) override
	{
		std::cout << "Boom!" << std::endl;
	}

};

class MyAction : public clang::ASTFrontendAction
{
protected:
	virtual clang::ASTConsumer *CreateASTConsumer(
		clang::CompilerInstance& compiler, llvm::StringRef inFile) override
	{
		return new MyAstConsumer();
	}
};


int main(int argc, const char** argv)
{
	llvm::cl::OptionCategory toolCategory("my-tool options");

	clang::tooling::CommonOptionsParser optionsParser(argc, argv, toolCategory);

	auto sources = optionsParser.getSourcePathList();

	clang::tooling::ClangTool tool(optionsParser.getCompilations(), sources);

	int result = tool.run(clang::tooling::newFrontendActionFactory<MyAction>().get());

//	if (argc > 1) {
//		clang::tooling::runToolOnCode(new MyAction, argv[1]);
//	}

//	llvm::IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOpts = new clang::DiagnosticOptions();

//	clang::TextDiagnosticPrinter* diagClient = new clang::TextDiagnosticPrinter(llvm::errs(), &*diagOpts);


//	llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagID(new clang::DiagnosticIDs());

//	clang::DiagnosticsEngine diags(diagID, &*diagOpts, diagClient);

//	clang::CompilerInstance ci;
//	ci.setDiagnostics(&diags);


//	clang::CompilerInvocation in;
//	clang::CompilerInvocation::CreateFromArgs(in, argv+1, argv + argc, ci.getDiagnostics());

//	if (in.getFrontendOpts().ShowHelp)
//	{
//		std::cout << "This is help" << std::endl;
//	}
//	else if (in.getFrontendOpts().Inputs.empty())
//	{
//		std::cout << "No inputs" << std::endl;
//	}
//	else
//	{
////		llvm::IntrusiveRefCntPtr<clang::vfs::FileSystem> fs = clang::vfs::getRealFileSystem();
////		clang::SourceManager sourceManager(diags, fs);
////		clang::TargetInfo targetInfo(diags, in.getTargetOpts());

////		clang::HeaderSearch headerSearch(in.getHeaderSearchOpts(), sourceManager, diags, in.getLangOpts(), &targetInfo);


//		for(const auto& input : in.getFrontendOpts().Inputs)
//		{
//			std::string f = input.getFile();
//			std::cout << "Input: " << f << std::endl;




////			clang::Preprocessor pp(
////				in.getPreprocessorOpts(),
////				diags,
////				in.getLangOpts(),
////				sourceManager,
////				headerSearch,

//			ci.createFileManager();
//			ci.createSourceManager(ci.getFileManager());
//			ci.createPreprocessor(clang::TU_Complete);
//			ci.createASTContext();

//			MyAstConsumer astConsumer;

//			ci.setASTConsumer(&astConsumer);

//			ci.createSema(clang::TU_Complete, nullptr);
//			ci.InitializeSourceManager(input);

//			clang::ParseAST(ci.getSema(), true);
//			//clang::ParseAST(pp, &astConsumer, ctx);


//		}
//	}

}
