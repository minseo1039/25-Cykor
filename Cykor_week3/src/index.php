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
    <h1>Main Page</h1>
    <h2>Welcome to Song Min Seo's Cykor Week 3</h2>
    <ul>
        <li><a href = "./index.php">Main</a></li>
        <?php
        if (isset($_SESSION['username'])) {
            echo "<li><a href='./logout.php'>Logout</a></li>";
        }
        else {
            echo "<li><a href='./login.php'>Login</a></li>";
            echo "<li><a href='./register.php'>Register</a></li>";
        }
        ?>
        <li><a href = "./list.php">List</a></li>
    </ul>

    <?php
    if (isset($_SESSION['username'])) {
        echo "<p>Wow!, " . htmlspecialchars($_SESSION['username']) . "!</p>";
    }
    ?>
</body>
</html>
