#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAXSIZE 1024//4K
typedef unsigned char byte;
class mipscpu
{
private:
	int Memory[MAXSIZE];
	int pagetable[4];
	int pag, ofs;
	int LRU[4];
public:
	mipscpu(){
		FILE *fp;
		int i;
		char a[1000];
		memset(LRU, 0, sizeof(LRU));
		memset(Memory,0, sizeof(Memory));
		memset(pagetable, -1, sizeof(pagetable));
		for (i = 0; i < 4; i++) LRU[i] = i;
		for (i = 0; i < 1024; i++)
		{
			char d[1000] = "mem";
			sprintf(a, "%d", i);
			strcat(d, a);
			strcat(d, ".txt");
			fp = fopen(d, "wb");
			fwrite(&Memory[0], 1, 1024, fp);
			fclose(fp);
		}
	}
	void putLRU(int p)
	{
		int i,pos=3;
		for (i = 0; i < 4; i++)
			if (LRU[i] == p){
				pos = p;
			}
		for (i = pos - 1; i >= 0; i--)
			LRU[i + 1] = LRU[i];
		LRU[0] = p;
	}
	int     getmem(int adr)
	{
		int p, p1;  //1K/page
		int i;
		char d[1000] = "mem";
		char a[1000];
		FILE *fp;
		pag = adr / 1024;
		ofs = adr % 1024;

		for (i = 0; i<4; i++){
			if (pag == pagetable[i]){
				putLRU(i);
				return 1024 * i + ofs;
			}
		}

		p1 = LRU[3];
		sprintf(a, "%d", pagetable[p1]);
		strcat(d, a);
		strcat(d, ".txt");
		fp = fopen(d, "wb");
		fwrite(&Memory[p1 * 1024/4], 1, 1024, fp);
		fclose(fp);
		strcpy(d, "mem");
		pagetable[p1] = pag;
		sprintf(a, "%d", pagetable[p1]);
		strcat(d, a);
		strcat(d, ".txt");
		fp = fopen(d, "rb");
		fread(&Memory[p1 * 1024/4], 1, 1024, fp);
		for (i = 0; i < 4; i++) LRU[i + 1] = LRU[i];
		LRU[0] = p1;
		fclose(fp);
		return p1 * 1024 + ofs;
	}
	int 	lw(int adr)
	{
		int m,addr;
		addr = getmem(adr);
		switch (addr % 4){
		case 0:
			m = Memory[addr >> 2];
			break;
		case 1:
			m = (Memory[addr >> 2]) << 8 | (Memory[(addr >> 2) + 1] >> 24);
			break;
		case 2:
			m = (Memory[addr >> 2] << 16) | (Memory[(addr >> 2) + 1] >> 16);
			break;
		case 3:
			m = (Memory[addr >> 2] << 24) | (Memory[(addr >> 2) + 1] >> 8);
			break;
		}
		return m;
	}
	void	sw(int adr, int dat)
	{
		int  a, b, c, d,addr;
		addr = getmem(adr);
		a = dat >> 24;
		b = (dat >> 16) & 0xFF;
		c = (dat >> 8) & 0xFF;
		d = dat & 0xFF;
		switch (addr % 4){
		case 0:
				Memory[addr >> 2] = dat;
				break;
		case 1:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0xFF000000) | (a << 16) | (b << 8) | c;
				Memory[(addr >> 2) + 1] = (Memory[(addr >> 2) + 1] & 0x00FFFFFF) | (d << 24);
			    break;
		case 2:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0xFFFF0000) | (a << 8) | b;
				Memory[(addr >> 2) + 1] = (Memory[(addr >> 2) + 1] & 0x0000FFFF) | (c << 24) | (d << 16);
				break;
		case 3:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0xFFFFFF00) | a;
				Memory[(addr >> 2) + 1] = (Memory[(addr >> 2) + 1] & 0xFF) | (b << 24) | (c << 16) | (d << 8);
			    break;
		}
	}
	int 	lh(int adr)
	{
		int m, addr;
		addr = getmem(adr);
		switch (addr % 4){
		case 0:
				m = Memory[addr >> 2] >> 16;
			    break;
		case 1:
				m = (Memory[addr >> 2] >> 8) & 0xFFFF;
			    break;
		case 2:
				m = Memory[addr >> 2] & 0xFFFF;
			    break;
		case 3:
				m = (Memory[addr >> 2] & 0xFF) << 8 | (Memory[(addr >> 2) + 1] >> 24);
			    break;
		}
		return m;
	}
	int 	lhu(int adr)
	{
		int m, addr;
		addr = getmem(adr);
		switch (addr % 4){
		case 0:
				m = Memory[addr >> 2] >> 16;
			    break;
		case 1:
				m = (Memory[addr >> 2] >> 8) & 0xFFFF;
			    break;
		case 2:
				m = Memory[addr >> 2] & 0xFFFF;
			    break;
		case 3:
				m = (Memory[addr >> 2] & 0xFF) << 8 | (Memory[(addr >> 2) + 1] >> 24);
			    break;
		}
		m = (unsigned int)m;
		return m;
	}
	void	sh(int adr, short dat)
	{
		int a, b,addr;
		addr = getmem(adr);
		a = dat >> 8;
		b = dat & 0xFF;
		switch (addr % 4){
		case 0:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0x0000FFFF) | (a << 24) | (b << 16);
			    break;
		case 1:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0xFF0000FF) | (a << 16) | (b << 8);
			    break;
		case 2:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0xFFFF0000) | dat;
			    break;
		case 3:
				Memory[addr >> 2] = (Memory[addr >> 2] & 0xFFFFFF00) | a;
				Memory[(addr >> 2) + 1] = (Memory[(addr >> 2) + 1] & 0x00FFFFFF) | (b << 24);
			    break;
		}
	}
	int	lb(int adr)
	{
		int m,addr;
		addr = getmem(adr);
        switch (addr % 4){
		case 0:
				m = Memory[addr >> 2] >> 24;
			    break;
		case 1:
				m = (Memory[addr >> 2] >> 16) & 0xFF;
			    break;
		case 2:
				m = (Memory[addr >> 2] >> 8) & 0xFF;
			    break;
		case 3:
				m = Memory[addr >> 2] & 0xFF;
			    break;
		}
		return m;
	}
	int 	lbu(int adr)
	{
		int m,addr;
		addr = getmem(adr);
		switch (addr % 4){
		case 0:
				m = Memory[addr >> 2] >> 24;
			    break;
		case 1:
				m = (Memory[addr >> 2] >> 16) & 0xFF;
			    break;
		case 2:
				m = (Memory[addr >> 2] >> 8) & 0xFF;
			    break;
		case 3:
				m = Memory[addr >> 2] & 0xFF;
			    break;
		}
		m = (unsigned int)m;
		return m;
	}
	void	sb(int adr, byte dat)
	{
		int addr;
		addr = getmem(adr);
		switch (addr % 4){
		case 0:
			Memory[addr >> 2] = (Memory[addr >> 2] & 0x00FFFFFF) | (dat << 24);
			break;
		case 1:
			Memory[addr >> 2] = (Memory[addr >> 2] & 0xFF00FFFF) | (dat << 16);
			break;
		case 2:
			Memory[addr >> 2] = (Memory[addr >> 2] & 0xFFFF00FF) | (dat << 8);
			break;
		case 3:
			Memory[addr >> 2] = (Memory[addr >> 2] & 0xFFFFFF00) | dat;
			break;
		}
	}
};
int main()
{
	mipscpu*one =new(mipscpu);
	int a, b;
	one->sw(0, 5);
	one->sh(1024*768, 2);
	a = one->lw(0);
	printf("%x\n", a);
	b = one->lh(1024*768);
	printf("%x\n", b);
	system("pause");
}
