<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>A PHP website</title>
	<link rel=stylesheet href="/style.css">
</head>
<body>
<ul>
		<li><a href="/index.php">Home</a></li>
		<li><a href="/form.html">Forms are cool</a></li>
		<li><a href="/hello.php">Let's say hello</a></li>
		<li><a href="/info.php">Here some $_SERVER variables</a></li>
	</ul>
	<h1><?php echo "hello ".$_POST['name']; ?></h1>
	<p>
			<table>
			<tr><td>$_POST array: </td> <td> <?php var_dump($_POST); ?></td></tr>
			<tr><td>$_GET array:  </td> <td> <?php var_dump($_GET); ?></td></tr>
			</table>
			
	</p>
</body>
</html>