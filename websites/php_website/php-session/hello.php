<?php
	session_start();

	if (!isset($_SESSION['color']))
		$color = "#0000FF";
	else
		$color = $_SESSION['color'];
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>PHP SESSIONS</title>
</head>
<body>
	<style>
		body {
			background-color: <?php echo $color; ?>;
		}
	</style>
	<h1>hello</h1>
</body>
</html>
