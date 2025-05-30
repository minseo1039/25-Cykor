<?php
session_start();
$conn = mysqli_connect("localhost", "root", "1234", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

$title = $_POST['title'];
$content = $_POST['content'];
$username = $_SESSION['username'];

$sql = "INSERT INTO posts (title, content, writer) 
        VALUES ('$title', '$content', '$username')";

if (mysqli_query($conn, $sql)) {
    echo "Good!";
    echo "<br><a href='list.php'>Back to list</a>";
}

mysqli_close($conn);
?>
