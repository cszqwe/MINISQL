#define PAGE_SIZE 8192 
struct frame{
	char content[PAGE_SIZE];
};
struct page{
	frame content;
	int pageId;
	bool isModified;
};


