#include "CompilerC0.h"
#include "Lexer.h"
#include "Tag.h"
#include <iostream>
#include "Executor.h"
#include "CodeGenerator.h"
#include <Windows.h>
#include <fstream>
#include "DSAOptimizer.h"
#include "GRDOptimizer.h"
#include "CSDOptimizer.h"

using namespace std;

void preProcess(FILE*& in,char* filename,string& path,string& file,string& asmpath){
	cout<<"Please input the file directory:"<<endl;
	//Open file
 	int j;
	for(j=0;j<1000;j++){
		char c=getchar();
		if(c=='\n'){
			filename[j]=0;
			break;
		}
		filename[j]=c;
	}
	while((in=fopen(filename,"r"))==NULL){
		cout<<"Can't open the input file. Please check the directory, and input again or exit:"<<endl;
		for(j=0;j<1000;j++){
			char c=getchar();
			if(c=='\n'){
				filename[j]=0;
				break;
			}
			filename[j]=c;
		}
	}	
	cout<<"File successfully opened."<<endl;
	//get path
	string tpath(filename);
	while (tpath[tpath.length() - 1] != '\\'){
		tpath.resize(tpath.size() - 1);
	}
	path=tpath;
	//get filename without extensive name
	int start=-1,end=-1;
	for(int i=j;i>=0;i--){
		if(end==-1&&filename[i]=='.')
			end=i;
		if(start==-1&&filename[i]=='\\')
			start=i;
		if(start!=-1&&end!=-1)
			break;
	}
	if(start>end)
		end=j;
	for(i=start+1;i<end;i++){
		file+=filename[i];
	}
	//get path for .asm
	asmpath=path+file+".asm";
}

void main()
{
	char filename[1000]="";
	string path="",file="",asmpath="";
	FILE *in=NULL;
	preProcess(in,filename,path,file,asmpath);
	QuadrupleMgr* QMgr=new QuadrupleMgr();
	SymbolTableMgr* SMgr=new SymbolTableMgr();
	Parser* parser=new Parser(in,SMgr,QMgr);
	CodeGenerator* codeGenerator=new CodeGenerator(SMgr,QMgr,asmpath);
	cout<<"Compiling..."<<endl<<endl<<"Report:"<<endl;
	Executor* executor=new Executor(path,file);
	parser->Program();
	if(parser->errorcount==0)
		cout<<"-->"<<"No Errors"<<endl;
	else
		cout<<"-->"<<parser->errorcount<<" Error(s)"<<endl;
	cout<<"============================"<<"Compilation";
	if(parser->errorcount>0)
	{
		cout<<" failed"<<"============================"<<endl;
		cout<<"Please modify your source file."<<endl<<endl;
		exit(0);
	}
	else{
		cout<<" succeeded"<<"============================"<<endl<<endl;
	}
	SMgr->DisplayAll();
	cout<<endl;
	QMgr->DisplayAllQuads();
	cout<<endl;
	QMgr->DisplayAllItems();
	cout<<endl;
	delete parser;
	parser=NULL;

	DSAOptimizer* DSAO=NULL;
	GRDOptimizer* GRDO=NULL;
	CSDOptimizer* CSDO=NULL;

	DSAO=new DSAOptimizer(SMgr,QMgr);
	DSAO->Initial();

	GRDO=new GRDOptimizer(SMgr,QMgr,DSAO->bbs,DSAO->bbsNumber);
	GRDO->CreateConflict();
	GRDO->RegisterDistribute();
	GRDO->Generate();
	QMgr->DisplayAllQuads();

	cout<<"Common Statment delete:"<<endl;
	CSDO=new CSDOptimizer(SMgr,QMgr,DSAO->bbs,DSAO->bbsNumber);
	CSDO->Execute();
	QMgr->DisplayAllQuads();

	codeGenerator->GenInitial(executor->include,executor->lib,executor->macro);
	codeGenerator->Execute();
	executor->Execute();
	fclose(in);
	return;
}