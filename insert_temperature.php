<?php

if(isset($_GET["rom1"], $_GET["temperature"])) {
   $chipAddress = $_GET["rom1"]; // get temperature value from HTTP GET
   $temperature = $_GET["temperature"]; // get temperature value from HTTP GET

   $servername = "localhost";
   $username = "esp32";
   $password = "GrainGuardian";
   $database_name = "tempReadings";

   // Create MySQL connection fom PHP to MySQL server
   $connection = new mysqli($servername, $username, $password, $database_name);
   // Check connection
   if ($connection->connect_error) {
      die("MySQL connection failed: " . $connection->connect_error);
   }

   $sql = "INSERT INTO testReadings(temp_value, chipAddress) 
		   VALUES ($temperature, chipAddress)";

   if ($connection->query($sql) === TRUE) {
      echo "New record created successfully";
   } else {
      echo "Error: " . $sql . " => " . $connection->error;
   }

   $connection->close();
} else {
   echo "temperature is not set in the HTTP request";
}
?>