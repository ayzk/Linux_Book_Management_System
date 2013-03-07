Multi-user Book Management Systems

To solve the problem of atomic operation, I use the Tx file, it is just like a lock(but much faster than the unix system lock).
The first one(named A) to use "query_borrow" write his Id into the Tx file, if there are someone else want to borrow the same book with A before A has finished the progress, They will see A`s Id in Tx file and fail to borrow the book. Only when A have finished he will erase his ID in Tx file and from now others can borrow the book.

2012/10/27
