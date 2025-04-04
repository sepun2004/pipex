/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_unsinged.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sepun <sepun@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 15:57:58 by sepun             #+#    #+#             */
/*   Updated: 2024/11/16 16:15:49 by sepun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int	ft_putnbr_unsinged(unsigned int n)
{
	int	count;

	count = 0;
	if (n > 0 && n <= 9)
	{
		count += ft_putnbr(n);
	}
	else if (n > 9)
	{
		count += ft_putnbr_unsinged(n / 10);
		count += ft_putnbr_unsinged(n % 10);
	}
	return (count);
}
