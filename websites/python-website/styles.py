import os
import re

def gen_style(bg_color):
	css = '''
body {
	font-family: Arial, sans-serif;
	text-align: center;
	background-color: %s;
	margin: 0;
	padding: 0;
}

h1 {
	color: #333;
	margin-top: 50px;
}

h3 {
	color: #545454;
	margin-top: 50px;
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

button {
	background-color: #4CAF50;
	border: none;
	color: white;
	padding: 10px 20px;
	text-align: center;
	text-decoration: none;
	display: inline-block;
	font-size: 16px;
	margin-top: 20px;
	cursor: pointer;
	border-radius: 5px;
	transition: background-color 0.3s ease;
}

button:hover {
	background-color: #45a049;
}

.refresh-link {
	display: block;
	margin-top: 20px;
}

.subscript {
	font-size: 12px;
	color: #999;
	margin-top: 50px;
	text-align: center;
}
''' % (bg_color)
	return css

env = os.environ
cookies = env.get('HTTP_COOKIE')
bg_color = '#f7f7f7'

match = []

if cookies is not None:
	pattern = r'bg_color=([^;]+)'
	match = re.search(pattern, cookies)
	if match:
		bg_color = match.group(1)

# Print CGI header and generated CSS
print("Content-Type: text/css; charset=UTF-8")
print("")
print(gen_style(bg_color))
