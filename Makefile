# _*_ Makefile _*_

producer_consumer: producer_consumer.o
	gcc producer_consumer.o -lpthread -o producer_consumer

producer_consumer.o: producer_consumer.c
	gcc -c producer_consumer.c
