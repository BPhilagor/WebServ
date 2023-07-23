#!/usr/bin/env python

import datetime

html = """
<html>
<head>
    <title>Current Time</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f7f7f7;
            text-align: center;
        }

        h1 {
            color: #333;
        }

        p {
            color: #777;
            font-size: 18px;
            margin-bottom: 20px;
        }

        a {
            color: #4CAF50;
            text-decoration: none;
        }
    </style>
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
