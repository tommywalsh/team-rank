from urllib.request import urlopen
import re
import sys

def retrieve_full_page(year):
    url = "http://collegehockeyinc.com/stats/compnatfull{0}.php".format(year[-2:])
    response = urlopen(url)
    return response.read().decode("utf-8")

def process_page(year, page):
    state = 'prequel'
    season = "{0}-{1}".format(int(year)-1, year[-2:])
    for line in page.splitlines():
        if (state == 'prequel'):
            if re.search(season, line):
                state = 'awaiting date'
        elif "Exhibition" in line or 'class="empty"' in line:
                state = 'awaiting date'
        elif (state == 'awaiting date'):
            if line.startswith("<tr") and "stats-section" in line:
                state = 'awaiting away team'
        elif (state == 'awaiting away team'):
            if "nowrap" in line and "class" not in line:
                items = line.split(">")
                items2 = items[1].split("&")
                awayTeam = items2[0]
                state = 'awaiting away score'
        elif (state == 'awaiting away score'):
            if "center" in line:
                items = line.split(">")
                items2 = items[1].split("<")
                if items2[0]:
                    awayScore = items2[0]
                    state = "awaiting home team"
                else:
                    # Game is not completed
                    state = 'awaiting date'
        elif (state == 'awaiting home team'):
            if "nowrap" in line:
                items = line.split(">")
                items2 = items[1].split("&")
                homeTeam = items2[0]
                state = 'awaiting home score'
        elif (state == 'awaiting home score'):
            if "center" in line:
                items = line.split(">")
                items2 = items[1].split("<")
                homeScore = items2[0]
                print(homeTeam + " " + homeScore + ", " + awayTeam + " " + awayScore)
                state = "awaiting away team"

if len(sys.argv) != 2:
    print("Usage: %s year".format(sys.argv[0]))
    print("where 'year' is the ending year of the season. e.g. say '2016' for the 2015-2016 season")
else:
    year = sys.argv[1]
    process_page(year, retrieve_full_page(year))
