#include "mlpa/analyzers/Analyzers.h"
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Lex/Lexer.h>
#include <set>

namespace mlpa {
namespace {
SourceLocation loc(const clang::SourceManager &sm, clang::SourceLocation l){ auto p=sm.getPresumedLoc(l); return {p.isValid()?p.getFilename():"<unknown>", p.isValid()?p.getLine():0, p.isValid()?p.getColumn():0}; }
void add(AnalysisResult&r,const clang::SourceManager&sm,clang::SourceLocation l,std::string id,Severity s,std::string t,std::string e,std::string fix){ r.issues.push_back({std::move(id),Category::MemorySafety,s,loc(sm,l),std::move(t),std::move(e),std::move(fix)}); }
class Visitor: public clang::RecursiveASTVisitor<Visitor>{
public: Visitor(clang::ASTContext&c,AnalysisResult&r):ctx(c),res(r),sm(c.getSourceManager()){}
 bool VisitCXXNewExpr(clang::CXXNewExpr*e){ news.insert(e); add(res,sm,e->getBeginLoc(),"MEM001",Severity::High,"Raw new allocation","Manual allocation is exception-prone and can leak when ownership is unclear.","Prefer std::unique_ptr, std::make_unique, or a standard container."); return true; }
 bool VisitCXXDeleteExpr(clang::CXXDeleteExpr*e){ auto text=clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(e->getArgument()->getSourceRange()),sm,ctx.getLangOpts()).str(); if(!deleted.insert(text).second) add(res,sm,e->getBeginLoc(),"MEM003",Severity::Critical,"Possible double delete","The same delete operand appears more than once in this translation unit.","Delete through a single owner and set raw pointers to nullptr only as a transitional mitigation."); return true; }
 bool VisitVarDecl(clang::VarDecl*d){ if(d->getType()->isPointerType()) add(res,sm,d->getLocation(),"MEM002",Severity::Medium,"Raw pointer declaration","Raw pointers make ownership and lifetime hard to audit.","Use references, std::span, std::unique_ptr, std::shared_ptr, or observer annotations as appropriate."); if(d->getType()->isArrayType()) add(res,sm,d->getLocation(),"MEM004",Severity::Medium,"C-style array","Fixed C arrays decay to pointers and lack bounds-aware APIs.","Use std::array for fixed size or std::vector for dynamic size."); return true; }
private: clang::ASTContext&ctx; AnalysisResult&res; const clang::SourceManager&sm; std::set<const clang::CXXNewExpr*>news; std::set<std::string>deleted;};}
void MemorySafetyAnalyzer::analyze(clang::ASTContext &context, AnalysisResult &result){ Visitor(context,result).TraverseDecl(context.getTranslationUnitDecl()); }
}
