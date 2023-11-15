# minishell

write and introduction to the project here eventually

Bash manual:
https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html

########################################
unresolved problems + questions
########################################
- in which order are the environment variables organized? 
- should we implement substring expansion?
- command substitution?
- filename expansion?
- in a path, where can/should the slash characters be?
-- I cannot cd /[dirname]/
-- I can cd [dirname]/
-- in envvar, HOME is /home/[user]/
-- when /cd ~/[dirname], will ~ be substituted with '/home/' or 'home/'

to do:
- create arr of local shell variables in t_shell struct
- write the export function -> copy xlocalvariable to environment
