<?php
  if($_POST){

    date_default_timezone_set("Europe/Paris");
    $bbq = $_POST['bbq'];
    $meat = $_POST['meat'];
    $foodtarget = $_POST['foodtarget'];
    $bbqtarget = $_POST['bbqtarget'];
    $description = $_POST['description'];

    $today = date("Y-m-d");
    $year = date("Y");
    $month = date("m");
    $hour = date("H:i:s");

    if (!empty($bbq) and !empty($meat) and !empty($foodtarget) and !empty($bbqtarget) and !empty($description)) {

			include 'bdd.php';
      $con=mysqli_connect($host, $user, $pass, $bdd);
    	if (mysqli_connect_errno()) {
    	  //echo "Failed to connect to MySQL: " . mysqli_connect_error();
        echo "ko";
    	}else { //bdd ok
        // query1
      	$result = mysqli_query($con,"select ID,END from session where ID = (select max(ID) as ID from session)");
      	$row = mysqli_fetch_array($result);
      	$end = $row['END'];
        $id = $row['ID'];

        if ($end == 1) {
          //add session
          mysqli_query($con,"INSERT INTO session (SESSION_DATE, SESSION_TIME, SESSION_MONTH,  SESSION_YEAR, DESCRIPTION, MEAT_ID, BBQ_TEMP, FOOD_TEMP, BBQ_ID, END_SESSION_DATE, END_SESSION_TIME, END)
          VALUES ('".$today ."', '".$hour ."', '".$month."', '".$year."', '".$description."', '".$meat."', '".$bbqtarget."', '".$foodtarget."', '".$bbq."', '".$today ."',  '".$hour ."', 0)");
        }else {
          //modify session
          mysqli_query($con,"UPDATE session SET MEAT_ID = '".$meat."',BBQ_ID = '".$bbq."',BBQ_TEMP = '".$bbqtarget."', FOOD_TEMP = '".$foodtarget."', DESCRIPTION = '".$description."' WHERE ID = '".$id."'");
        }
        mysqli_close($con);
        echo "ok";
      }

    }else { //data ko
      echo "ko";
    }

  }else {  //post ko
    echo "ko";
  }

?>
