#include <iostream>

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/CompilerInvocation.h>
#include <clang/Basic/Version.h>

int main(int argc, char** argv)
{
	clang::CompilerInstance ci;

	clang::CompilerInvocation in;
	//clang::CompilerInvocation::CreateFromArgs(in, argv+1, argv + argc, ci.getDiagnostics());



	//ci.setInvocation(&in);
	//ci.setTarget(clang::TargetInfo::CreateTargetInfo( ci.getDiagnostics(), in.getTargetOpts()));

}
