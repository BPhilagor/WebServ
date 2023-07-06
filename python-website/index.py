import os

def generate_directory_index(directory_path):
	# Generate HTML for directory listing
	content = '<h1>Directory For Python WebSite</h1>'
	content += '<h3>This is an example site where everything is generated with python</h2>'
	content += '<p>'
	for item in os.listdir(directory_path):
		item_path = os.path.join(directory_path, item)
		if os.path.isdir(item_path):
			item += '/'
		content += f'<li><a href="{item}">{item}</a></li>'
	content += '</p>'

	# Generate HTML
	html = f'''
	<!DOCTYPE html>
	<html>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" type="text/css" href="styles.css">
	<head>
		<title>Directory For Python WebSite</title>
	</head>
	<body>
		{content}
	</body>
	</html>
	'''
	print(html)

directory_path = '/Users/znichola/Documents/WebServ/python-website/'

print("Content-Type: text/html; charset=UTF-8")
print("")



generate_directory_index(directory_path)
