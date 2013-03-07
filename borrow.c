#include<stdio.h>
#include<stdlib.h>
#include "library.h"
enum bool{false=0,true=!false};


int main(int argc, char* argv[])
{

	FILE *log=fopen("book.log","a");
	char str[50];

	char *p1,*p2;
	int bookId,userId;
	if (argc!=3 || (bookId=strtol(argv[1],&p1,10),userId=strtol(argv[2],&p2,10),p1==argv[1]||p2==argv[2]))
	{
		fputs("invalid borrow arguments\n",log);
		fclose(log);
		return 0;
	}

	FILE* books=fopen("books","r+b");
	fseek(books,0,SEEK_END);
	int size=ftell(books);

	if (userId<0)
	{
		sprintf(str,"invalid user ID %d\n",userId);
		fputs(str,log);
		fclose(log);
		fclose(books);
		return 0;
	}
	else if (bookId<0 || (bookId)*sizeof(struct BookInfo)>=size)
	{
		sprintf(str,"invalid book index %d\n",bookId);
		fputs(str,log);
		fclose(log);
		fclose(books);
		return 0;
	}
	else
	{
		FILE *tx=fopen("Tx","r+b");
		fseek(tx,sizeof(struct BookTx)*bookId,SEEK_SET);
		struct BookTx bookAtomic;
		fread(&bookAtomic,sizeof(bookAtomic),1,tx);
		if (bookAtomic.userID!=NO_USER)
		{
			sprintf(str,"%d failed to borrow %d\n",userId,bookId);
			fputs(str,log);
		}
		else
		{

		
			fseek(books,bookId*sizeof(struct BookInfo),SEEK_SET);
			struct BookInfo book;
			fread(&book,sizeof(book),1,books);
	
			enum bool already=book.userID_for_resv_request[0]==userId;
			int i;
			for (i=1;i<=book.number_of_resv_req;i++)
				if (book.userID_for_resv_request[i]==userId)
					already=true;
	
			if ((already && !(book.userID_for_resv_request[0]==NO_USER && book.userID_for_resv_request[1]==userId) )
				|| (!already && book.number_of_resv_req==15) )
			{
				sprintf(str,"%d couldn`t borrow %d\n",userId,bookId);
				fputs(str,log);
			}
			else if (book.userID_for_resv_request[0]==NO_USER
				 && (book.number_of_resv_req==0 || book.userID_for_resv_request[1]==userId))
			{
				sprintf(str,"%d borrowed %d\n",userId,bookId);
				fputs(str,log);
	
				book.userID_for_resv_request[0]=userId;
				if (book.number_of_resv_req!=0)
				{
					int i;
					for (i=1;i<book.number_of_resv_req;i++)
						book.userID_for_resv_request[i]=book.userID_for_resv_request[i+1];
					book.number_of_resv_req--;
				}
				fseek(books,bookId*sizeof(struct BookInfo),SEEK_SET);
				fwrite(&book,sizeof(book),1,books);
			}
			else
			{
				sprintf(str,"%d reserved %d\n",userId,bookId);
				fputs(str,log);
	
				book.userID_for_resv_request[++book.number_of_resv_req]=userId;
				fseek(books,bookId*sizeof(struct BookInfo),SEEK_SET);
				fwrite(&book,sizeof(book),1,books);
			}
		}
		fclose(tx);
		fclose(log);
		fclose(books);
	}
	return 0;
}
