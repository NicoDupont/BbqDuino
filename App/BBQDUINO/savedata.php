<?php
	// save data sent by the arduino
	ini_set('display_errors', 5);
  if($_GET){

		$session_id = $_GET['session'];
    $sensor_id = $_GET['id'];
    $temp_celcius = $_GET['temp'];
    $temp_far=(($temp_celcius * (1.8)) + 32);

    if(isset($temp_celcius) && isset($sensor_id) && isset($session_id)) {
			include 'bdd.php';
      try
      	{
      		$bdd = new PDO("mysql:host=$host;dbname=$bdd;charset=utf8", $user, $pass);
					$bdd->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      	}
      catch(Exception $e)
      	{
          die('Erreur : '.$e->getMessage());
      	}
      $qry = $bdd->prepare('INSERT INTO temperature (SENSOR_ID, CELCIUS, FAHRENHEIT, DATE_TEMP, TIME_TEMP, MOIS_TEMP, YEAR_TEMP, SESSION_ID)
                  VALUES( :sensor_id, :temp_celcius, :temp_far, CURDATE(), CURTIME(), MONTH(CURDATE()), YEAR(CURDATE()), :session_id)');

			$qry->execute(array('sensor_id' => $sensor_id,
													'temp_celcius' => $temp_celcius,
													'temp_far' => $temp_far,
													'session_id' => $session_id
												));
    }
  }
?>
