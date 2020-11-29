#ifndef FT_PRINTF_H
# define FT_PRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct	s_flag
{
	const char	*fmt;
	va_list	ap;
	int		ret;
	bool	negative;
	bool	zero;
	int		minField;
	int		vaDigit;
	char	conversion;
}				t_flag;

void	ft_bzero(void *s, size_t n);
void	ft_putchar(char c, t_flag **flag);
void	ft_putnbr(int n, t_flag **flag);
void	ft_putusnbr(unsigned int nbr, t_flag **flag);
void	ft_putxnbr(unsigned int nbr, t_flag **flag);
void	ft_putlxnbr(unsigned int nbr, t_flag **flag);
void	ft_putadnbr(uintptr_t nbr, t_flag **flag);
size_t	ft_strlen(const char *s);
void pf_initflag(t_flag **flag);
void pf_pack_flag(t_flag **flag);
int pf_switch(t_flag **flag);
int ft_printf(const char *fmt, ...);

#endif

