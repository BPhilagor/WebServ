<?php
	if (isset($_POST['submit']))
	{
		$target_dir = "./uploads/";
		$target_file = $target_dir . basename($_FILES['file']['name']);
		if (move_uploaded_file($_FILES['file']['tmp_name'], $target_file))
		{
			$status = "ok";
		}
		else
		{
			$status = "failure";
		}
	}
?>
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>PHP uploads</title>
</head>
<body>
	<style>
		body {
			background-color: <?php echo "#" . dechex(rand(0, 1 << 31)); ?> ;
		}
	</style>
	<h1>PHP uploads</h1>
		<?php
		if (isset($status))
		{
			if ($status == "ok")
			{
				echo "<div style='color:green;'>File upload was successfully uploaded at ".$target_file,"</div>";
			}
			else
			{
				echo "<div style='color:red;'>Something went wrong with the file upload</div>";
			}
		}
		?>
	<form method="POST" action="/index.php" enctype="multipart/form-data">
		<label>Upload a file: </label>		<input type="file" name="file">
		<input type="submit" name="submit">
	</form>
</body>
</html>