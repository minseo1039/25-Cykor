<?php
$conn = mysqli_connect("localhost", "root", "", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

$username = $_POST['username'];
$password = $_POST['password'];

$sql = "INSERT INTO users (username, password) VALUES ('$username', '$password')";

if (mysqli_query($conn, $sql)) {
    echo "Registration successful!";
    echo "<br><a href='./login.php'>Login Page</a>";
}

mysqli_close($conn);
?>
