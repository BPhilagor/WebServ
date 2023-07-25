<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>A PHP website</title>
</head>
<body>
	<h1>Current directory structure</h1>
	<p>Obtained with PHP's getcwd() and scandir() functions</p>
	<h2>Index of: <?php echo getcwd();?> </h2>
	<?php print_r(scandir(".")); ?>
</body>
</html>