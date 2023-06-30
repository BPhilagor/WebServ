#!/bin/bash

add_para ()
{
	echo '<p>'
	printf "$@"
	echo '</p>'
}

echo "Content-Type: text/html"
echo

printf '
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<link rel="stylesheet" type="text/css" href="styles.css">
	<title>This basic site</title>
</head>
<body>
'
	add_para $(pwd)
	git log --oneline --decorate | xargs -I{} echo "<p>" {} "</p>"

printf '
</body>
</html>
'
