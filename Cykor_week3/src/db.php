<?php
$host = 'localhost';
$dbname = 'cykor_db';
$user = 'root';
$pass = '1234';

$conn = new mysqli($host, $user, $pass, $dbname);

if ($conn->connect_error) {
    die("DB fail: " . $conn->connect_error);
}
?>