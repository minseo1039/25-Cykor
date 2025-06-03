<?php
session_start();

$conn = mysqli_connect("db", "root", "1234", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

$username = $_POST['username'];
$password = $_POST['password'];

$sql = "SELECT * FROM users WHERE username='$username' AND password='$password'";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) === 1) {
    $_SESSION['username'] = $username;
    echo "login Success!";
    echo "<br><a href='./index.php'>Main Page</a>";
} else {
    echo "Failed to login.";
    echo "<br><a href='./login.php'>Login Page</a>";
}

mysqli_close($conn);
?>
