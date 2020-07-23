<?php
$json = file_get_contents('php://input');
$data = json_decode($json,true);
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "biometric_attendance";
$lec_id=$data['Lecture_Id'];
$fp_id=$data['fp_id'];
//echo $data['Lecture_Id'];
//echo $data['fp_id'];
// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "INSERT INTO `attandence`(`lec_id`, `student_id`) VALUES (".$lec_id.",".$fp_id.");";
if ($conn->query($sql) === TRUE) {
    echo "Record updated successfully";
} else {
    echo "Error updating record: " . $conn->error;
}

$conn->close();
?>
