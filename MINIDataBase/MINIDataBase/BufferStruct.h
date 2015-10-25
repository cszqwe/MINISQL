#define PAGE_SIZE 8192 
#define SUCCESS 0
#define FAIL 1 
#include <string>
using namespace std;
struct frame{
	char content[PAGE_SIZE];
};
struct page{
	frame content;
	int pageId;
	bool isModified;
	int modifyContent(string newContent){
		if (newContent.size()>PAGE_SIZE) return FAIL;
		for (int i = 0; i< newContent.size(); i++){
			content.content[i] = newContent[i];
		}
		for (int i = newContent.size(); i < PAGE_SIZE; i++){
			content.content[i] = ' ';
		}
	}
};


