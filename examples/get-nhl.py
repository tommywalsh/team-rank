import urllib2
import re
import sys

def retrieve_full_page(year):
    url = "http://www.hockey-reference.com/leagues/NHL_{0}_games.html".format(year)
    response = urllib2.urlopen(url)
    return response.read()

def pull_contents(cell):
    result = re.search('([^>]*)</.*$', cell)
    return result.group(1)

def process_line(line):
    cells = line.split("<td")
    team1 = pull_contents(cells[1])
    score1 = pull_contents(cells[2])
    team2 = pull_contents(cells[3])
    score2 = pull_contents(cells[4])
    if score1 and score2:
        print "{0} {1}, {2} {3}".format(team1, score1, team2, score2)

def process_page(page):
    state = 'prequel'
    for line in page.splitlines():
        if (state == 'prequel'):
            if re.search('sortable stats_table', line):
                state = 'awaiting table body'
        elif (state == 'awaiting table body'):
            if re.search('tbody', line):
                state = 'processing lines'
                process_line(line)
        elif (state == 'processing lines'):
            if not re.search('date_game', line):
                return
            else:
                process_line(line)

if len(sys.argv) != 2:
    print "Usage: %s year".format(sys.argv[0])
    print "where 'year' is the ending year of the season. e.g. say '2016' for the 2015-2016 season"
else:
    year = sys.argv[1]
    process_page(retrieve_full_page(year))
