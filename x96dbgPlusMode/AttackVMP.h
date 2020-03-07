#pragma once
#include "PluginMain.h"
#include<stack>

enum AnalysisResult
{
	Success,
	failure,
	Unknow
}; 
enum JmpIntoPointType
{
	Jmp,
	Jxx

};

struct Block  
{
	duint BaseAddress;
	duint Size;
};

struct CodeBlock
{
	duint BeginAddr;
	duint EndAddr;
};

struct JmpIntoPoint
{
	JmpIntoPointType type;
	duint From;
	duint To;
};

struct Call 
{
	duint ReturnAddress;
	int OperateSize;
};

//Show About Dialog
class CAttackVMP
{

public:
	static CAttackVMP & GetInstance() {
		static CAttackVMP instance;
		return instance;
	}
	CAttackVMP();

	~CAttackVMP();

	/*中止解析*/
	bool stopanalysis;

	/*错误信息*/
	std::string errorstr;

	/*汇编指令结构*/
	DISASM_INSTR instr;

	/*下一条需要解析的指令*/
	duint NextExce;

	/*当前块临时变量*/
	CodeBlock cb;

	/*错误信息*/
	std::list<std::string> errorstack;

	/*需要解析的模块(地址范围)*/
	std::list<Block> analysisblock;

	/*已经解析的代码快*/
	std::list<CodeBlock> codeblock;

	/*模拟函数调用堆栈 */
	std::stack<Call> callstack;

	/*call入口地址表*/
	std::list<duint> functionaddress;

	/*jxx跳转表*/
	std::list<JmpIntoPoint> jmpaddress;

	/*
		解析程序运行结构
		AnalysisBeginAddress : 开始解析的地址
	*/
	AnalysisResult StaticAnalysis(duint AnalysisBeginAddress = 0);
	/*
		解析Normal类型指令
	*/
	void InstructionNormal();
	/*
		解析Branch类型指令
	*/
	duint InstructionBranch();
	/*
		解析Stack类型指令
	*/
	void InstructionStack();
	/*
		获取反汇编指令
		instruction : 传入的反汇编指令
		dest : 返回的指令字符串
	*/
	void InstructionOperate(const char * instruction ,char * dest);
	/*
		检测跳转位置是否解析过
	*/
	bool CheckAddrNotIncodeblock(duint address);
	/*
		检测跳转位置是否解析
	*/
	bool CheckCallNotAnalysis(duint address);
	/*
		清除上次的解析数据
	*/
	void Reset();

	duint MainModuleBase;
	duint MainModuleSize;
	duint MainModuleEntry;
	int pluginHandle;
	HWND hwndDlg;
	int hMenu;
	int hMenuDisasm;
	int hMenuDump;
	int hMenuStack;
	HINSTANCE hInstance;
};






void cbMenuCallback(CBTYPE type, PLUG_CB_MENUENTRY* info);

//void TrackExecution();

