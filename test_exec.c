#include "parser.h"

t_simple_cmds *create_simple_cmds(char **str, char *in, char *out, char **envp, int index, int amount_of_cmds) {
    t_simple_cmds *cmd = malloc(sizeof(t_simple_cmds));
    if (cmd == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Allocate memory and copy the string array
    int num_strings = 0;
    while (str[num_strings] != NULL) {
        num_strings++;
    }

    cmd->str = malloc((num_strings + 1) * sizeof(char *));
    if (cmd->str == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_strings; i++) {
        cmd->str[i] = strdup(str[i]);
        if (cmd->str[i] == NULL) {
            perror("strdup");
            exit(EXIT_FAILURE);
        }
    }
    cmd->str[num_strings] = NULL;

    // Allocate memory and copy the 'in' and 'out' strings
    cmd->in = (in != NULL) ? strdup(in) : NULL;
    if (in != NULL && cmd->in == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    cmd->out = (out != NULL) ? strdup(out) : NULL;
    if (out != NULL && cmd->out == NULL) {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    // Allocate memory and copy the environment variables array

    cmd->env = envp;
    cmd->index = index;
    cmd->amount_of_cmds = amount_of_cmds;
    cmd->next = NULL;
    cmd->prev = NULL;

    return cmd;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
    // Create the first node
    char *str1[] = {"ls", "-l", NULL};
    char *in1 = NULL;
    char *out1 = NULL;
    //char *env1[] = {envp[0], envp[1], NULL}; // Copy only two environment variables for example
    t_simple_cmds *node1 = create_simple_cmds(str1, in1, out1, envp, 0, 2);

    // Create the second node
    char *str2[] = {"sort", "-r", NULL};
    char *in2 = NULL;
    char *out2 = ft_strdup("out.txt");
   // char *env2[] = {envp[2], envp[3], NULL}; // Copy only two environment variables for example
    t_simple_cmds *node2 = create_simple_cmds(str2, in2, out2, envp, 1, 2);

    // Link the nodes
    node1->next = node2;
    node2->prev = node1;

    // Now you have two nodes with the specified data
    // You can access the data like node1->str[0], node1->str[1], etc.

    // Don't forget to free the allocated memory when you're done using it
	ft_printf("node 1 has input/output %s/%s\n", node1->in, node1->out);
	ft_printf("node 2 has input/output %s/%s\n", node2->in, node2->out);
	executor(node1);
    free(node1);
    free(node2);
    return 0;
}