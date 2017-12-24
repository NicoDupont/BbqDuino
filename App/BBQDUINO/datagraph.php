<?php

	header('Content-type: application/json');
	ini_set('display_errors', 5);
	include 'bdd.php';

	$con=mysqli_connect($host, $user, $pass, $bdd);

	if (mysqli_connect_errno()) {
	  echo "Failed to connect to MySQL: " . mysqli_connect_error();
	}

  $result = mysqli_query($con,"select ID,SESSION_TIME,FOOD_TEMP,BBQ_TEMP from session where ID = (select max(ID) as ID from session)");
  $row = mysqli_fetch_assoc($result);
  $start = $row['SESSION_TIME'];
  $id = $row['ID'];
	$ft = $row['FOOD_TEMP'];
	$bt = $row['BBQ_TEMP'];

  $result = mysqli_query($con,"select SENSOR_ID as SENSOR,CELCIUS,REPLACE(SUBSTR(TIME_TEMP,1,5),':','H') as TIME, '".$bt."' as BBQ, '".$ft."' as FOOD
															from temperature where SESSION_ID = '".$id."' and TIME_TEMP  >= '".$start ."' order by SENSOR_ID,TIME_TEMP");
  $rows = array();
	while($r = mysqli_fetch_assoc($result)) {
    $rows[] = $r;
	}
  mysqli_close($con);
	$jsondata = json_encode($rows);
	//file_put_contents('datagraph.json', $jsondata);
	echo $jsondata;

?>
