<?php
	// send data to the arduino
	//ini_set('display_errors', 5);
	include 'bdd.php';

	$con=mysqli_connect($host, $user, $pass, $bdd);

	if (mysqli_connect_errno()) {
	  //echo "Failed to connect to MySQL: " . mysqli_connect_error();
	}

	$result = mysqli_query($con,"select max(ID) as ID from session");
	$row = mysqli_fetch_array($result);
	$id = $row['ID'];

	$result1 = mysqli_query($con,"select BBQ_TEMP,FOOD_TEMP,END from session where ID='".$id."'");
	$row1 = mysqli_fetch_array($result1);
	$bbqtemp= $row1['BBQ_TEMP'];
	$foodtemp= $row1['FOOD_TEMP'];
	$end= $row1['END'];

	echo "|";
	echo $id;
	echo "|";
	echo $bbqtemp;
	echo "|";
	echo $foodtemp;
	echo "|";
	echo $end;
	echo "|";

?>
