#include "minishell.h"

void add_redir_node(t_redir **head, t_lexertype type, char *str) {
    t_redir *new_node = malloc(sizeof(t_redir));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->type = type;
    new_node->str = strdup(str);
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        t_redir *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void add_simple_cmds_node(t_simple_cmds **head, char **str, int (*builtin)(struct s_simple_cmds *), t_redir *redir, char **env, int index, int amount_of_cmds) {
    t_simple_cmds *new_node = malloc(sizeof(t_simple_cmds));
    if (new_node == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    new_node->str = str;
    new_node->builtin = builtin;
    new_node->redir = redir;
    new_node->env = env;
    new_node->index = index;
    new_node->amount_of_cmds = amount_of_cmds;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
    } else {
        t_simple_cmds *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
        new_node->prev = current;
    }
}

void free_redir_list(t_redir *head) {
    while (head != NULL) {
        t_redir *temp = head;
        head = head->next;
        free(temp->str);
        free(temp);
    }
}

void free_simple_cmds_list(t_simple_cmds *head) {
    while (head != NULL) {
        t_simple_cmds *temp = head;
        head = head->next;
        free(temp->str);
        free_redir_list(temp->redir);
        free(temp->env);
        free(temp);
    }
}

void print_redir_list(t_redir *head) {
    t_redir *current = head;
    while (current != NULL) {
        printf("Type: %d, String: %s\n", current->type, current->str);
        current = current->next;
    }
}

void print_simple_cmds_list(t_simple_cmds *head) {
    t_simple_cmds *current = head;
    while (current != NULL) {
        ft_printf("Index: %d, Amount of Commands: %d, ", current->index, current->amount_of_cmds);
        int i = -1;
        while (current->str[++i])
            ft_printf("String %d:%s ", i, current->str[i]);
        write(1, "\n", 1);
        print_redir_list(current->redir); // Print redir list for each t_simple_cmds node
        write(1, "\n", 1);
        current = current->next;
    }
}

char    **arrdup(char **env)
{
    int     i;
    char    **ret;
    i = 0;
    while (env[i])
        i++;
    ret = ft_calloc(sizeof(char **), i + 2);
    if (!ret)
        return (NULL);
    i = 0;
    while (env[i] != NULL)
    {
        ret[i] = ft_strdup(env[i]);
        if (ret[i] == NULL)
            return (NULL);
        i++;
    }
    ret[i] = NULL;
    return (ret);
}

int main(int ac, char **av, char **envp) {
    t_redir **redir_list = NULL;
    t_simple_cmds *cmds_list = NULL;
    char    **env = arrdup(envp);
    char    *str0[3];
    char    *str1[3];

    (void)ac;
    redir_list = ft_calloc(sizeof(t_redir), 3);

    // Add sample redir nodes
    add_redir_node(&redir_list[0], l_in, "input.txt");

    // Add sample t_simple_cmds node
    str0[0] = ft_strdup(av[1]);
    str0[1] = ft_strdup(av[2]);
    str0[2] = NULL;
    add_simple_cmds_node(&cmds_list, str0, NULL, redir_list[0], env, 0, 2);
 
    //add another redir
    add_redir_node(&redir_list[1], l_append, "a.txt");
    add_redir_node(&redir_list[1], l_append, "b.txt");
    //add another command
    
    str1[0] = ft_strdup(av[3]);
    str1[1] = ft_strdup(av[4]);
    str1[2] = NULL;
    add_simple_cmds_node(&cmds_list, str1, NULL, redir_list[1], env, 1, 2);

    // Print the lists for verification
    print_simple_cmds_list(cmds_list);
    ft_printf("error code: %d\n", executor(cmds_list));
    // Free the allocated memory
    /*
    i = 0;
    while(redir_list[i])
    {
        free_redir_list(redir_list[i]);
        i++;
    }
    free_simple_cmds_list(cmds_list);
    */
    return 0;
}