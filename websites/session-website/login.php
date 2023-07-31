<html>
<fieldset>
<legend>Login </legend>
<form action="loginprocess.php" method="POST"><br><br>
Username:<input type="text" required="" name="uname"><br><br>
Password:<input type="text" required="" name="upassword"><br><br>
<input type="checkbox" value="1" name="remember">keep me login:
<input type="submit" value="Login" name="sub">
<br>
<?php
if(isset($_REQUEST["err"]))
	$msg="Invalid username or Password";
?>
<p style="color:red;">
<?php if(isset($msg))
{

echo $msg;
}
?>
</p>


</form>

</fieldset>
</center>
</html>
