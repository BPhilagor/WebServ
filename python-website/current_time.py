#!/usr/bin/env python

import datetime

print ("Content-Type: text/html", end='\r\n')
print (end='\r\n')


print( "<html><body>")
print ("<h1>Current time</h1>")
print ("<p>The current time is: " + str(datetime.datetime.now()) + "</p>")
print ('<a href="/current_time.py"> click to refresh the time </a>')
print ("</body></html>")
