def generate_html():
	# Generate HTML content
	html = '''
	<!DOCTYPE html>
	<html>
	<head>
		<title>Infinite Loop</title>
	</head>
	<body>
		<h1>Infinite Loop</h1>
		<p>This page demonstrates an infinite loop.</p>
	</body>
	</html>
	'''
	return html

html_content = generate_html()

print ("Content-Type: text/html")
print ()
print(html_content.crash)
