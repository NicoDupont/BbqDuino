<?php
  if($_POST){

    date_default_timezone_set("Europe/Paris");
		$session_id = $_POST['ID'];
    $today = date("Y-m-d");
    $hour = date("H:i:s");
    /*
    echo $today;
    echo "\n";
    echo $time;
    */
    if(isset($session_id)) {
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
      $qry = $bdd->prepare('UPDATE session SET END = 1, END_SESSION_DATE = :today, END_SESSION_TIME = :hour WHERE ID = :session_id');

			$qry->execute(array('today' => $today,
                          'hour' => $time,
													'session_id' => $session_id
												));
      echo "ok";
    }else {
      echo "ko";
    }
  }
  else {
    echo "ko";
  }
?>
