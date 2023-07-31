<?php

if(!isset($_COOKIE["login"]))// $_COOKIE is a variable and login is a cookie name

	header("location:login.php");

?>

<h1>Hey ! welcome to main page .</h1>

<a href="page1.php"><h2><font color="">PAGE 1</font></h2>
<a href="page2.php"><h2><font color="">PAGE 2</font></h2>
<a href="page3.php"><h2><font color="">PAGE 3</font></h2>

<a href="logout.php"><h2><font color="red">Logout</font></h2>
