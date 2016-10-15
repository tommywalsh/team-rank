import urllib2
import re
import sys

def retrieve_full_page(year):
    url = "http://collegehockeyinc.com/stats/compnatfull{0}.php".format(year[-2:])
    response = urllib2.urlopen(url)
    return response.read()

def process_page(year, page):
    state = 'prequel'
    season = "{0}-{1}".format(int(year)-1, year)
    for line in page.splitlines():
        if (state == 'prequel'):
            if re.search(season, line):
                state = 'awaiting date'
        elif re.search('Exhibition', line):
                state = 'awaiting date'
        elif (state == 'awaiting date'):
            if re.search('\d\d/\d\d/\d\d', line):
                state = 'awaiting away team'
        elif (state == 'awaiting away team'):
            m = re.search('"left">([^<]+)<', line)
            awayTeam = m.group(1)
            state = 'awaiting away score'
        elif (state == 'awaiting away score'):
            # For some reason, away score and home team are on the same line!
            m = re.search('(\d+).*?(at|vs) ([^<]+)<', line)
            if m:
                awayScore = m.group(1)
                homeTeam = m.group(3)
                state = 'awaiting home score'
            else:
                # Game is not completed
                state = 'awaiting date'
        elif (state == 'awaiting home score'):
            m = re.search('\d+', line)
            homeScore= m.group()
            state = 'awaiting notes'
        elif (state == 'awaiting notes'):
            if re.search('/tr', line):
                print homeTeam + " " + homeScore + ", " + awayTeam + " " + awayScore
                state = 'awaiting date'

if len(sys.argv) != 2:
    print "Usage: %s year".format(sys.argv[0])
    print "where 'year' is the ending year of the season. e.g. say '2016' for the 2015-2016 season"
else:
    year = sys.argv[1]
    process_page(year, retrieve_full_page(year))
