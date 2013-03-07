#include<stdio.h>
#include<stdlib.h>
#include "library.h"
int main(int argc, char* argv[])
{
	char *p;
	int bookId;
	if (argc!=2 || (bookId=strtol(argv[1],&p,10),p==argv[1]))
	{
		FILE *log=fopen("book.log","a");
		fputs("invalid query arguments\n",log);
		fclose(log);
		return 0;
	}

	FILE* books=fopen("books","rb");
	fseek(books,0,SEEK_END);
	int size=ftell(books);

	if (bookId<0 || (bookId)*sizeof(struct BookInfo)>=size)
	{
		printf("invalid %d\n",bookId);
		FILE *log=fopen("book.log","a");
		char str[30];
		sprintf(str,"invalid book index %d\n",bookId);
		fputs(str,log);
		fclose(log);
	}
	else
	{
		fseek(books,bookId*sizeof(struct BookInfo),SEEK_SET);
		struct BookInfo book;
		fread(&book,sizeof(book),1,books);
		printf("%s %d\n",book.title,book.number_of_resv_req);
		int i;
		for(i=0;i<16;i++)
		{

			if (i<=book.number_of_resv_req)
				printf("%d",book.userID_for_resv_request[i]);
			else
				printf("#");
			if (i==7||i==15)
				printf("\n");
			else
				printf(" ");
		}
	}
	fclose(books);
	return 0;
}
