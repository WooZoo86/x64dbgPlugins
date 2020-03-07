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

	/*��ֹ����*/
	bool stopanalysis;

	/*������Ϣ*/
	std::string errorstr;

	/*���ָ��ṹ*/
	DISASM_INSTR instr;

	/*��һ����Ҫ������ָ��*/
	duint NextExce;

	/*��ǰ����ʱ����*/
	CodeBlock cb;

	/*������Ϣ*/
	std::list<std::string> errorstack;

	/*��Ҫ������ģ��(��ַ��Χ)*/
	std::list<Block> analysisblock;

	/*�Ѿ������Ĵ����*/
	std::list<CodeBlock> codeblock;

	/*ģ�⺯�����ö�ջ */
	std::stack<Call> callstack;

	/*call��ڵ�ַ��*/
	std::list<duint> functionaddress;

	/*jxx��ת��*/
	std::list<JmpIntoPoint> jmpaddress;

	/*
		�����������нṹ
		AnalysisBeginAddress : ��ʼ�����ĵ�ַ
	*/
	AnalysisResult StaticAnalysis(duint AnalysisBeginAddress = 0);
	/*
		����Normal����ָ��
	*/
	void InstructionNormal();
	/*
		����Branch����ָ��
	*/
	duint InstructionBranch();
	/*
		����Stack����ָ��
	*/
	void InstructionStack();
	/*
		��ȡ�����ָ��
		instruction : ����ķ����ָ��
		dest : ���ص�ָ���ַ���
	*/
	void InstructionOperate(const char * instruction ,char * dest);
	/*
		�����תλ���Ƿ������
	*/
	bool CheckAddrNotIncodeblock(duint address);
	/*
		�����תλ���Ƿ����
	*/
	bool CheckCallNotAnalysis(duint address);
	/*
		����ϴεĽ�������
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

