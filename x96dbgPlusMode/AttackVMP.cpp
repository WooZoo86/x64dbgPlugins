#include <algorithm>
#include "AttackVMP.h"
#include "Plugin.h"
#include "x96CallbackFunction.h"

using namespace Script;

CAttackVMP::CAttackVMP()
{
}

CAttackVMP::~CAttackVMP()
{
}

AnalysisResult CAttackVMP::StaticAnalysis(duint AnalysisBeginAddress)
{
	if (AnalysisBeginAddress == 0)
	{
		AnalysisBeginAddress = MainModuleEntry;
	}
	NextExce = AnalysisBeginAddress;
	cb.BeginAddr = NextExce;
	while ((MainModuleBase < NextExce) && (NextExce < (MainModuleBase + MainModuleSize)))
	{		
		DbgDisasmAt(NextExce, &instr);
		PLOG("%llx    %s", NextExce, instr.instruction);
		switch (instr.type)
		{
		case DISASM_INSTRTYPE::instr_normal:
			InstructionNormal();
			NextExce += instr.instr_size;
			break;
		case DISASM_INSTRTYPE::instr_branch:
			NextExce = InstructionBranch();
			break;
		case DISASM_INSTRTYPE::instr_stack:
			InstructionStack();
			NextExce += instr.instr_size;
			break;
		default:
			break;
		}
		if (stopanalysis)
		{
			break;
		}

	}
	//Debug::StepOver();
	//PLOG("%llx", RegIP);
	return AnalysisResult::Unknow;
}

void CAttackVMP::InstructionNormal()
{
	char instruction[64];
	InstructionOperate(instr.instruction, instruction);
	if (!strcmp("int3", instruction))/* int3 很可能解析错误先返回 */
	{
		stopanalysis = true;
		errorstr = "operater : Int 3 \n";
	}
}

duint CAttackVMP::InstructionBranch()
{
	char instruction[64];
	std::string err;
	duint jmpaddr;
	InstructionOperate(instr.instruction, instruction);
	switch (instr.argcount)
	{
	case 0:/*没有操作数 好像只有ret*/
		/*???error???*/
		jmpaddr = NextExce;/*将当前执行地址给jmpaddr过解析模块检测*/
		break;
	case 1:/*一个操作数*/
		if (instr.arg[0].type == DISASM_ARGTYPE::arg_normal && instr.arg[0].segment == SEGMENTREG::SEG_DEFAULT)
		{
			jmpaddr = instr.arg[0].constant;
		}
		else
		{
			std::string errbuf = "InstructionBranch->jmp->case 1->else\n";
			err = err + errbuf;
		}
		break;
	case 2:/*两个操作数*/
		break;
	case 3:/*三个操作数*/
		break;
	default:
		break;
	}
	bool inblock = false;
	/*跳转地址是否在需要解析的代码块*/
	std::find_if(std::begin(analysisblock), std::end(analysisblock), [&](decltype(analysisblock)::reference block) {
		if ((block.BaseAddress < jmpaddr) && (jmpaddr < (block.BaseAddress + block.Size)))
		{
			inblock = true;
			return true;
		}
		else
			return false;
	});
	if (inblock)
	{
		if (!strcmp("call", instruction))/* call */
		{
			if (CheckCallNotAnalysis(jmpaddr)) {
				cb.EndAddr = NextExce;
				codeblock.push_back(cb);
				cb.BeginAddr = jmpaddr;
				functionaddress.push_back(jmpaddr);
				callstack.push(Call{ NextExce , instr.instr_size });
				return jmpaddr;/* call address */
			}
			else
			{
				return NextExce + instr.instr_size;
			}
		}
		else if (!strcmp("ret", instruction))/* ret */
		{
			Call call = callstack.top();
			callstack.pop();
			cb.EndAddr = NextExce;
			codeblock.push_back(cb);
			cb.BeginAddr = call.ReturnAddress + call.OperateSize;
			return call.ReturnAddress + call.OperateSize;/* from call return */
		}
		else if (!strcmp("jmp", instruction))/* jmp */
		{
			cb.EndAddr = NextExce;
			codeblock.push_back(cb);
			cb.BeginAddr = jmpaddr;
			return jmpaddr;
		}
		else/* j?? */
		{
			/*
			std::string errbuf = "Unknow Operate : " + std::string(instr.instruction) + "\n";
			err = err + errbuf;
			errorstack.emplace_back(err);
			*/
			//if( std::end(jmpaddress)==std::find(std::begin(jmpaddress), std::end(jmpaddress), JmpIntoPoint{ JmpIntoPointType::Jxx ,NextExce,jmpaddr }));
			//{
			cb.EndAddr = NextExce;
			codeblock.push_back(cb);
			cb.BeginAddr = NextExce + instr.instr_size;
			jmpaddress.emplace_back(JmpIntoPoint{ JmpIntoPointType::Jxx ,NextExce,jmpaddr });
			return NextExce + instr.instr_size;/* 返回下一个指令地址 */
			//}
		}
	}
	else
		return NextExce + instr.instr_size;
}


void CAttackVMP::InstructionStack()
{

}

void CAttackVMP::InstructionOperate(const char * instruction, char * dest)
{
	char * src= (char *)instruction;
	while ( *src != ' ' &&  *src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
}

bool CAttackVMP::CheckAddrNotIncodeblock(duint address)
{

	auto it = std::find_if(std::begin(codeblock),std::end(codeblock), [=](CodeBlock cdblk) {
		if ((cdblk.EndAddr<= address) && (address <= cdblk.BeginAddr))
		{
			return true;
		}
		return false;
	});
	if (it == std::end(codeblock))
		return true;
	else
		return false;
}

bool CAttackVMP::CheckCallNotAnalysis(duint address)
{
	auto it = std::find_if(std::begin(functionaddress), std::end(functionaddress), [=](duint calladdr) {
		if (calladdr == address)
		{
			return true;
		}
		return false;
	});
	if (it == std::end(functionaddress))
		return true;
	else
		return false;
}

void CAttackVMP::Reset()
{
	stopanalysis = false;
	errorstack.clear();
	codeblock.clear();
	while (!callstack.empty())
	{
		callstack.pop();
	}
	functionaddress.clear();
	jmpaddress.clear();

}

void cbMenuCallback(CBTYPE type, PLUG_CB_MENUENTRY* info)
{
	/*
	switch (info->hEntry)
	{
	case MenuList::PLUGIN_MENU_ABOUT:
		MessageBoxA(hwndDlg, "Attack VMP", "About", MB_OK + MB_ICONINFORMATION);
		break;
	case MenuList::PLUGIN_MENU_ANALYSIS:
		TrackExecution();
	default:
		break;
	}*/
}

#ifdef _WIN64
#define GetIP GetRIP
#else
#define GetIP GetEIP
#endif // _WIN64
/*
typedef struct
{
	char instruction[64];
	DISASM_INSTRTYPE type;
	int argcount;
	int instr_size;
	DISASM_ARG arg[3];
} DISASM_INSTR;

void TrackExecution()
{
	//duint RegIP = Register::GetIP();

	DISASM_INSTR * instr = new DISASM_INSTR;
	duint NextExce = MainModuleEntry;
	while ( (MainModuleBase < NextExce) && (NextExce < (MainModuleBase + MainModuleSize)) )
	{
		DbgDisasmAt(NextExce, instr);
		printf("%llx    %s",NextExce,instr->instruction);
		if (instr->type == DISASM_INSTRTYPE::instr_branch)
		{
		//	if(instr->argcount < )
		}
		NextExce += instr->instr_size;
	}
	delete instr;
	//Debug::StepOver();
	//PLOG("%llx", RegIP);
	PLOG("%s", instr->instruction);

}
*/


