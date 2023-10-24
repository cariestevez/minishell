#include "parser.h"

int free_lexer(t_lexer *lexer)
{
    //t_lexer *head;
    t_lexer *tmp;

    while (lexer)
    {
        tmp = lexer->next;
        free(lexer);
        lexer = tmp;
    }
    return (0);
}

int minishell_loop()
{
    char        *str;
    t_lexer         *lexer;
    const char    *prompt;

    prompt = "pRoMpT hErE ~(*__*)~> ";
    
    str = readline (prompt);
    lexer = ft_lexer(str);
    printf("lexer returned\n");
    free_lexer(lexer);
    return (0);

}

int main()
{
    while (1)
        minishell_loop();
}