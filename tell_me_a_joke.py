#!/usr/bin/env python

# Get a joke from /r/jokes with single command!
# Requirements: Python2
# license: WTFPL
# Author: Ankush Menat github://menatankush

import json
from random import randint
import urllib2

# You can change this with your faviorite sub
sub = "jokes"

json_loc = "https://www.reddit.com/r/" + sub + ".json"

# Reddit doesn't like python's default user agent.
request = urllib2.Request(json_loc, headers={'User-Agent': 'Bored programmer\'s bot'})

raw_data = urllib2.urlopen(request)
data = json.load(raw_data)

# Selecting random post
post = data["data"]["children"][randint(0, 24)]["data"]

print(post["title"] + "\n\n" + post["selftext"])
