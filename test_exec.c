#include "executor.h"

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
        printf("Index: %d, Amount of Commands: %d\n", current->index, current->amount_of_cmds);
        print_redir_list(current->redir); // Print redir list for each t_simple_cmds node
        current = current->next;
    }
}

int main(int ac, char **av, char **envp) {
    t_redir **redir_list = NULL;
    t_simple_cmds *cmds_list = NULL;

    (void)ac;
    redir_list = ft_calloc(sizeof(t_redir), 2);

    // Add sample redir nodes
    add_redir_node(&redir_list[0], l_in, "input.txt");

    // Add sample t_simple_cmds node
    char *str0[] = {av[1], av[2], NULL};
    add_simple_cmds_node(&cmds_list, str0, NULL, redir_list[0], envp, 0, 2);

    //add_redir_node(&redir_list[1], l_out, NULL);
    //add_redir_node(&redir_list[1], l_in, NULL);

    add_redir_node(&redir_list[1], l_append, "output.txt");

    char *str2[] = {av[5], av[6]};
    add_simple_cmds_node(&cmds_list, str2, NULL, redir_list[1], envp, 0, 2);

    // Print the lists for verification
    print_redir_list(*redir_list);
    print_simple_cmds_list(cmds_list);

    executor(cmds_list);
    // Free the allocated memory
    int i = 0;
    while(redir_list[i])
    {
        free_redir_list(redir_list[i]);
        i++;
    }
    free_simple_cmds_list(cmds_list);

    return 0;
}