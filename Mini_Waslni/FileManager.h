#pragma once
#include "Graph.h"

class FileManager
{
public:
	static void loadGraph(Graph graph, string filename);
	static void saveGraph(Graph& graph, string filename);
};

