all:query borrow query_borrow return
.PHONY:all

query:library.h
	cc -o query query.c
borrow:library.h
	cc -o borrow borrow.c
query_borrow:library.h
	cc -o query_borrow query_borrow.c
return:library.h
	cc -o return return.c
