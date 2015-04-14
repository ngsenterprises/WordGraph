
//#include <cstring>
#include <vector>
#include <stack>
#include <unordered_map>
#include <string.h>

struct aspNode
{
	char letter;
	bool isword;

	std::unordered_map<char, aspNode*> children;

	aspNode() : letter(0), isword(false) {};
	aspNode(char letter, bool isword = false) : letter(letter), isword(isword) {};
};


struct aspWordSuffix
{
	char *word;
	int wordlen;
	char *suffix;
	aspWordSuffix() : word(0), wordlen(0), suffix(0) {};
	aspWordSuffix(char *word, int wordlen, char *suffix) : word(word), wordlen(wordlen), suffix(suffix) {};
};


class aspGraph
{
public:
	aspGraph() : root(0), num_compound_words(0), max_word(""), wordStack() {};

	aspNode root;
	int num_compound_words;
	std::string max_word;
	std::vector<aspWordSuffix*> wordStack;

	void insert(char *word);
	bool isCompoundWord(char *data);

	void freeNode(aspNode *);
	void freeGraph();

};

int initdata(std::vector<std::string> &words, std::string &fn);


