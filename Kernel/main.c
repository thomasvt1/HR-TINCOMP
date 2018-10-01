#include <stddef.h>
#include <stdint.h>
#include "uart.h"

static void *_end = (void *) 0x400000;

struct block
{
    void *addr;
    size_t size;
	struct block* next;
};

static struct block* head = NULL;

void *malloc(size_t size)
{
	struct block* bp = head;
	if(head == NULL) {
		head = bp = _end;
		bp->addr = sizeof(struct block) + _end;
	} else {
		for(bp = head; bp->next != NULL; bp = bp->next);
		bp->next = bp->addr + bp->size;
		bp = bp->next;
		bp->addr = bp + sizeof(struct block);
	}
	bp->size = size;
	bp->next = NULL;
	return bp->addr;
}


void free(void *ptr)
{
	struct block* bp = head;
	if (bp->next == NULL)
	{
		if (bp->addr == ptr)
			bp = NULL;
	}
	else
	{
		for (bp = head; bp->addr != ptr; bp = bp->next);
		if (bp->next->addr == ptr)
		{
			struct block* del = bp->next;
			if (del->next != NULL)
			{
				bp->next = bp->next->next;
			}
			del = NULL;
		}
	}
}


char *strcpy(char *dest, const char *src)
{
	char *d = dest;
	const char *s = src;

	while ((*d++ = *s++));

	return dest;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];

	while (i < n)
		dest[i++] = '\0';

	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	if (d == s)
		return dest;

	if (s > d) {
		while (n--)
			*d++ = *s++;
	} else {
		d += n - 1;
		s += n - 1;

		while (n--)
			*d-- = *s--;
	}

	return dest;
}


//EXTRA FUNCTIE!!!!
int strcmp(const char *src1, const char *src2)
{
	while (*src1 == *src2 && *src1) {
		src1++;
		src2++;
	}

	return *(unsigned char *)src1 - *(unsigned char *)src2;
}



void main()
{
	char *str, buf[128];
	int i;
	
	uart_init();

	char *abc;
	if (!(abc = malloc(69)))
		uart_puts("out of memory\n");
	strcpy(abc, "~ Welcome to Thomas Kernel ~\n");
	uart_puts(abc);

	for (;;) {
		uart_puts("> ");
		i = 0;
		while ((buf[i] = uart_getc())) {
			if (buf[i] == '\n' || buf[i] == '\r')
				break;
			else
				uart_putc(buf[i++]);
		}
		buf[i] = '\0';
		uart_putc('\n');

		if (!(str = malloc(13)))
			uart_putc('s');

		for (i = 0; buf[i]; i++)
			str[i] = buf[i];

		if (strcmp(str, "help") == 0)
		{
			uart_puts("----- HELP MENU -----\n");
			uart_puts("ping localhost - pings localhost\n");
		}
		else if (strcmp(str, "ping localhost") == 0)
		{
			uart_puts("Pinging LAPTOP-THOMAS [::1] with 32 bytes of data:");
			for (int i=0; i < 3; i++)
				uart_puts("Reply from ::1: time<1ms\n");
		}
		else
		{
			uart_puts("Command unknown");
		}

		//uart_puts(str);
		uart_putc('\n');
		free(str);
	}
}
