<?php
session_start();
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Cykor Week 3</title>
</head>
<body>
    <h1>Write Page</h1>
    <form action="write_process.php" method="POST">
        <label>Title:</label><br>
        <input type="text" name="title"><br><br>
        <label>Content:</label><br>
        <textarea name="content"></textarea><br><br>
        <button type="submit">Post</button>
    </form>
    <br>
    <a href="list.php">Back to List</a>
</body>
</html>
