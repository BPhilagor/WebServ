#!/usr/bin/env python
import cgi

def generate_html_page(title, content, server_name):
	html = '''
<!DOCTYPE html>
<html>
<head>
	<title>%s</title>
</head>
<body>
	<h1>%s</h1>
	<p>%s</p>
	<p>server name is: %s</p>
	<a href="/current_time.py"> click for time </a>
</body>
</html>
	''' % (title, title, content, server_name)
	return html
# Read form data
form = cgi.FieldStorage()
page_title = form.getvalue('title', 'My Generated Page')
page_content = form.getvalue('content', 'This is a dynamically generated HTML page.')
server_name = form.getvalue('SERVER_SOFTWARE')

# Generate HTML page
generated_html = generate_html_page(page_title, page_content, server_name)

# Print CGI header and generated HTML
print("Content-Type: text/html")
# print("Content-length: " + str(len(generated_html)))
print("")
print(generated_html)
