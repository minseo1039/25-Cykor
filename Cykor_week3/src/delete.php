<?php
$conn = mysqli_connect("db", "root", "1234", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

if (!isset($_GET['title'])) {
    die("Nothing to delete.");
}

$title = $_GET['title'];

$sql = "DELETE FROM posts WHERE title = '$title'";

if (mysqli_query($conn, $sql)) {
    echo "Detete successful!<br>";
    echo "<br><a href='list.php'>Back to list</a>";
}

mysqli_close($conn);
?>
