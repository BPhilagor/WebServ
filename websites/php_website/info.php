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

	<?php
		//echo '$argc:  '.$argc.' $argv: ';
		echo $argc;
		echo $argv;
		//vardump($argv);
	?>

	<h1> Contents of the $_SERVER variable </h1>
	<table>
		<ul>
		<li>PHP_SELF = <?php echo $_SERVER["PHP_SELF"] ?? "UNDEFINED"; ?></li>
		<li>ARGV = <?php echo $_SERVER["ARGV"] ?? "UNDEFINED"; ?></li>
		<li>ARGC = <?php echo $_SERVER["ARGC"] ?? "UNDEFINED"; ?></li>
		<li>GATEWAY_INTERFACE = <?php echo $_SERVER["GATEWAY_INTERFACE"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_ADDR = <?php echo $_SERVER["SERVER_ADDR"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_NAME = <?php echo $_SERVER["SERVER_NAME"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_SOFTWARE = <?php echo $_SERVER["SERVER_SOFTWARE"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_PROTOCOL = <?php echo $_SERVER["SERVER_PROTOCOL"] ?? "UNDEFINED"; ?></li>
		<li>REQUEST_METHOD = <?php echo $_SERVER["REQUEST_METHOD"] ?? "UNDEFINED"; ?></li>
		<li>REQUEST_TIME = <?php echo $_SERVER["REQUEST_TIME"] ?? "UNDEFINED"; ?></li>
		<li>REQUEST_TIME_FLOAT = <?php echo $_SERVER["REQUEST_TIME_FLOAT"] ?? "UNDEFINED"; ?></li>
		<li>QUERY_STRING = <?php echo $_SERVER["QUERY_STRING"] ?? "UNDEFINED"; ?></li>
		<li>DOCUMENT_ROOT = <?php echo $_SERVER["DOCUMENT_ROOT"] ?? "UNDEFINED"; ?></li>
		<li>HTTPS = <?php echo $_SERVER["HTTPS"] ?? "UNDEFINED"; ?></li>
		<li>REMOTE_ADDR = <?php echo $_SERVER["REMOTE_ADDR"] ?? "UNDEFINED"; ?></li>
		<li>REMOTE_HOST = <?php echo $_SERVER["REMOTE_HOST"] ?? "UNDEFINED"; ?></li>
		<li>REMOTE_PORT = <?php echo $_SERVER["REMOTE_PORT"] ?? "UNDEFINED"; ?></li>
		<li>REMOTE_USER = <?php echo $_SERVER["REMOTE_USER"] ?? "UNDEFINED"; ?></li>
		<li>REDIRECT_REMOTE_USER = <?php echo $_SERVER["REDIRECT_REMOTE_USER"] ?? "UNDEFINED"; ?></li>
		<li>SCRIPT_FILENAME = <?php echo $_SERVER["SCRIPT_FILENAME"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_ADMIN = <?php echo $_SERVER["SERVER_ADMIN"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_PORT = <?php echo $_SERVER["SERVER_PORT"] ?? "UNDEFINED"; ?></li>
		<li>SERVER_SIGNATURE = <?php echo $_SERVER["SERVER_SIGNATURE"] ?? "UNDEFINED"; ?></li>
		<li>PATH_TRANSLATED = <?php echo $_SERVER["PATH_TRANSLATED"] ?? "UNDEFINED"; ?></li>
		<li>SCRIPT_NAME = <?php echo $_SERVER["SCRIPT_NAME"] ?? "UNDEFINED"; ?></li>
		<li>REQUEST_URI = <?php echo $_SERVER["REQUEST_URI"] ?? "UNDEFINED"; ?></li>
		<li>PHP_AUTH_DIGEST = <?php echo $_SERVER["PHP_AUTH_DIGEST"] ?? "UNDEFINED"; ?></li>
		<li>PHP_AUTH_USER = <?php echo $_SERVER["PHP_AUTH_USER"] ?? "UNDEFINED"; ?></li>
		<li>PHP_AUTH_PW = <?php echo $_SERVER["PHP_AUTH_PW"] ?? "UNDEFINED"; ?></li>
		<li>AUTH_TYPE = <?php echo $_SERVER["AUTH_TYPE"] ?? "UNDEFINED"; ?></li>
		<li>PATH_INFO = <?php echo $_SERVER["PATH_INFO"] ?? "UNDEFINED"; ?></li>
		<li>ORIG_PATH_INFO = <?php echo $_SERVER["ORIG_PATH_INFO"] ?? "UNDEFINED"; ?></li>
		<li>POTATO = <?php echo $_SERVER["potato"] ?? "UNDEFINED"; ?></li>
</ul>
	</table>
	<h2>Contents of $_ENV</h2>
	<p><?php print_r($_ENV);?>

	<h1> Content of the $_COOKIE variable </h1>
	<p> <?php print_r($_COOKIE); ?> </p>
</body>
</html>