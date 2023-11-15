int	ft_echo(t_shell *shell)
{
	char	*display;

	display = expander(shell->cmds->str[1]);
	if (display < 0)
		return (-1);
	ft_pustr_fd(display, 1);
	if (ft_strncmp(shell->cmds->str[2], "-n") != 0)
		write(1, "\n", 1);
	return (0);
}