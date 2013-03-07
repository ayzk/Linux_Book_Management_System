#include<stdio.h>
#include<stdlib.h>
#include "library.h"

int main(int argc, char* argv[])
{
//	argc=3;
//	argv[1]="0";
//	argv[2]="2";
	FILE *log=fopen("book.log","a");
	char str[50];

	char *p1;
	int bookId;
	if ( argc!=2 || ( bookId=strtol(argv[1],&p1,10),p1==argv[1]))
	{
		fputs("invalid return arguments\n",log);
		fclose(log);
		return 0;
	}

	FILE* books=fopen("books","r+b");
	fseek(books,0,SEEK_END);
	int size=ftell(books);

    if (bookId<0 || (bookId)*sizeof(struct BookInfo)>=size)
	{
		sprintf(str,"invalid book index %d\n",bookId);
		fputs(str,log);
		fclose(log);
		fclose(books);
		return 0;
	}
	else
	{
		fseek(books,bookId*sizeof(struct BookInfo),SEEK_SET);
		struct BookInfo book;
		fread(&book,sizeof(book),1,books);
		if (book.userID_for_resv_request[0]==NO_USER)
		{
			sprintf(str,"%d is not held by anyone\n",bookId);
			fputs(str,log);
		}
		else
		{
			sprintf(str,"%d returned %d\n",book.userID_for_resv_request[0],bookId);
			fputs(str,log); 
			book.userID_for_resv_request[0]=NO_USER;
			fseek(books,bookId*sizeof(struct BookInfo),SEEK_SET);
			fwrite(&book,sizeof(book),1,books);
		}

		fclose(log);
		fclose(books);
	}
	return 0;
}
