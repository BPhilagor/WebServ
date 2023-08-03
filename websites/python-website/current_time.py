#!/usr/bin/env python

import datetime

html = """
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<link rel="stylesheet" type="text/css" href="styles.py">
<title>Time</title>
</head>
<body>
    <h1>Current Time</h1>
    <p>The current time is: """ + str(datetime.datetime.now()) + """</p>
    <a href="/current_time.py">Click to refresh the time</a>
</body>
</html>
"""

print ("Content-Type: text/html")
print ()
print(html)
