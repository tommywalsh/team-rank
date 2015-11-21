# team-rank
Ranks teams based on game results, using a Bradley-Terry algorithm

OVERVIEW:
Given a list of game results, this program will spit out a ranking of the teams involved.

HOW TO RUN:
First, you'll need a list of game results in the correct format. There needs to be one game per line, with each line 
looking something like this:

Some Team Name 7, Other Team Name 4

That is, each line will have a team name, then a space, then an integer score, then a comma, then a space, then 
another team name, then a space, then an integer score. Some examples are included in this project.

The program accepts this data on stdin, and will spit out a report to stdout.

If possible, all teams will be ranked together in the same list. Each team will have a rating number. The rating
numbers are based on a Bradley-Terry algorithm. This means that their ratios are what's important -- if Team A's
rating is two times greater than Team B's, then that means that Team A's results were twice as good as Team B's.
The rating is derived solely from game results, with wins being better than losses, and games against higher-rated
teams being better than games against lower-rated teams.

Sometimes, it is not possible to rank all teams together. In order for two teams to be ranked together in the same
list, there must be a string of wins or ties from each team to the other team. When it is not possible to rank every
team together, the program will break the teams up into a set of "pods", where each pod has its own rating system,
and the pods are partially ordered with respect to one another, as possible.

This project also includes a data scraper which will download college hockey scores. The output of this scraper can
be piped into the main program.

HOW TO BUILD:
Typing "make" should work, if your system is exactly like mine. This will compile all files, run all tests, and
link the main binary. If your system is not exactly like mine, you might have to tweak the makefile.
