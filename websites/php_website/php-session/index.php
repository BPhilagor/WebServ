<?php
	session_start();

	if (isset($_POST['color']))
	{
		$_SESSION['color'] = $_POST['color'];
	}


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
	<form method="POST" action="/php-session/index.php" enctype="multipart/form-data">
		<label>Choose a color:</label>
		<input type="color" name="color">
		<input type="submit">
	</form>
</body>
</html>
