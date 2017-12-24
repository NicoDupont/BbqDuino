<?php
	header('Content-type: application/json');
	ini_set('display_errors', 5);
	include 'bdd.php';
	$con=mysqli_connect($host, $user, $pass, $bdd);
	if (mysqli_connect_errno()) {
	  echo "Failed to connect to MySQL: " . mysqli_connect_error();
	}
	$result = mysqli_query($con,"select ID,NAME from bbq");
	$rows = array();
	while($r = mysqli_fetch_array($result)) {
    $rows[] = $r;
	}
	mysqli_close($con);
	$jsondata = json_encode($rows);
	echo $jsondata;
?>
