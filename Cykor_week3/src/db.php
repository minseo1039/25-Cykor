<?php
$host = 'localhost';
$dbname = 'your_database';
$user = 'your_username';
$pass = 'your_password';

$conn = new mysqli($host, $user, $pass, $dbname);

if ($conn->connect_error) {
    die("DB fail: " . $conn->connect_error);
}
?>