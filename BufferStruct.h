#define PAGE_SIZE 8192 
#define SUCCESS 0
#define FAIL 1 
struct frame{
	char content[PAGE_SIZE];
};
struct page{
	frame content;
	int pageId;
	bool isModified;
};


