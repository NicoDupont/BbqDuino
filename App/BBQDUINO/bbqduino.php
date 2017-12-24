<?php
	header('Content-type: application/json');
	ini_set('display_errors', 5);
	include 'bdd.php';

	$con=mysqli_connect($host, $user, $pass, $bdd);

	if (mysqli_connect_errno()) {
	  echo "Failed to connect to MySQL: " . mysqli_connect_error();
	}

	$result = mysqli_query($con,"select s.DESCRIPTION, s.END, s.SESSION_TSTAMP,s.SESSION_TIME as START_TIME,s.BBQ_TEMP as BBQ_TARGET,s.FOOD_TEMP as FOOD_TARGET,b.ID as BBQID, b.NAME as BBQ_NAME,m.ID as MEATID, m.NAME as MEAT_NAME, air.CELCIUS as AIR_TEMP,bbq.CELCIUS as BBQ_TEMP,food.CELCIUS as FOOD_TEMP,s.ID as SESSION
from session as s
left outer join bbq as b
on s.BBQ_ID=b.ID
left outer join meat as m
on s.MEAT_ID=m.ID
left outer join (select SESSION_ID,CELCIUS
				from temperature as a
				inner join (select SENSOR_ID,max(ID) as id
							from temperature
							where SESSION_ID = (select max(ID) as ID from session)
							group by SENSOR_ID) as b
				on a.id=b.id and b.sensor_id=1) as air
on s.ID=air.SESSION_ID
left outer join (select SESSION_ID,CELCIUS
				from temperature as a
				inner join (select SENSOR_ID,max(ID) as id
							from temperature
							where SESSION_ID = (select max(ID) as ID from session)
							group by SENSOR_ID) as b
				on a.id=b.id and b.sensor_id=2) as bbq
on s.ID=bbq.SESSION_ID
left outer join (select SESSION_ID,CELCIUS
				from temperature as a
				inner join (select SENSOR_ID,max(ID) as id
							from temperature
							where SESSION_ID = (select max(ID) as ID from session)
							group by SENSOR_ID) as b
				on a.id=b.id and b.sensor_id=3) as food
on s.ID=food.SESSION_ID
where s.ID =(select max(ID) as ID from session);");
	$row = mysqli_fetch_assoc($result);
	mysqli_close($con);
	$jsondata = json_encode($row);
	echo $jsondata;

	/*
	$SESSION = $row['SESSION'];
	$START = $row['START_TIME'];
	$BBQ_TARGET = $row['BBQ_TARGET'];
	$BBQ_TEMP = $row['BBQ_TEMP'];
	$FOOD_TARGET = $row['FOOD_TARGET'];
	$FOOD_TEMP = $row['FOOD_TEMP'];
	$AIR_TEMP= $row['AIR_TEMP'];
	$MEAT_NAME= $row['MEAT_NAME'];
	$BBQ_NAME= $row['BBQ_NAME'];

	echo $SESSION;
	echo $START;
	echo $BBQ_TARGET;
	echo $BBQ_TEMP;
	echo $FOOD_TARGET;
	echo $FOOD_TEMP;
	echo $AIR_TEMP;
	echo $MEAT_NAME;
	echo $BBQ_NAME;
	*/

?>
