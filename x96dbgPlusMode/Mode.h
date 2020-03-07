#pragma once


#include "PluginMain.h"



/*
	传入BridgeCFNodeList类型的数组绘制程序连线图
*/
void graphAnalyze(){

	BridgeCFNodeList *list;
	BridgeCFGraphList * graph = new BridgeCFGraphList;

	DbgAnalyzeFunction(0x7ff8dd227b30, graph);

	list = reinterpret_cast<BridgeCFNodeList*>(graph->nodes.data);
	PLOG("%lld\n", list[1].start);

	GuiLoadGraph(graph, 0x7ff8dd227b30);
}

bool InRange(duint addrs) 
{


}


void AnalyzeDebuger() {

	duint next;
	while (InRange(next))
	{

		if (InRange)
		{
			Script::Debug::StepIn();
		}
		else
		{
			Script::Debug::StepOver();
		}

	}








	//DbgCmdExecDirect("StepOver");
	//DbgCmdExecDirect("StepInto");
	//DbgCmdExecDirect("StepOut");

}






