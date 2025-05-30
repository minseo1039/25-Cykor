<?php
$conn = mysqli_connect("localhost", "root", "", "cykor_db");

if (!$conn) {
    die("DB fail: " . mysqli_connect_error());
}

$title = $_POST['title'];        
$new_title = $_POST['new_title'];
$content = $_POST['content'];

$sql = "UPDATE posts SET title = '$new_title', content = '$content' WHERE title = '$title'";

if (mysqli_query($conn, $sql)) {
    echo "Edit successful!<br>";
    echo "<br><a href='list.php?title=" . $new_title . "'>Back to list</a>";
}

mysqli_close($conn);
?>
