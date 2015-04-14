

#include "WordGraph.h"


#include <ctime>
#include <ratio>
#include <chrono>
#include <stdio.h>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

#include <string>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS


namespace {

	typedef std::unordered_map<char, aspNode*>::const_iterator Itr;

	char *data[] = { "cat", "cats", "catsdogcats", "catxdogcatsrat",
		"dog", "dogcatsdog", "hippopotamuses", "rat", "ratcat",
		"ratcatdog", "ratcatdogcat", "sogcatscats", "sojcatscats" };

	//    const size_t FILE_COLUMNS   = 1024L;
	//    const size_t FILE_ROWS      = 1024L;
	//    const size_t FILE_READ      = 1047552L;

	//char sdata[FILE_READ];
	//char rawData[FILE_READ] = {0};
	//char strData[1047552L];

	std::vector<std::string> word_list;



}



bool aspGraph::isCompoundWord(char *suffix)
{
	//printf("isCompoundWord [%s] ", suffix);

	aspNode *cur = &(this->root);
	Itr itr;
	char *pchar = suffix;

	while (*pchar != 0)
	{
		if ((itr = cur->children.find(*pchar)) == cur->children.end())
		{
			//printf("false\n");
			return false;
		}
		cur = (aspNode*)itr->second;
		if (cur->isword)
		{
			if (*(pchar + 1) == 0)
			{
				//printf("true\n");
				return true;
			}


			if (isCompoundWord(pchar + 1))
			{
				//printf("true\n");
				return true;
			}
		}

		pchar += 1;
	}

	return (cur->isword) ? true : false;

}//--------------------------------------------------------------


void aspGraph::insert(char *word)
{
	aspNode *cur = &(this->root);
	char *pchar = word;
	std::vector<char*> suffix;
	Itr itr;

	int word_len = 0;

	//printf("[");
	while (*pchar != 0)
	{
		//printf("%c", *pchar);

		//list does contain the character 
		if ((itr = (Itr)cur->children.find(*pchar)) != cur->children.end())
		{
			cur = (aspNode*)itr->second;
			if (cur->isword && (pchar + 1) != 0)
				suffix.push_back(pchar + 1);
		}
		else
		{
			aspNode *next = new aspNode(*pchar);
			cur->children[*pchar] = next;
			cur = next;
		}
		word_len += 1;
		pchar += 1;
	}

	if (cur != nullptr) {
		cur->isword = true;
		//printf("]\n");
		if (!suffix.empty())
		{
			for (char *p : suffix)
				wordStack.push_back(new aspWordSuffix(word, word_len, p));
		}
	}

};//-------------------------------------


void aspGraph::freeNode(aspNode *node)
{
	for (Itr itr = node->children.begin(); itr != node->children.end(); itr++)
		freeNode((aspNode*)itr->second);
	delete node;
};//----------------------------------------


void aspGraph::freeGraph()
{
	for (Itr itr = root.children.begin(); itr != root.children.end(); itr++)
		freeNode((aspNode*)itr->second);
	wordStack.clear();
};//---------------------------------





int main(int argc, char *argv[])
{
	using namespace std::chrono;
	//std::string fn = "C:\\data\\db\\little_word.txt";//"C:\\data\\db\\little_word.txt";

	if (argc < 2)
	{
		printf("need file name.\n");
		return 0;
	}
	
	std::string fn(argv[1]);
	//printf("%s\n", fn.c_str());

	if (initdata(word_list, fn) != 0)
		return -1;

	aspGraph graph;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	std::vector<std::string>::iterator it = word_list.begin();
	while (it != word_list.end())
	{
		graph.insert((char*)((*it).c_str()));
		++it;
	}

	char *max_word = 0;
	int max_length = 0;
	int compound_count = 0;

	std::vector<aspWordSuffix*>::iterator itr;
	for (itr = graph.wordStack.begin(); itr != graph.wordStack.end(); itr++)
	{
		//printf("[%s][%s]\n", (*itr)->word, (*itr)->suffix);

		if (graph.isCompoundWord((*itr)->suffix))
		{
			compound_count += 1;
			if (max_length < (*itr)->wordlen)
			{
				max_word = (*itr)->word;
				max_length = (*itr)->wordlen;
				//printf("maxword [%s]\n", max_word);
			}
		}
	}

	high_resolution_clock::time_point t2 = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

	printf("elapsed %f\n", time_span.count());
	printf("compound words : %d \n", compound_count);
	printf("max word       : [%s]\n", max_word);

	graph.freeGraph();

	return 0;

};//end main







int initdata(std::vector<std::string> &words, std::string &fn)
{
	std::string str;
	std::ifstream ifs(fn);
	
	while (ifs >> str)
	{
		if (!str.empty())
		{
			words.push_back(str);
			//printf("[%s]\n", str.c_str());
		}
	}
	ifs.close();

	//printf("elems %d\n", words.size());

	return 0;
};



