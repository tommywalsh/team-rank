import urllib2
import re

url = "http://collegehockeyinc.com/stats/compnatfull16.php"
year = "2015-2016"

def retrieve_full_page():
    response = urllib2.urlopen(url)
    return response.read()

def process_page(page):
    state = 'prequel'

    for line in page.splitlines():
        if (state == 'prequel'):
            if re.search(year, line):
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
            if re.search('&nbsp;', line):
                state = 'awaiting date'
                if not re.search('Exhibition', line):
                    print homeTeam + " " + homeScore + ", " + awayTeam + " " + awayScore

process_page(retrieve_full_page())
