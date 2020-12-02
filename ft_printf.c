#include "ft_printf.h"

void pf_initflag(t_flag **flag)
{
	(*flag)->negative = false;
	(*flag)->zero = false;
	(*flag)->minField = -1;
	(*flag)->vaDigit = -1;
	ft_bzero(&(*flag)->conversion, sizeof(char));
}

int pf_pack_minf(t_flag **flag)
{
	int *tmp;

	tmp = &(*flag)->minField;
	*tmp = 0;
	while (*(*flag)->fmt && *(*flag)->fmt >= '0' && *(*flag)->fmt <= '9')
	{
		*tmp *= 10;
		*tmp += *(*flag)->fmt - '0';
		((*flag)->fmt)++;
	}
	if (!*(*flag)->fmt)
		return (0);
	return (1);
}

int pf_pack_vad(t_flag **flag)
{
	int *tmp;

	if (*(*flag)->fmt == '*')
		return (1);
	tmp = &(*flag)->vaDigit;
	*tmp = 0;
	((*flag)->fmt)++;
	if (!*(*flag)->fmt)
		return (0);
	if (!(*(*flag)->fmt >= '0' && *(*flag)->fmt <= '9'))
	{
		((*flag)->fmt)--;
		return (1);
	}
	while (*(*flag)->fmt >= '0' && *(*flag)->fmt <= '9')
	{
		*tmp *= 10;
		*tmp += *(*flag)->fmt - '0';
		((*flag)->fmt)++;
	}
	if (!*(*flag)->fmt)
		return (0);
	return (1);
}

void pf_pack_asta(t_flag **flag)
{
	if (*(*flag)->fmt == '.')
	{
		((*flag)->fmt)++;
		if (*(*flag)->fmt && *(*flag)->fmt == '*')
			(*flag)->vaDigit = va_arg((*flag)->ap, int);
		else
			((*flag)->fmt)--;
	}
	else
	{
		(*flag)->minField = va_arg((*flag)->ap, int);
		if ((*flag)->minField < 0)
		{
			(*flag)->negative = true;
			(*flag)->minField *= -1;
		}
	}
}

void pf_pack_flag(t_flag **flag)
{
	(*flag)->fmt++;
	while (*(*flag)->fmt)
	{
		if (*(*flag)->fmt == '-')
			(*flag)->negative = true;
		else if (*(*flag)->fmt == '0')
			(*flag)->zero = true;
		else if (*(*flag)->fmt == '*')
			pf_pack_asta(flag);
		else if (*(*flag)->fmt >= '1' && *(*flag)->fmt <= '9')
			if (!(pf_pack_minf(flag)))
				break ;
		if (*(*flag)->fmt == '.')
		{
			pf_pack_asta(flag);
			if (!(pf_pack_vad(flag)))
				break ;
		}
		if (*(*flag)->fmt == 'c' || *(*flag)->fmt == 's' || *(*flag)->fmt == 'p' || *(*flag)->fmt == 'd' ||
			*(*flag)->fmt == 'i' || *(*flag)->fmt == 'u' || *(*flag)->fmt == 'x' || *(*flag)->fmt == 'X' || *(*flag)->fmt == '%')
		{
			(*flag)->conversion = *(*flag)->fmt;
			((*flag)->fmt)++;
			break ;
		}
		((*flag)->fmt)++;
	}
}

void pf_print_onechar(t_flag **flag, int c)
{
	if ((*flag)->minField > 1)
	{
		if ((*flag)->negative)
		{
			(*flag)->ret += write(1, &c, 1);
			while (--((*flag)->minField))
				(*flag)->ret += write(1, " ", 1);
		}
		else if ((*flag)->zero)
		{
			while (--((*flag)->minField))
				(*flag)->ret += write(1, "0", 1);
			(*flag)->ret += write(1, &c, 1);
		}
		else
		{
			while (--((*flag)->minField))
				(*flag)->ret += write(1, " ", 1);
			(*flag)->ret += write(1, &c, 1);
		}
	}
	else
		(*flag)->ret += write(1, &c, 1);
}

void pf_print_char(t_flag **flag)
{
	int c;

	c = va_arg((*flag)->ap, int);
	pf_print_onechar(flag, c);
}

void pf_print_str_sub(t_flag **flag, char *str)
{
	if ((*flag)->minField > (*flag)->vaDigit)
	{
		if ((*flag)->negative)
		{
			(*flag)->ret += write(1, str, (*flag)->vaDigit);
			while (((*flag)->minField)-- > (*flag)->vaDigit)
				(*flag)->ret += write(1, " ", 1);
		}
		else if ((*flag)->zero)
		{
			while (((*flag)->minField)-- > (*flag)->vaDigit)
				(*flag)->ret += write(1, "0", 1);
			(*flag)->ret += write(1, str, (*flag)->vaDigit);
		}
		else
		{
			while (((*flag)->minField)-- > (*flag)->vaDigit)
				(*flag)->ret += write(1, " ", 1);
			(*flag)->ret += write(1, str, (*flag)->vaDigit);
		}
	}
	else
		(*flag)->ret += write(1, str, (*flag)->vaDigit);
}

void pf_print_str(t_flag **flag)
{
	char *str;
	int min;

	str = va_arg((*flag)->ap, char *);
	if ((*flag)->vaDigit == -1 || (*flag)->vaDigit > ft_strlen(str))
		(*flag)->vaDigit = ft_strlen(str);
	pf_print_str_sub(flag, str);
}

int pf_check_digit(int num)
{
	int dret;

	dret = 0;
	if (num > 0)
	{
		while (num > 1)
		{
			num /= 10;
			dret++;
		}
	}
	else
	{
		while (num < -1)
		{
			num /= 10;
			dret ++;
		}
	}
	return (dret);
}

void pf_print_zero(t_flag **flag, int digit)
{
	int counter;

	counter = 0;
	while (counter++ < (*flag)->vaDigit - digit)
		(*flag)->ret += write(1, "0", 1);
}

void pf_print_space(t_flag **flag)
{
	int counter;

	counter = 0;
	while (counter++ < (*flag)->minField - (*flag)->vaDigit)
		(*flag)->ret += write(1, " ", 1);
}

void pf_print_num_pos(t_flag **flag, int digit, int num)
{
	if ((*flag)->negative)
	{
		pf_print_zero(flag, digit);
		ft_putnbr(num, flag);
		pf_print_space(flag);
	}
	else if ((*flag)->zero)
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putnbr(num, flag);
	}
	else
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putnbr(num, flag);
	}
}

void pf_print_num_neg(t_flag **flag, int digit, int num)
{
	((*flag)->minField)--;
	if ((*flag)->negative)
	{
		(*flag)->ret += write(1, "-", 1);
		pf_print_zero(flag, digit);
		ft_putnbr(-num, flag);
		pf_print_space(flag);
	}
	else if ((*flag)->zero)
	{
		pf_print_space(flag);
		(*flag)->ret += write(1, "-", 1);
		pf_print_zero(flag, digit);
		ft_putnbr(-num, flag);
	}
	else
	{
		pf_print_space(flag);
		(*flag)->ret += write(1, "-", 1);
		pf_print_zero(flag, digit);
		ft_putnbr(-num, flag);
	}
}

void pf_print_num(t_flag **flag)
{
	int num;
	int digit;

	num = va_arg((*flag)->ap, int);
	digit = pf_check_digit(num);
	if ((*flag)->minField < (*flag)->vaDigit)
		(*flag)->minField = (*flag)->vaDigit;
	if ((*flag)->vaDigit < digit)
		(*flag)->vaDigit = digit;
	if ((*flag)->minField > digit)
	{
		if (num >= 0)
			pf_print_num_pos(flag, digit, num);
		else
			pf_print_num_neg(flag, digit, num);
	}
	else
	{
		ft_putnbr(num, flag);
	}
}

int pf_check_usdigit(unsigned int num)
{
	int dret;

	dret = 0;
	while (num >= 1)
	{
		num /= 10;
		dret++;
	}
	return (dret);
}

void pf_print_usnum_pos(t_flag **flag, int digit, unsigned int num)
{
	if ((*flag)->negative)
	{
		pf_print_zero(flag, digit);
		ft_putusnbr(num, flag);
		pf_print_space(flag);
	}
	else if ((*flag)->zero)
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putusnbr(num, flag);
	}
	else
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putusnbr(num, flag);
	}
}

void pf_print_usnum(t_flag **flag)
{
	unsigned int num;
	int digit;

	num = va_arg((*flag)->ap, unsigned int);
	digit = pf_check_usdigit(num);
	if ((*flag)->minField < (*flag)->vaDigit)
		(*flag)->minField = (*flag)->vaDigit;
	if ((*flag)->vaDigit < digit)
		(*flag)->vaDigit = digit;
	if ((*flag)->minField > digit)
		pf_print_usnum_pos(flag, digit, num);
	else
		ft_putusnbr(num, flag);
}

int pf_check_xdigit(unsigned int num)
{
	int count;

	count = 0;
	while (num >= 1)
	{
		num /= 16;
		count++;
	}
	return (count);
}

void pf_print_xnum_sub(t_flag **flag, int digit, unsigned int num)
{
	if ((*flag)->negative)
	{
		pf_print_zero(flag, digit);
		ft_putxnbr(num, flag);
		pf_print_space(flag);
	}
	else if ((*flag)->zero)
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putxnbr(num, flag);
	}
	else
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putxnbr(num, flag);
	}
}

void pf_print_xnum(t_flag **flag)
{
	unsigned int num;
	int digit;

	num = va_arg((*flag)->ap, unsigned int);
	digit = pf_check_xdigit(num);

	if ((*flag)->minField < (*flag)->vaDigit)
		(*flag)->minField = (*flag)->vaDigit;
	if ((*flag)->vaDigit < digit)
		(*flag)->vaDigit = digit;
	if ((*flag)->minField > digit)
		pf_print_xnum_sub(flag, digit, num);
	else
		ft_putxnbr(num, flag);
}

void pf_print_lxnum_sub(t_flag **flag, int digit, unsigned int num)
{
	if ((*flag)->negative)
	{
		pf_print_zero(flag, digit);
		ft_putlxnbr(num, flag);
		pf_print_space(flag);
	}
	else if ((*flag)->zero)
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putlxnbr(num, flag);
	}
	else
	{
		pf_print_space(flag);
		pf_print_zero(flag, digit);
		ft_putlxnbr(num, flag);
	}
}

void pf_print_lxnum(t_flag **flag)
{
	unsigned int num;
	int digit;

	num = va_arg((*flag)->ap, unsigned int);
	digit = pf_check_xdigit(num);

	if ((*flag)->minField < (*flag)->vaDigit)
		(*flag)->minField = (*flag)->vaDigit;
	if ((*flag)->vaDigit < digit)
		(*flag)->vaDigit = digit;
	if ((*flag)->minField > digit)
		pf_print_lxnum_sub(flag, digit, num);
	else
		ft_putlxnbr(num, flag);
}

int pf_check_adddigit(uintptr_t address)
{
	int count;

	count = 0;
	while (address >= 1)
	{
		address /= 16;
		count++;
	}
	return (count);
}

void pf_print_add(t_flag **flag, int digit, uintptr_t address)
{
	(*flag)->minField -= 2;
	if ((*flag)->negative)
	{
		(*flag)->ret += write(1, "0x", 2);
		pf_print_zero(flag, digit);
		ft_putadnbr(address, flag);
		pf_print_space(flag);
	}
	else if ((*flag)->zero)
	{
		pf_print_space(flag);
		(*flag)->ret += write(1, "0x", 2);
		pf_print_zero(flag, digit);
		ft_putadnbr(address, flag);
	}
	else
	{
		pf_print_space(flag);
		(*flag)->ret += write(1, "0x", 2);
		pf_print_zero(flag, digit);
		ft_putadnbr(address, flag);
	}
}

void pf_print_address(t_flag **flag)
{
	uintptr_t address;
	int digit;

	address = (uintptr_t)va_arg((*flag)->ap, void *);
	digit = pf_check_adddigit(address);
	if ((*flag)->minField < (*flag)->vaDigit)
		(*flag)->minField = (*flag)->vaDigit;
	if ((*flag)->vaDigit < digit)
		(*flag)->vaDigit = digit;// + 1;
	if ((*flag)->minField > digit)
		pf_print_add(flag, digit, address);
	else
	{
		(*flag)->ret += write(1, "0x", 2);
		ft_putadnbr(address, flag);
	}
}

void pf_print_percent(t_flag **flag)
{
	pf_print_onechar(flag, '%');
}

int pf_switch(t_flag **flag)
{
	pf_initflag(flag);
	pf_pack_flag(flag);
//	printf("\nneg:%d\nzero:%d\nminField:%d\nvaDigit:%d\nconversion:%c\n", (*flag).negative, (*flag).zero, (*flag).minField, (*flag).vaDigit, (*flag).conversion);
	if ((*flag)->conversion == 'c')
		pf_print_char(flag);
	else if((*flag)->conversion == 's')
		pf_print_str(flag);
	else if((*flag)->conversion == 'p')
		pf_print_address(flag);
	else if((*flag)->conversion == 'd' || (*flag)->conversion == 'i')
		pf_print_num(flag);
	else if((*flag)->conversion == 'u')
		pf_print_usnum(flag);
	else if((*flag)->conversion == 'x')
		pf_print_xnum(flag);
	else if((*flag)->conversion == 'X')
		pf_print_lxnum(flag);
	else if((*flag)->conversion == '%')
		pf_print_percent(flag);
/*
	else
		pf_print_erroract(&flag);
*/
	return (1);
}

int ft_error_free_flag(t_flag **flag)
{
	if (flag)
	{
		free(*flag);
		*flag = NULL;
	}
	return (-1);
}

void ft_last_free_flag(t_flag **flag)
{
	if (flag)
	{
		free(*flag);
		*flag = NULL;
	}
}

int ft_printf(const char *fmt, ...)
{
	t_flag *flag;
	int ret;

	if (!(flag = malloc(sizeof(t_flag))))
		return (-1);
	flag->ret = 0;
	if (!fmt)
		return (ft_error_free_flag(&flag));
	va_start(flag->ap, fmt);
	flag->fmt = fmt;
	while (*flag->fmt)
		if (*flag->fmt != '%')
			flag->ret += write(1, flag->fmt++, 1);
		else if (!(pf_switch(&flag)))
				return (-1);
	va_end(flag->ap);
	ret = flag->ret;
	ft_last_free_flag(&flag);
	return (ret);
}

int main()
{
	int d = -777;
	int i = 666;
	int count= 0;
	unsigned int u = 555;
	unsigned int x = 444;
	unsigned int X = 333;
	char *str = "aaa";
	char c = 'a';

//---------\0--------------
	printf("\n-------------null-------------\n");
	count = ft_printf("%s", "\0");
	printf("\n%d", count);
	count = printf("%s", "\0");
	printf("\n%d", count);

//---------asta------------
	printf("\n-------------asta-------------\n");
	count = ft_printf("[%10.5d]", i);
	count = ft_printf("[%*.*d]", 10, 5, i);
	printf("\n%d\n", count);
	count = printf("[%10.5d]", i);
	count = printf("[%*.*d]", 10, 5, i);
	printf("\n%d\n", count);

	count = ft_printf("[%-10.5d]", i);
	count = ft_printf("[%*.*d]", -10, 5, i);
	printf("\n%d\n", count);
	count = printf("[%-10.5d]", i);
	count = printf("[%*.*d]", -10, 5, i);
	printf("\n%d\n", count);

	count = ft_printf("[%10.-5d]", i);
	count = ft_printf("[%*.*d]", 10, -5, i);
	printf("\n%d\n", count);
	count = printf("[%10.-5d]", i);
	count = printf("[%*.*d]", 10, -5, i);
	printf("\n%d\n", count);

	count = ft_printf("[%10.5s]", str);
	count = ft_printf("[%*.*s]", 10, 5, str);
	printf("\n%d\n", count);
	count = printf("[%10.5s]", str);
	count = printf("[%*.*s]", 10, 5, str);
	printf("\n%d\n", count);
//---------error-----------
	count = ft_printf(NULL);
	ft_printf("%d\n", count);
//---------%d-----------
	count = printf("[%d]", d);
	printf("-->%d\n", count);
	count = printf("[%d]", d);
	printf("-->%d\n", count);
	count = ft_printf("[%2.10d]", d);
	printf("-->%d\n", count);
	count = printf("[%2.10d]", d);
	printf("-->%d\n", count);
	count = ft_printf("[%10.1d]", d);
	printf("-->%d\n", count);
	count = printf("[%10.1d]", d);
	printf("-->%d\n", count);
//-----------%p-------------
	printf("\n-------------p-------------\n");
	count = ft_printf("[%20.15p]", str);
	printf("-->%d\n", count);
	count = printf("[%20.15p]", str);
	printf("-->%d\n", count);
	count = ft_printf("[%2.15p]", str);
	printf("-->%d\n", count);
	count = printf("[%2.15p]", str);
	printf("-->%d\n", count);
	count = ft_printf("[%20.3p]", str);
	printf("-->%d\n", count);
	count = printf("[%20.3p]", str);
	printf("-->%d\n", count);

//-----------%u-------------
	printf("\n-------------u-------------\n");
	count = ft_printf("[%10.5d]", u);
	printf("-->%u\n", count);
	count = printf("[%10.5d]", u);
	printf("-->%u\n", count);
	count = ft_printf("[%2.10d]", u);
	printf("-->%u\n", count);
	count = printf("[%2.10d]", u);
	printf("-->%u\n", count);
	count = ft_printf("[%10.1d]", u);
	printf("-->%u\n", count);
	count = printf("[%10.1d]", u);
	printf("-->%u\n", count);
//-----------%x-------------
	printf("\n-------------x-------------\n");
	count = ft_printf("[%10.5x]", x);
	printf("-->%d\n", count);
	count = printf("[%10.5x]", x);
	printf("-->%d\n", count);
	count = ft_printf("[%2.10x]", x);
	printf("-->%d\n", count);
	count = printf("[%2.10x]", x);
	printf("-->%d\n", count);
	count = ft_printf("[%10.1x]", x);
	printf("-->%d\n", count);
	count = printf("[%10.1x]", x);
	printf("-->%d\n", count);
//-----------%X-------------
	printf("\n-------------X-------------\n");
	count = ft_printf("[%10.5X]", X);
	printf("-->%d\n", count);
	count = printf("[%10.5X]", X);
	printf("-->%d\n", count);
	count = ft_printf("[%2.10X]", X);
	printf("-->%d\n", count);
	count = printf("[%2.10X]", X);
	printf("-->%d\n", count);
	count = ft_printf("[%10.1X]", X);
	printf("-->%d\n", count);
	count = printf("[%10.1X]", X);
	printf("-->%d\n", count);
//-----------%s-------------
	printf("\n-------------s-------------\n");
	count = ft_printf("[%20.15s]", str);
	printf("-->%d\n", count);
	count = printf("[%20.15s]", str);
	printf("-->%d\n", count);
	count = ft_printf("[%2.15s]", str);
	printf("-->%d\n", count);
	count = printf("[%2.15s]", str);
	printf("-->%d\n", count);
	count = ft_printf("[%20.3s]", str);
	printf("-->%d\n", count);
	count = printf("[%20.3s]", str);
	printf("-->%d\n", count);
//-----------%c-------------
	printf("\n-------------c-------------\n");
	count = ft_printf("[%20.15c]", c);
	printf("-->%d\n", count);
	count = printf("[%20.15c]", c);
	printf("-->%d\n", count);
	count = ft_printf("[%2.15c]", c);
	printf("-->%d\n", count);
	count = printf("[%2.15c]", c);
	printf("-->%d\n", count);
	count = ft_printf("[%20.3c]", c);
	printf("-->%d\n", count);
	count = printf("[%20.3c]", c);
	printf("-->%d\n", count);
//-----------%%-------------
	printf("\n-------------percent-------------\n");
	count = ft_printf("[%20.15%]");
	printf("-->%d\n", count);
	count = printf("[%20.15%]");
	printf("-->%d\n", count);
	count = ft_printf("[%2.15%]");
	printf("-->%d\n", count);
	count = printf("[%2.15%]");
	printf("-->%d\n", count);
	count = ft_printf("[%20.3%]");
	printf("-->%d\n", count);
	count = printf("[%20.3%]");
	printf("-->%d\n", count);
}
