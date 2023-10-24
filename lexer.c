#include "parser.h"

t_lexer *lexer_node(t_lexer *prev)
{
    t_lexer *node;
    
    node = (t_lexer *)malloc(sizeof(t_lexer));
    if (node == NULL)
        return (NULL);
    node->str = NULL;
    node->key = 0;
    node->prev = prev;
    node->next = NULL;
    if (!prev)
        node->index = 0;
    else
        node->index = prev->index + 1;
    return (node);
}

void print_lex(t_lexer *lexer)
{
    ft_printf("index: %d\n", lexer->index);
    if (lexer->str)
        ft_printf("word: %s\n", lexer->str);
    if (lexer->key)
    {
        if (lexer->key == l_pipe)
            ft_printf("key: pipe\n");
        if (lexer->key == l_space)
            ft_printf("key: space\n");
        else
            ft_printf("key: %u\n", lexer->key);
    }
    write(1, "\n", 1);
    return ;
}

void    free_array(char **str)
{
    int i;

    i = -1;
    while (str[++i])
        free(str[i]);
    return ;
}

t_lexertype get_key(char *str, int i)
{
    if (str[i] == '<')
    {
        if (str[i + 1] == '<')
            return (l_append);
        else
            return (l_in);
    }
    if (str[i] == '>')
        return (l_out);
    if (str[i] == '|')
        return (l_pipe);
    if (str[i] == '"')
        return (l_dquote);
    if (str[i] == 39)
        return (l_quote);
    if (str[i] == ' ')
        return (l_space);
    return (0);
}

int if_quotes(char *str, int i)
{
    int tmp;

    tmp = 0;
    if (str[i] == 34 || str[i] == 39)
    {
        tmp = str[i];
        i++;
        while (str[i] != tmp)
            i++;
        i++;
    }
    return (i);
}

int    parse_lexlst(t_lexer *lexer, char *str)
{
    int i;
    int start;

    i = 0;
    start = 0;
    while (str[i])
    {
        i = if_quotes(str, i);
        if (get_key(str, i) != 0)
        {
            //if (get_key(str, i - 1) == 0)
            //{}
            lexer->str = ft_substr(str, start, i - start);
            print_lex(lexer);
            lexer->next = lexer_node(lexer);
            lexer = lexer->next;
            if (!lexer)
            {
                free_lexer(lexer);
                free(str);
                return (-1);
            }
            while (get_key(str, i) == l_space)
                i++;
            start = i;
        }
        i++;
    }
    lexer->str = ft_substr(str, start, i - start);
    print_lex(lexer);
    if (!lexer)
    {
        free_lexer(lexer);
        free(str);
        return (-1);
    }
    return (1);
}


t_lexer	*ft_lexer(char *str)
{
    t_lexer     *lexer;
    t_lexer     *head;
    int           i;
    int           start;

    i = 0;
    start = 0;
    lexer = lexer_node(NULL);
    if (!lexer)
    {
        free(str);
        return (NULL);
    }
    head = lexer;
    if (!parse_lexlst(lexer, str))
        return (NULL);
    ft_printf("lexer has %d nodes\n", lexer->index + 1);
    free(str);
    return (head);
}
