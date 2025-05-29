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
    <h1>List Page</h1>
    <h2>Welcome to Song Min Seo's Cykor Week 3</h2>
    <ul>
        <li><a href = "./index.php">Main</a></li>
        <li><a href = "./login.php">Login</a></li>
        <li><a href = "./register.php">Register</a></li>
        <li><a href = "./list.php">List</a></li>
    </ul>

    <?php
    if (isset($_SESSION['username'])) {
        echo "<p>Creat the your context!</p>";
        echo "<a href='write.php'>Write</a>";
    }
    ?>

    <h3>List</h3>
</body>
</html>