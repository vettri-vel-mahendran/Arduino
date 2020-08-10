<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "id12171130_example_esp_data";
// REPLACE with Database user
$username = "id12171130_example_esp_board";
// REPLACE with Database user password
$password = "123456789";

$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $Machine_Number = $Timestamp = $current = $voltage = $power = $energy = $frequency =$pf = $ontime = $offtime = $runtime = $rpm ="";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $Machine_Number = test_input($_POST["Machine_Number"]);
        $Timestamp = test_input($_POST["Timestamp"]);
        $current = test_input($_POST["current"]);
        $voltage = test_input($_POST["voltage"]);
        $power = test_input($_POST["power"]);
        $energy = test_input($_POST["energy"]);
        $frequency = test_input($_POST["frequency"]);
        $pf = test_input($_POST["pf"]);
        $ontime = test_input($_POST["ontime"]);
        $offtime = test_input($_POST["offtime"]);
        $runtime = test_input($_POST["runtime"]);
        $rpm = test_input($_POST["rpm"]);
        
        // Create connection
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Check connection
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (Machine_Number, Timestamp, current, voltage, power, energy, frequency, pf, ontime, offtime, runtime,rpm)
        VALUES ('" . $Machine_Number . "', '" . $Timestamp . "', '" . $current . "', '" . $voltage . "', '" . $power . "', '" . $energy . "', '" . $frequency . "', '" . $pf . "','" . $ontime . "','" . $offtime . "','" . $runtime . "','" . $rpm . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}